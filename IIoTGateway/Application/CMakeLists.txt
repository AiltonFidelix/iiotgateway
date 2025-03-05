set(LIBRARY_NAME Application)

set(HEADERS
    dbconnection.h
    interface.h
    gateway.h
)

set(SOURCES
    dbconnection.cpp
    interface.cpp
    gateway.cpp
)

set(RESOURCES application.qrc)

set(QT_COMPONENTS
    Core
    HttpServer
    Sql
)

find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS ${QT_COMPONENTS})
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS ${QT_COMPONENTS})

add_library(${LIBRARY_NAME} SHARED ${HEADERS} ${SOURCES} ${RESOURCES})

foreach(component IN LISTS QT_COMPONENTS)
    target_link_libraries(${LIBRARY_NAME} Qt${QT_VERSION_MAJOR}::${component})
endforeach()

target_include_directories(${LIBRARY_NAME} PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}")
