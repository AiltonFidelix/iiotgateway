set(EXECUTABLE_NAME Runner)

set(HEADERS
    src/utils/testutils.h
    src/modbus/mockmodbusclient.h
    src/mqtt/mockmqttclient.h
)

set(SOURCES
    src/utils/testutils.cpp
    src/gpio/testgpiopin.cpp
    src/modbus/testcommmodbus.cpp
    src/modbus/testcommmodbusrequestparser.cpp
    src/modbus/testcommmodbussettingsparser.cpp
    src/mqtt/testcommmqtt.cpp
    src/mqtt/testcommmqttsettingsparser.cpp
    src/network/testnetworkmanager.cpp
    src/main.cpp
)

set(RESOURCES tests.qrc)

set(COMPONENTS Core)

set(LIBRARIES
    Application
    Communication
    Device
    gtest_main
    gmock_main
    paho-mqtt3a
    paho-mqtt3as
    paho-mqttpp3
)

include_directories("src/utils")

set(PROJECT_SOURCES ${HEADERS} ${SOURCES})

find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS ${COMPONENTS})
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS ${COMPONENTS})

add_executable(${EXECUTABLE_NAME} ${PROJECT_SOURCES} ${RESOURCES})

target_link_libraries(${EXECUTABLE_NAME} Qt${QT_VERSION_MAJOR}::Core)

target_sources(${EXECUTABLE_NAME} PRIVATE)

foreach(component IN LISTS COMPONENTS)
    target_link_libraries(${EXECUTABLE_NAME} Qt${QT_VERSION_MAJOR}::${component})
endforeach()

foreach(library IN LISTS LIBRARIES)
    target_link_libraries(${EXECUTABLE_NAME} ${library})
    target_include_directories(${EXECUTABLE_NAME} PRIVATE "${CMAKE_SOURCE_DIR}/${library}")
endforeach()
