set(HEADERS
    interface.h
    gateway.h
)

set(SOURCES
    interface.cpp
    gateway.cpp
)

set(COMPONENTS Core)

find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS ${COMPONENTS})
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS ${COMPONENTS})

add_library(Application ${HEADERS} ${SOURCES})

foreach(component IN LISTS COMPONENTS)
    target_link_libraries(Application Qt${QT_VERSION_MAJOR}::${component})
endforeach()

target_include_directories(Application PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}")
