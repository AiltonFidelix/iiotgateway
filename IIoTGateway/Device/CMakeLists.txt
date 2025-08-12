set(LIBRARY_NAME Device)

set(HEADERS
    driver/driver_global.h
    driver/gpio/gpio_global.h
    driver/gpio/gpiopin.h
    driver/gpio/gpiostate.h
    driver/gpio/gpiomode.h
    network/networkmanager.h
)

set(SOURCES
)

set(QT_COMPONENTS
    Core
    Network
)

find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS ${QT_COMPONENTS})
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS ${QT_COMPONENTS})

add_library(${LIBRARY_NAME} SHARED ${HEADERS} ${SOURCES}
    device_global.h
    network/network_global.h

    network/networkmanagerfactory.h network/networkmanagerfactory.cpp
    network/hostnetworkmanager.h network/hostnetworkmanager.cpp
    network/raspberrynetworkmanager.h network/raspberrynetworkmanager.cpp)

foreach(component IN LISTS QT_COMPONENTS)
    target_link_libraries(${LIBRARY_NAME} Qt${QT_VERSION_MAJOR}::${component})
endforeach()

target_include_directories(${LIBRARY_NAME} PUBLIC
    "${CMAKE_CURRENT_SOURCE_DIR}"
)
