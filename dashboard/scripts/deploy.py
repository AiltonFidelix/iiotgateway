#!/usr/bin/env python3

import shutil
import subprocess
from pathlib import Path

APP_NAME = "dashboard"
BUILD_PATH = Path("build-wasm")
PROJECT_DIR = Path(__file__).resolve().parent.parent

DEPLOY_PATH = (
    PROJECT_DIR
    / ".."
    / "distro"
    / "board"
    / "iiotgateway"
    / "raspberrypi3-64"
    / "rootfs_overlay"
    / "var"
    / "www"
    / "dashboard"
).resolve()


def run(cmd):
    subprocess.run(cmd, check=True)


def main():
    run(["python3", "scripts/build.py", "-c"])

    shutil.copy(BUILD_PATH / f"{APP_NAME}.js", DEPLOY_PATH)
    shutil.copy(BUILD_PATH / f"{APP_NAME}.wasm", DEPLOY_PATH)
    shutil.copy(BUILD_PATH / "qtloader.js", DEPLOY_PATH)


if __name__ == "__main__":
    main()
