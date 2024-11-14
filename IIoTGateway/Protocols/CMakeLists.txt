set(HEADERS
    comminterface.h
    commfactory.h
    modbus/commmodbus.h
    modbus/commmodbusrtu.h
    modbus/commmodbustcp.h
    modbus/writeregistermodel.h
)

set(SOURCES
    commfactory.cpp
    modbus/commmodbus.cpp
    modbus/commmodbusrtu.cpp
    modbus/commmodbustcp.cpp
    modbus/writeregistermodel.cpp
)

set(COMPONENTS
    Core
    SerialBus
    SerialPort
)

find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS ${COMPONENTS})
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS ${COMPONENTS})

add_library(Protocols ${HEADERS} ${SOURCES})

foreach(component IN LISTS COMPONENTS)
    target_link_libraries(Protocols Qt${QT_VERSION_MAJOR}::${component})
endforeach()

target_include_directories(Protocols PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}")
