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

    network/network_global.h
    network/networkmanager.h
    network/raspberrynetworkmanager.h
    network/hostnetworkmanager.h
    network/networkmanagerfactory.h
)

set(SOURCES
    driver/gpio/hostgpiopin.cpp
    driver/gpio/gpiopinfactory.cpp

    network/raspberrynetworkmanager.cpp
    network/hostnetworkmanager.cpp
    network/networkmanagerfactory.cpp
)

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
