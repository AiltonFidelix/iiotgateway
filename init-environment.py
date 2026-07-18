#!/usr/bin/python3

import os
import shutil
import subprocess

TMP_PATH = "/tmp"
OPT_PATH = "/opt"

GTEST_REPOSITORY = "https://github.com/google/googletest"
GTEST_BRANCH = "v1.16.0"

PAHO_MQTT_CPP_REPOSITORY = "https://github.com/eclipse-paho/paho.mqtt.cpp.git"
PAHO_MQTT_CPP_BRANCH = "v1.5.1"

EMSCRIPTEM_REPOSITORY = "https://github.com/emscripten-core/emsdk.git"
EMSCRIPTEM_BRANCH = "3.1.56"

BUILDROOT_REPOSITORY = "https://gitlab.com/buildroot.org/buildroot.git"
BUILDROOT_BRANCH = "2025.02.x"


def run(cmd, cwd=None):
    subprocess.run(cmd, check=True, cwd=cwd)


def install_gtest():
    GMOCK_HEADER = "/usr/local/include/gmock/gmock.h"
    GMOCK_LIB = "/usr/local/lib64/libgmock.a"
    GTEST_HEADER = "/usr/local/include/gtest/gtest.h"
    GTEST_LIB = "/usr/local/lib64/libgtest_main.a"

    if (
        os.path.exists(GMOCK_HEADER)
        and os.path.exists(GMOCK_LIB)
        and os.path.exists(GTEST_HEADER)
        and os.path.exists(GTEST_LIB)
    ):
        print("---> Google Test is already installed")
        return

    print("---> Installing Google Test")

    if os.path.exists(GTEST_PATH):
        shutil.rmtree(GTEST_PATH)

    GTEST_PATH = f"{TMP_PATH}/googletest"

    try:
        run(["git", "clone", "--branch", GTEST_BRANCH, GTEST_REPOSITORY], TMP_PATH)
        run(["cmake", "-B", "build"], GTEST_PATH)
        run(["cmake", "--build", "build", "--target", "install"], GTEST_PATH)
        print("---> Google Test installed successfully")
    except Exception as e:
        print(f"---> Error installing Google Test: {e}")


def install_paho_mqtt():

    PAHO_C_HEADER = "/usr/local/include/MQTTAsync.h"
    PAHO_C_LIB = "/usr/local/lib64/libpaho-mqtt3a.so"
    PAHO_CPP_HEADER = "/usr/local/include/mqtt/async_client.h"
    PAHO_CPP_LIB = "/usr/local/lib64/libpaho-mqttpp3.so"

    if (
        os.path.exists(PAHO_C_HEADER)
        and os.path.exists(PAHO_C_LIB)
        and os.path.exists(PAHO_CPP_HEADER)
        and os.path.exists(PAHO_CPP_LIB)
    ):
        print("---> Paho MQTT CPP is already installed")
        return

    print("---> Installing Paho MQTT CPP")

    PAHO_MQTT_CPP_PATH = f"{TMP_PATH}/paho.mqtt.cpp"

    if os.path.exists(PAHO_MQTT_CPP_PATH):
        shutil.rmtree(PAHO_MQTT_CPP_PATH)

    try:
        run(
            [
                "git",
                "clone",
                "--branch",
                PAHO_MQTT_CPP_BRANCH,
                PAHO_MQTT_CPP_REPOSITORY,
            ],
            TMP_PATH,
        )

        run(["git", "submodule", "update", "--init"], PAHO_MQTT_CPP_PATH)

        run(
            [
                "cmake",
                "-B",
                "build",
                "-DPAHO_WITH_MQTT_C=ON",
                "-DPAHO_ENABLE_TESTING=OFF",
                "-DCMAKE_C_STANDARD=11",  # With GCC 16.1.1 is using c23 by default and failing because of bool type
                "-DCMAKE_C_STANDARD_REQUIRED=ON",
                "-DCMAKE_C_EXTENSIONS=OFF",
            ],
            PAHO_MQTT_CPP_PATH,
        )

        run(["cmake", "--build", "build", "--target", "install"], PAHO_MQTT_CPP_PATH)
        print("---> Paho Mqtt Cppt installed successfully")
    except Exception as e:
        print(f"---> Error installing Paho Mqtt Cpp: {e}")


def install_emscripten():
    EMSDK_PATH = f"{OPT_PATH}/emsdk"

    if os.path.exists(EMSDK_PATH):
        print("---> Emscripten is already installed")
        return

    print("---> Installing Emscripten")

    try:
        run(["git", "clone", EMSCRIPTEM_REPOSITORY], OPT_PATH)
        run(["./emsdk", "install", EMSCRIPTEM_BRANCH], EMSDK_PATH)
        run(["./emsdk", "activate", EMSCRIPTEM_BRANCH], EMSDK_PATH)
        print("---> Emscripten installed successfully")
    except Exception as e:
        print(f"---> Error installing Emscripten: {e}")


def install_buildroot():
    BUILDROOT_PATH = f"{OPT_PATH}/buildroot"

    if os.path.exists(BUILDROOT_PATH):
        print("---> Buildroot is already installed")
        return

    print("---> Installing Buildroot")

    try:
        run(
            ["git", "clone", "--branch", BUILDROOT_BRANCH, BUILDROOT_REPOSITORY],
            OPT_PATH,
        )
        print("---> Buildroot installed successfully")
    except Exception as e:
        print(f"---> Error installing Buildroot: {e}")


def verify_tool(tool: str):
    tool_path = shutil.which(tool)

    if tool_path:
        print(f"[{tool}] is installed at: {tool_path}")
    else:
        print(f"\033[31m[{tool}] is not installed or not in the system PATH.\033[0m")
        exit(1)


def verify_tools():
    print("---> Verifying required tools")

    verify_tool("cmake")
    verify_tool("git")


def main():
    verify_tools()
    install_gtest()
    install_paho_mqtt()
    install_emscripten()
    install_buildroot()


if __name__ == "__main__":
    main()
