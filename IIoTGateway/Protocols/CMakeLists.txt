set(LIBRARY_NAME Protocols)

set(HEADERS
    comm_global.h
    comminterface.h
    commfactory.h

    modbus/commmodbus_global.h
    modbus/commmodbusclientadapter.h
    modbus/commmodbusclientinterface.h
    modbus/commmodbusrequestparser.h
    modbus/commmodbus.h
    modbus/commmodbusrtu.h
    modbus/commmodbustcp.h

    mqtt/commmqtt.h
    mqtt/commmqttcallback.h
    mqtt/commmqttactionlistener.h
    mqtt/commmqttsettingsparser.h
)

set(SOURCES
    commfactory.cpp
    modbus/commmodbusclientadapter.cpp
    modbus/commmodbusrequestparser.cpp
    modbus/commmodbus.cpp
    modbus/commmodbusrtu.cpp
    modbus/commmodbustcp.cpp

    mqtt/commmqtt_global.h
    mqtt/commmqtt.cpp
    mqtt/commmqttcallback.cpp
    mqtt/commmqttactionlistener.cpp
    mqtt/commmqttsettingsparser.cpp
)

set(QT_COMPONENTS
    Core
    SerialBus
    SerialPort
)

set(LIBRARIES paho-mqttpp3)

find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS ${QT_COMPONENTS})
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS ${QT_COMPONENTS})

add_library(${LIBRARY_NAME} SHARED ${HEADERS} ${SOURCES})

foreach(component IN LISTS QT_COMPONENTS)
    target_link_libraries(${LIBRARY_NAME} Qt${QT_VERSION_MAJOR}::${component})
endforeach()

foreach(library IN LISTS LIBRARIES)
    target_link_libraries(${LIBRARY_NAME} ${library})
endforeach()

target_include_directories(${LIBRARY_NAME} PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}")
