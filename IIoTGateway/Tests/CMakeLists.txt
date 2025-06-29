set(EXECUTABLE_NAME TestsRunner)

set(HEADERS
    mockmodbusclient.h
    testcommmodbus.h
    testmodbusjsonparser.h
    testutils.h
)

set(SOURCES
    testcommmodbus.cpp
    testmodbusjsonparser.cpp
    testutils.cpp
    main.cpp
)

set(RESOURCES tests.qrc)

set(COMPONENTS Core)

set(LIBRARIES
    Application
    Protocols
    gtest_main
    gmock_main
    paho-mqttpp3
)

set(PROJECT_SOURCES ${HEADERS} ${SOURCES})

find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS ${COMPONENTS})
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS ${COMPONENTS})

add_executable(${EXECUTABLE_NAME} ${PROJECT_SOURCES} ${RESOURCES}
    testmqttsettingsparser.h testmqttsettingsparser.cpp
    testmodbussettingsparser.h testmodbussettingsparser.cpp)

target_link_libraries(${EXECUTABLE_NAME} Qt${QT_VERSION_MAJOR}::Core)

target_sources(${EXECUTABLE_NAME} PRIVATE)

foreach(component IN LISTS COMPONENTS)
    target_link_libraries(${EXECUTABLE_NAME} Qt${QT_VERSION_MAJOR}::${component})
endforeach()

foreach(library IN LISTS LIBRARIES)
    target_link_libraries(${EXECUTABLE_NAME} ${library})
    target_include_directories(${EXECUTABLE_NAME} PRIVATE "${CMAKE_SOURCE_DIR}/${library}")
endforeach()
