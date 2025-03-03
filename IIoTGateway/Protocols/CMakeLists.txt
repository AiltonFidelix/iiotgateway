set(LIBRARY_NAME Protocols)

set(HEADERS
    comminterface.h
    commfactory.h
    modbus/modbusclientadapter.h
    modbus/modbusclientinterface.h
    modbus/modbusjsonparser.h
    modbus/commmodbus.h
    modbus/commmodbusrtu.h
    modbus/commmodbustcp.h
    modbus/writeregistermodel.h
    mqtt/commmqtt.h
    mqtt/commmqttcallback.h
)

set(SOURCES
    commfactory.cpp
    modbus/modbusclientadapter.cpp
    modbus/modbusjsonparser.cpp
    modbus/commmodbus.cpp
    modbus/commmodbusrtu.cpp
    modbus/commmodbustcp.cpp
    modbus/writeregistermodel.cpp
    mqtt/commmqtt.cpp
    mqtt/commmqttcallback.cpp
)

set(QT_COMPONENTS
    Core
    SerialBus
    SerialPort
)

set(LIBRARIES paho-mqttpp3)

find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS ${QT_COMPONENTS})
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS ${QT_COMPONENTS})

add_library(${LIBRARY_NAME} SHARED ${HEADERS} ${SOURCES}
    mqtt/commmqttactionlistener.h mqtt/commmqttactionlistener.cpp)

foreach(component IN LISTS QT_COMPONENTS)
    target_link_libraries(${LIBRARY_NAME} Qt${QT_VERSION_MAJOR}::${component})
endforeach()

foreach(library IN LISTS LIBRARIES)
    target_link_libraries(${LIBRARY_NAME} ${library})
endforeach()

target_include_directories(${LIBRARY_NAME} PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}")
