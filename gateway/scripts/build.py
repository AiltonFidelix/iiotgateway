#!/usr/bin/env python3

import sys
import shutil
import subprocess
from pathlib import Path

ARCHS = ["amd64", "aarch64"]
QT_VERSION = "6.8.3"


def options():
    print("Usage: ./build.py [OPTION]")
    print(f"  Options: {' '.join(ARCHS)}")
    sys.exit(0)


def verify_build_folder(arch, flag):
    build_dir = Path(f"build-{arch}")

    if flag == "-c":
        shutil.rmtree(build_dir, ignore_errors=True)

    if not build_dir.exists():
        build_dir.mkdir()


def run(cmd, cwd=None):
    subprocess.run(cmd, check=True, cwd=cwd)


def main():
    args = sys.argv[1:]

    if len(args) == 0:
        options()

    arch = args[0]
    flag = args[1] if len(args) == 2 else None

    if arch not in ARCHS:
        options()

    docker_image = f"qt-{arch}:{QT_VERSION}"
    project_root = Path.cwd()

    if arch == "aarch64":
        verify_build_folder(arch, flag)

        build_path = f"/src/build-{arch}"

        run(
            [
                "docker",
                "run",
                "--rm",
                "-v",
                f"{project_root}:/src",
                "-w",
                build_path,
                docker_image,
                "qt-cmake",
                "-DCMAKE_BUILD_TYPE=Release",
                "-DENABLE_RASPBERRY_GPIO=ON",
                "-DENABLE_LINUX_REBOOT=ON",
                "..",
            ]
        )

        run(
            [
                "docker",
                "run",
                "--rm",
                "-v",
                f"{project_root}:/src",
                "-w",
                build_path,
                docker_image,
                "cmake",
                "--build",
                ".",
                "--parallel",
                "12",
            ]
        )

    elif arch == "amd64":
        verify_build_folder(arch, flag)

        build_dir = Path(f"build-{arch}")

        qt_cmake_path = Path.home() / "Qt" / QT_VERSION / "gcc_64" / "bin" / "qt-cmake"

        run([str(qt_cmake_path), "-DCMAKE_BUILD_TYPE=Release", ".."], cwd=build_dir)

        run(["cmake", "--build", ".", "--parallel", "12"], cwd=build_dir)


if __name__ == "__main__":
    main()
