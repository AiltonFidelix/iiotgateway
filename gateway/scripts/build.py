#!/usr/bin/env python3

import argparse
import shutil
import subprocess
from pathlib import Path
import multiprocessing

SUPPORTED_ARCHS = ["amd64", "aarch64"]
QT_VERSION = "6.8.3"


def run(cmd, cwd=None):
    subprocess.run(cmd, check=True, cwd=cwd)


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
    docker_image = f"qt-aarch64:{qt_version}"

    project_root = Path.cwd()
    docker_dir = f"/src/{build_dir}"

    run(
        [
            "docker",
            "run",
            "--rm",
            "-v",
            f"{project_root}:/src",
            "-w",
            docker_dir,
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
            docker_dir,
            docker_image,
            "cmake",
            "--build",
            ".",
            "--parallel",
            jobs,
        ]
    )


def main():

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
