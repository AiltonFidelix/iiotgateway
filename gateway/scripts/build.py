#!/usr/bin/env python3

import argparse
import shutil
import subprocess
from pathlib import Path
import multiprocessing

SUPPORTED_ARCHS = ["amd64", "aarch64"]
QT_VERSION = "6.8.3"

CONTAINER_ENGINE = ""


def run(cmd, cwd=None):
    try:
        subprocess.run(cmd, check=True, cwd=cwd)
    except Exception as e:
        print(e)
        print(f"\033[31m---> Failed running command: '{" ".join(cmd)}'")
        exit(1)


def clean(build_dir: Path):
    shutil.rmtree(build_dir, ignore_errors=True)


def check_build_folder(build_dir: Path):
    if not build_dir.exists():
        build_dir.mkdir()


def build_amd64(build_dir: Path, qt_version: str, jobs: str, with_test: bool = False):
    qt_cmake_path = Path.home() / "Qt" / qt_version / "gcc_64" / "bin" / "qt-cmake"

    cmake_cmd = [str(qt_cmake_path), "-DCMAKE_BUILD_TYPE=Release", ".."]

    if with_test:
        index = len(cmake_cmd) - 1
        cmake_cmd.insert(index, "-DENABLE_TESTS=ON")

    run(cmake_cmd, cwd=build_dir)

    run(["cmake", "--build", ".", "--parallel", jobs], cwd=build_dir)


def build_aarch64(build_dir: Path, qt_version: str, jobs: str, with_test: bool = False):
    qt_image = f"qt-aarch64:{qt_version}"

    project_root = Path.cwd()
    container_dir = f"/src/{build_dir}"

    run(
        [
            CONTAINER_ENGINE,
            "run",
            "--rm",
            "-v",
            f"{project_root}:/src:z",
            "-w",
            container_dir,
            qt_image,
            "qt-cmake",
            "-DCMAKE_BUILD_TYPE=Release",
            "-DENABLE_RASPBERRY_GPIO=ON",
            "-DENABLE_LINUX_REBOOT=ON",
            "..",
        ]
    )

    run(
        [
            CONTAINER_ENGINE,
            "run",
            "--rm",
            "-v",
            f"{project_root}:/src:z",
            "-w",
            container_dir,
            qt_image,
            "cmake",
            "--build",
            ".",
            "--parallel",
            jobs,
        ]
    )


def get_container_engine():
    tool_path = shutil.which("docker")

    global CONTAINER_ENGINE

    if tool_path:
        print(f"---> Using [docker] as container engine")
        CONTAINER_ENGINE = "docker"
        return

    tool_path = shutil.which("podman")

    if tool_path:
        print(f"---> Using [podman] as container engine")
        CONTAINER_ENGINE = "podman"
        return

    print("\033[31m---> No container engine found, please install [docker] or [podman]")
    exit(0)


def main():

    get_container_engine()

    parser = argparse.ArgumentParser(description="gateway build script")

    parser.add_argument(
        "--arch",
        choices=SUPPORTED_ARCHS,
        default="amd64",
        metavar="ARCH",
        help="Target architecture (default: amd64)",
    )

    archs_build = {
        "amd64": build_amd64,
        "aarch64": build_aarch64,
    }

    parser.add_argument(
        "--qt-version",
        default=QT_VERSION,
        metavar="VER",
        help=f"Qt framework version (default: {QT_VERSION})",
    )

    default_num_of_jobs = str(multiprocessing.cpu_count() - 4)

    parser.add_argument(
        "--jobs",
        default=default_num_of_jobs,
        metavar="JOBS",
        help=f"Number of parallel jobs (default: {default_num_of_jobs})",
    )

    parser.add_argument(
        "--clean", action="store_true", help="Clean previous build artifacts"
    )

    parser.add_argument(
        "--with-tests",
        action="store_true",
        help="Include test library in the build",
    )

    args = parser.parse_args()

    build_dir = Path(f"build-{args.arch}")

    if args.clean:
        clean(build_dir)

    check_build_folder(build_dir)
    archs_build[args.arch](build_dir, args.qt_version, args.jobs, args.with_tests)


if __name__ == "__main__":
    main()
