#!/usr/bin/env python3

import argparse
import subprocess
import os
import shutil
import multiprocessing

BUILD_DIR = "build-wasm"
QT_VERSION = "6.8.3"
COMPILER = "wasm_singlethread"


def run(cmd):
    subprocess.run(cmd, check=True)


def clean():
    shutil.rmtree(BUILD_DIR, ignore_errors=True)


def build(qt_version: str, asm_cc: str, jobs: str):
    emsdk_env = os.path.expanduser("~/.emsdk/emsdk_env.sh")

    toolchain_path = os.path.expanduser(
        f"~/Qt/{qt_version}/{asm_cc}/lib/cmake/Qt6/qt.toolchain.cmake"
    )

    prefix_path = os.path.expanduser(f"~/Qt/{qt_version}/{asm_cc}")

    bash_script = f"""
        set -e
        source "{emsdk_env}"
        cmake -S . -B {BUILD_DIR} \
            -DCMAKE_TOOLCHAIN_FILE="{toolchain_path}" \
            -DCMAKE_PREFIX_PATH="{prefix_path}"
        cmake --build {BUILD_DIR} --parallel {jobs}
    """

    run(["bash", "-c", bash_script])


def main():

    parser = argparse.ArgumentParser(description="dasboard build script")

    parser.add_argument(
        "--wasm-cc",
        default=COMPILER,
        metavar="WC",
        help=f"Web assembly compiler (default: {COMPILER})",
    )

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

    args = parser.parse_args()

    if args.clean:
        clean()

    build(args.qt_version, args.wasm_cc, args.jobs)


if __name__ == "__main__":
    main()
