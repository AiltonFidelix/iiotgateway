cmake_minimum_required(VERSION 3.20)

project(IIoTGateway VERSION 1.0.0 LANGUAGES CXX)

add_compile_definitions(APP_NAME="${PROJECT_NAME}")

option(ADD_HOST "Add host implementation" ON)
option(ADD_TARGET "Add target implementation" OFF)

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
add_subdirectory(Tests)

include(GNUInstallDirs)

include(FetchContent)

FetchContent_Declare(googletest
    GIT_REPOSITORY https://github.com/google/googletest
    GIT_TAG v1.16.0
)

FetchContent_MakeAvailable(googletest)

# Install libraries
# https://github.com/eclipse-paho/paho.mqtt.cpp version v1.5.1

install(TARGETS ${PROJECT_NAME}
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)
