#!/usr/bin/env python3

import sys
import subprocess
import os
import shutil

QT_VERSION = "6.8.3"
COMPILER = "wasm_singlethread"


def run(cmd):
    subprocess.run(cmd, check=True)


def main():
    args = sys.argv[1:]

    if len(args) == 1 and args[0] == "-c":
        shutil.rmtree("build-wasm", ignore_errors=True)

    emsdk_env = os.path.expanduser("~/.emsdk/emsdk_env.sh")

    toolchain_path = os.path.expanduser(
        f"~/Qt/{QT_VERSION}/{COMPILER}/lib/cmake/Qt6/qt.toolchain.cmake"
    )

    prefix_path = os.path.expanduser(f"~/Qt/{QT_VERSION}/{COMPILER}")

    bash_script = f"""
        set -e
        source "{emsdk_env}"
        cmake -S . -B build-wasm \
            -DCMAKE_TOOLCHAIN_FILE="{toolchain_path}" \
            -DCMAKE_PREFIX_PATH="{prefix_path}"
        cmake --build build-wasm
    """

    run(["bash", "-c", bash_script])


if __name__ == "__main__":
    main()
