set(LIBRARY_NAME Communication)

set(HEADERS
    comm_global.hpp
    comminterface.hpp
    commfactory.hpp

    modbus/commmodbus_global.hpp
    modbus/commmodbusclientadapter.hpp
    modbus/commmodbusclientinterface.hpp
    modbus/commmodbusrequestparser.hpp
    modbus/commmodbussettingsparser.hpp
    modbus/commmodbus.hpp
    modbus/commmodbusrtu.hpp
    modbus/commmodbustcp.hpp

    mqtt/commmqtt.hpp
    mqtt/commmqttcallback.hpp
    mqtt/commmqttactionlistener.hpp
    mqtt/commmqttsettingsparser.hpp
)

set(SOURCES
    commfactory.cpp
    modbus/commmodbusclientadapter.cpp
    modbus/commmodbusrequestparser.cpp
    modbus/commmodbussettingsparser.cpp
    modbus/commmodbus.cpp
    modbus/commmodbusrtu.cpp
    modbus/commmodbustcp.cpp

    mqtt/commmqtt_global.hpp
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
set_target_properties(${LIBRARY_NAME} PROPERTIES VERSION ${PROJECT_VERSION})

foreach(component IN LISTS QT_COMPONENTS)
    target_link_libraries(${LIBRARY_NAME} Qt${QT_VERSION_MAJOR}::${component})
endforeach()

target_include_directories(${LIBRARY_NAME} PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}")

foreach(library IN LISTS LIBRARIES)
    target_link_libraries(${LIBRARY_NAME} ${library})
endforeach()

