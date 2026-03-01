#!/usr/bin/env python3

import os
import shutil
import subprocess
from pathlib import Path

APP_VERSION = "1.0.0"
APP_NAME = "IIoTDashboard"
APP_FOLDER = f"{APP_NAME}-deploy"
BUILD_PATH = Path("build-wasm")


def run(cmd):
    subprocess.run(cmd, check=True)


def main():
    app_folder = Path(APP_FOLDER)
    deb_file = Path(f"{APP_NAME}-{APP_VERSION}.deb")
    generated_deb = Path(f"{APP_FOLDER}.deb")

    run(["python3", "scripts/build.py", "-c"])

    if app_folder.exists():
        shutil.rmtree(app_folder)

    if deb_file.exists():
        deb_file.unlink()

    (app_folder / "DEBIAN").mkdir(parents=True)
    (app_folder / "etc/nginx/sites-available").mkdir(parents=True)
    (app_folder / "var/www/iiotdashboard").mkdir(parents=True)

    shutil.copy("debian/control", app_folder / "DEBIAN")
    shutil.copy("debian/postinst", app_folder / "DEBIAN")
    shutil.copy("nginx/iiotdashboard", app_folder / "etc/nginx/sites-available")
    shutil.copy("www/index.html", app_folder / "var/www/iiotdashboard")
    shutil.copy("www/favicon.ico", app_folder / "var/www/iiotdashboard")
    shutil.copy(BUILD_PATH / f"{APP_NAME}.js", app_folder / "var/www/iiotdashboard")
    shutil.copy(BUILD_PATH / f"{APP_NAME}.wasm", app_folder / "var/www/iiotdashboard")
    shutil.copy(BUILD_PATH / "qtloader.js", app_folder / "var/www/iiotdashboard")

    os.chmod(app_folder / "DEBIAN/postinst", 0o755)

    run(["dpkg", "--build", str(app_folder)])

    shutil.rmtree(app_folder)

    if generated_deb.exists():
        generated_deb.rename(deb_file)


if __name__ == "__main__":
    main()
