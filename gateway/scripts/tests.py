#!/usr/bin/env python3

import sys
import subprocess

ARCH = "amd64"


def run(cmd):
    subprocess.run(cmd, check=True)


def main():
    args = sys.argv[1:]
    num_args = len(args)

    run(["python3", "scripts/build.py", ARCH, "-t"])

    if num_args == 1 and args[0] == "-m":
        run(["valgrind", f"build-{ARCH}/Tests/Runner"])
    else:
        run([f"build-{ARCH}/Tests/Runner"])


if __name__ == "__main__":
    main()
