#!/usr/bin/env python3

import argparse
import subprocess

ARCH = "amd64"


def run(cmd):
    subprocess.run(cmd, check=True)


def main():
    parser = argparse.ArgumentParser(description="gateway test script")

    parser.add_argument(
        "-m",
        "--mem-check",
        action="store_true",
        help="Run valgrind memory check",
    )

    args = parser.parse_args()

    runner = f"build-{ARCH}/Tests/Runner"

    run(
        [
            "python3",
            "scripts/build.py",
            "-t",
            "-a",
            ARCH,
        ]
    )

    if args.mem_check:
        run(["valgrind", runner])
    else:
        run([runner])


if __name__ == "__main__":
    main()
