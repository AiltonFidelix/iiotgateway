from pathlib import Path

ROOTFS_OVERLAY_PATH = (
    Path(__file__).resolve().parent
    / "board"
    / "iiotgateway"
    / "raspberrypi3-64"
    / "rootfs_overlay"
).resolve()

DASHBOARD_PATH = (ROOTFS_OVERLAY_PATH / "var" / "www" / "dashboard").resolve()

GATEWAY_BIN_PATH = (ROOTFS_OVERLAY_PATH / "opt" / "iiotgateway" / "bin").resolve()

USR_LIB_PATH = (ROOTFS_OVERLAY_PATH / "usr" / "lib").resolve()


def remove(directory: Path, pattern: str):
    files_to_remove = list(directory.rglob(pattern))

    for file in files_to_remove:
        file.unlink()


def clean_static():
    remove(DASHBOARD_PATH, "*.js")
    remove(DASHBOARD_PATH, "*.wasm")


def clean_binaries():
    remove(GATEWAY_BIN_PATH, "gateway")


def clean_libraries():
    remove(USR_LIB_PATH, "lib*.so*")


def main():
    clean_static()
    clean_binaries()
    clean_libraries()


if __name__ == "__main__":
    main()
