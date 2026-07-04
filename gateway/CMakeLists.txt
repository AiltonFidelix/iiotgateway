cmake_minimum_required(VERSION 3.20...4.3.0)

project(gateway VERSION 1.0.0 LANGUAGES CXX)

option(ENABLE_RASPBERRY_GPIO "Enable raspberry hardware GPIO" OFF)
option(ENABLE_LINUX_REBOOT "Enable linux system reboot" OFF)
option(ENABLE_LINUX_NETWORK_MANAGER "Enable linux network manager" OFF)
option(ENABLE_TESTS "Enable unit tests" OFF)

add_compile_definitions(APP_NAME="${PROJECT_NAME}")

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 20)
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

install(TARGETS ${PROJECT_NAME}
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)
