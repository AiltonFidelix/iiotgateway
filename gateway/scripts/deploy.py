#!/usr/bin/env python3

import sys
import shutil
import subprocess
from pathlib import Path

ARCHS = ["aarch64"]

APP_VERSION = "1.0.0"
APP_NAME = "gateway"
QT_VERSION = "6.8.3"

PROJECT_DIR = Path(__file__).resolve().parent.parent

DEPLOY_PATH = (
    PROJECT_DIR
    / ".."
    / "distro"
    / "board"
    / "iiotgateway"
    / "raspberrypi3-64"
    / "rootfs_overlay"
).resolve()

EXECUTABLE_INSTALL_PATH = (DEPLOY_PATH / "opt" / "iiotgateway" / "bin").resolve()

LIBRARIES_INSTALL_PATH = (DEPLOY_PATH / "usr" / "lib").resolve()

PLUGINS_INSTALL_PATH = (DEPLOY_PATH / "usr" / "lib" / "qt6" / "plugins").resolve()

SQL_PLUGINS_INSTALL_PATH = (PLUGINS_INSTALL_PATH / "sqldrivers").resolve()

QT_LIBRARIES = [
    "libQt6Core",
    "libQt6HttpServer",
    "libQt6Sql",
    "libQt6Network",
    "libQt6WebSockets",
    "libQt6SerialBus",
    "libQt6SerialPort",
]

QT_LIBRARIES_PATH = f"/Qt/{QT_VERSION}/arm64/lib"


def options():
    print("Usage: ./deploy.py [OPTION]")
    print(f"  Options: {' '.join(ARCHS)}")
    sys.exit(0)


def run(cmd, cwd=None, stdout=None):
    subprocess.run(cmd, check=True, cwd=cwd, stdout=stdout)


def copy_shared_library_from_docker(docker_image, path, file_pattern, dest_path):

    temp_tar = Path("libs.tar.gz")

    with open(temp_tar, "wb") as f:
        run(
            [
                "docker",
                "run",
                "--rm",
                docker_image,
                "bash",
                "-c",
                f"cd {path} && tar czf - {file_pattern}",
            ],
            stdout=f,
        )

    run(["tar", "xzf", str(temp_tar), "-C", str(dest_path)])
    temp_tar.unlink()


def main():
    args = sys.argv[1:]

    if len(args) == 0:
        options()

    arch = args[0]

    if arch not in ARCHS:
        options()

    docker_image = f"qt-{arch}:{QT_VERSION}"

    run(["python3", "scripts/build.py", arch, "-c"])

    shutil.copy(f"build-{arch}/Main/{APP_NAME}", EXECUTABLE_INSTALL_PATH)

    for pattern in [
        f"build-{arch}/Application/libApplication.so*",
        f"build-{arch}/Communication/libCommunication.so*",
        f"build-{arch}/Device/libDevice.so*",
    ]:
        for file in Path().glob(pattern):
            shutil.copy(file, LIBRARIES_INSTALL_PATH)

    for qtlib in QT_LIBRARIES:
        copy_shared_library_from_docker(
            docker_image, QT_LIBRARIES_PATH, f"{qtlib}.so*", LIBRARIES_INSTALL_PATH
        )

    copy_shared_library_from_docker(
        docker_image,
        f"/Qt/{QT_VERSION}/arm64/plugins/sqldrivers",
        "libqsqlite.so",
        SQL_PLUGINS_INSTALL_PATH,
    )

    copy_shared_library_from_docker(
        docker_image,
        "/qt/arm64/sysroot/usr/lib",
        "libwiringPi.so*",
        LIBRARIES_INSTALL_PATH,
    )

    copy_shared_library_from_docker(
        docker_image,
        "/qt/arm64/sysroot/usr/local/lib",
        "libpaho-mqttpp3.so*",
        LIBRARIES_INSTALL_PATH,
    )

    copy_shared_library_from_docker(
        docker_image,
        "/qt/arm64/sysroot/usr/local/lib",
        "libpaho-mqtt3as.so*",
        LIBRARIES_INSTALL_PATH,
    )

    copy_shared_library_from_docker(
        docker_image,
        "/qt/arm64/sysroot/lib/aarch64-linux-gnu",
        "libicu*.so*",
        LIBRARIES_INSTALL_PATH,
    )


if __name__ == "__main__":
    main()
