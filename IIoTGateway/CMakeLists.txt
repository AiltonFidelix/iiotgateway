cmake_minimum_required(VERSION 3.20)

project(IIoTGateway VERSION 1.0.0 LANGUAGES CXX)

option(ENABLE_RASPBERRY_GPIO "Enable raspberry hardware GPIO" OFF)
option(ENABLE_LINUX_REBOOT "Enable linux system reboot" OFF)
option(ENABLE_LINUX_NETWORK_MANAGER "Enable linux network manager" OFF)
option(ENABLE_TESTS "Enable unit tests" OFF)
option(ENABLE_GTEST_INSTALL "Enable googletest install" OFF)
option(ENABLE_MQTT_PAHO_INSTALL "Enable mqttpaho install" OFF)

add_compile_definitions(APP_NAME="${PROJECT_NAME}")

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

message(STATUS "Compiler detected: ${CMAKE_CXX_COMPILER_ID}")
message(STATUS "Architecture detected: ${CMAKE_SYSTEM_PROCESSOR}")

set(COMPONENTS Core)

find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS ${COMPONENTS})
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS ${COMPONENTS})

add_subdirectory(Application)
add_subdirectory(Communication)
add_subdirectory(Device)
add_subdirectory(Main)

if(ENABLE_TESTS)
    add_subdirectory(Tests)
endif()

include(GNUInstallDirs)

include(FetchContent)

if(ENABLE_GTEST_INSTALL)
    FetchContent_Declare(googletest
        GIT_REPOSITORY https://github.com/google/googletest
        GIT_TAG v1.16.0
    )
    FetchContent_MakeAvailable(googletest)
endif()

if(ENABLE_MQTT_PAHO_INSTALL)
    FetchContent_Declare(mqttpaho
        GIT_REPOSITORY https://github.com/eclipse-paho/paho.mqtt.cpp
        GIT_TAG v1.5.1
    )
    FetchContent_MakeAvailable(mqttpaho)
endif()

install(TARGETS ${PROJECT_NAME}
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)
