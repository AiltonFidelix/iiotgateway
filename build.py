#!/usr/bin/python3

import sys
import argparse
import subprocess
import multiprocessing
from functools import wraps
from pathlib import Path

BUILDROOT_PATH = (Path(__file__).resolve().parent / "buildroot").resolve()
DISTRO_PATH = (Path(__file__).resolve().parent / "distro").resolve()
DEF_CONFIG_FILE = "raspberrypi3_64_defconfig"


def run(cmd, cwd=None):
    subprocess.run(cmd, check=True, cwd=cwd)


def clean_gateway_artifacts():
    print("---> Cleaning gateway built artifacts in distro")
    run(["python3", "clean.py"], cwd="distro")


def build_and_install(application: str):
    def inner_build_and_install(func):
        @wraps(func)
        def wrapper(*args, **kwargs):
            print(f"---> Building application {application}")

            command = func(*args, **kwargs)
            run(command, cwd=application)

            print(f"---> Installing application {application}")
            run(["python3", "scripts/install.py"], cwd=application)

            return command

        return wrapper

    return inner_build_and_install


@build_and_install("dashboard")
def build_and_install_dashboard(jobs: str, clean: bool = False):
    command = ["python3", "scripts/build.py", "--jobs", jobs]

    if clean:
        command.append("--clean")

    return command


@build_and_install("gateway")
def build_and_install_gateway(jobs: str, clean: bool = False, arch: str = "aarch64"):
    command = ["python3", "scripts/build.py", "--arch", arch, "--jobs", jobs]

    if clean:
        command.append("--clean")

    return command


def main():
    parser = argparse.ArgumentParser(description="IIoTGateway platform build")

    parser.add_argument(
        "--default", action="store_true", help="Use all default options"
    )

    default_num_of_jobs = str(multiprocessing.cpu_count() - 4)

    parser.add_argument(
        "--jobs",
        default=default_num_of_jobs,
        metavar="JOBS",
        help=f"Number of parallel jobs (default: {default_num_of_jobs})",
    )

    parser.add_argument(
        "--clean-distro",
        action="store_true",
        help="Clean previous distro build artifacts",
    )

    parser.add_argument(
        "--clean-gateway",
        action="store_true",
        help="Clean previous gateway build artifacts",
    )

    parser.add_argument(
        "--skip-gateway", action="store_true", help="Skip gateway build"
    )

    parser.add_argument(
        "--defc",
        default=DEF_CONFIG_FILE,
        metavar="DEFC",
        help=f"defconfig file (default: {DEF_CONFIG_FILE})",
    )

    args = parser.parse_args()

    if len(sys.argv) == 1:
        parser.print_help()
        exit(0)

    if args.clean_gateway:
        clean_gateway_artifacts()

    if args.clean_distro:
        print("---> Cleaning distro built artifacts in buildroot")
        run(["make", "clean"], cwd=BUILDROOT_PATH)

    if not args.skip_gateway:
        build_and_install_dashboard(args.jobs, args.clean_gateway)
        build_and_install_gateway(args.jobs, args.clean_gateway)

    print("---> Building IIoTGateway system...")

    run(["make", f"BR2_EXTERNAL={DISTRO_PATH}", args.defc], cwd=BUILDROOT_PATH)
    run(["make", f"-j{args.jobs}"], cwd=BUILDROOT_PATH)


if __name__ == "__main__":
    main()
