set(LIBRARY_NAME Application)

set(HEADERS
    dbconnection.h
    dbstorage.h
    control.h
    gateway.h
    storageinterface.h
)

set(SOURCES
    dbconnection.cpp
    dbstorage.cpp
    control.cpp
    gateway.cpp
)

set(RESOURCES application.qrc)

set(QT_COMPONENTS
    Core
    HttpServer
    Sql
)

set(LIBRARIES
    Communication
    Device
)

find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS ${QT_COMPONENTS})
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS ${QT_COMPONENTS})

add_library(${LIBRARY_NAME} SHARED ${HEADERS} ${SOURCES} ${RESOURCES})
set_target_properties(${LIBRARY_NAME} PROPERTIES VERSION ${PROJECT_VERSION})

foreach(component IN LISTS QT_COMPONENTS)
    target_link_libraries(${LIBRARY_NAME} Qt${QT_VERSION_MAJOR}::${component})
endforeach()

target_include_directories(${LIBRARY_NAME} PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}")

foreach(library IN LISTS LIBRARIES)
    target_link_libraries(${LIBRARY_NAME} ${library})
    target_include_directories(${LIBRARY_NAME} PRIVATE "${CMAKE_SOURCE_DIR}/${library}")
endforeach()
