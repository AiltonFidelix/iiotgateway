set(LIBRARY_NAME Device)

set(HEADERS
    device_global.h

    driver/driver_global.h
    driver/gpio/gpio_global.h
    driver/gpio/gpiopin.h
    driver/gpio/hostgpiopin.h
    driver/gpio/gpiostate.h
    driver/gpio/gpiomode.h
    driver/gpio/gpiopinfactory.h

    logging/logging_global.h
    logging/loghandler.h

    network/network_global.h
    network/networkmanager.h
    network/raspberrynetworkmanager.h
    network/hostnetworkmanager.h
    network/networkmanagerfactory.h
)

set(SOURCES
    driver/gpio/hostgpiopin.cpp
    driver/gpio/gpiopinfactory.cpp

    logging/loghandler.cpp

    network/raspberrynetworkmanager.cpp
    network/hostnetworkmanager.cpp
    network/networkmanagerfactory.cpp
)

if(CMAKE_SYSTEM_PROCESSOR STREQUAL "aarch64")
    message(STATUS "Adding aarch64 resources to the device library")
    list(APPEND HEADERS driver/gpio/raspberrygpiopin.h)
    list(APPEND SOURCES driver/gpio/raspberrygpiopin.cpp)
endif()

set(QT_COMPONENTS
    Core
    Network
)

find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS ${QT_COMPONENTS})
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS ${QT_COMPONENTS})

add_library(${LIBRARY_NAME} SHARED ${HEADERS} ${SOURCES})

foreach(component IN LISTS QT_COMPONENTS)
    target_link_libraries(${LIBRARY_NAME} Qt${QT_VERSION_MAJOR}::${component})
endforeach()

target_include_directories(${LIBRARY_NAME} PUBLIC
    "${CMAKE_CURRENT_SOURCE_DIR}"
)

if(CMAKE_SYSTEM_PROCESSOR STREQUAL "aarch64")
    target_link_libraries(${LIBRARY_NAME} wiringPi)
endif()
