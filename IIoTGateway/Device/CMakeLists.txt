set(LIBRARY_NAME Device)

set(HEADERS
    device_global.h
    driver/driver_global.h
    driver/gpio/gpio_global.h
    driver/gpio/gpiopin.h
    driver/gpio/gpiostate.h
    driver/gpio/gpiomode.h
    driver/gpio/gpiopinfactory.h
    logging/logging_global.h
    logging/loghandler.h
    network/network_global.h
    network/networkmanager.h
    network/networkmanagerfactory.h
    system/system_global.h
    system/reboot.h
    system/rebootfactory.h
)

set(SOURCES
    driver/gpio/gpiopinfactory.cpp
    logging/loghandler.cpp
    network/networkmanagerfactory.cpp
    system/rebootfactory.cpp
)

if(CMAKE_SYSTEM_PROCESSOR STREQUAL "aarch64" OR ADD_TARGET)
if(ENABLE_GPIO)
    list(APPEND HEADERS driver/gpio/raspberrygpiopin.h)
    list(APPEND SOURCES driver/gpio/raspberrygpiopin.cpp)
else()
    list(APPEND HEADERS driver/gpio/fakegpiopin.h)
    list(APPEND SOURCES driver/gpio/fakegpiopin.cpp)
endif()
    message(STATUS "Adding raspberry implementation to the device library")

    list(APPEND HEADERS
        network/raspberrynetworkmanager.h
    )
    list(APPEND SOURCES
        network/raspberrynetworkmanager.cpp
    )
elseif(CMAKE_SYSTEM_PROCESSOR STREQUAL "x86_64" OR ADD_HOST)
    message(STATUS "Adding host implementation to the device library")

    list(APPEND HEADERS
        network/hostnetworkmanager.h
        system/hostreboot.h
    )
    list(APPEND SOURCES
        network/hostnetworkmanager.cpp
        system/hostreboot.cpp
    )
endif()

set(QT_COMPONENTS
    Core
    Network
)

find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS ${QT_COMPONENTS})
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS ${QT_COMPONENTS})

add_library(${LIBRARY_NAME} SHARED ${HEADERS} ${SOURCES})

set_target_properties(${LIBRARY_NAME} PROPERTIES VERSION ${PROJECT_VERSION})

foreach(component IN LISTS QT_COMPONENTS)
    target_link_libraries(${LIBRARY_NAME} Qt${QT_VERSION_MAJOR}::${component})
endforeach()

target_include_directories(${LIBRARY_NAME} PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}")

if(CMAKE_SYSTEM_PROCESSOR STREQUAL "aarch64")
    target_link_libraries(${LIBRARY_NAME} wiringPi)
endif()
