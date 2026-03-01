set(EXECUTABLE_NAME gateway)
set(SOURCES main.cpp)
set(QT_COMPONENTS Core)
set(LIBRARIES Application Device)
set(PROJECT_SOURCES ${SOURCES})

find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS ${QT_COMPONENTS})
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS ${QT_COMPONENTS})

add_executable(${EXECUTABLE_NAME} ${PROJECT_SOURCES})

target_link_libraries(${EXECUTABLE_NAME} Qt${QT_VERSION_MAJOR}::Core)

foreach(component IN LISTS QT_COMPONENTS)
    target_link_libraries(${EXECUTABLE_NAME} Qt${QT_VERSION_MAJOR}::${component})
endforeach()

foreach(library IN LISTS LIBRARIES)
    target_link_libraries(${EXECUTABLE_NAME} ${library})
    target_include_directories(${EXECUTABLE_NAME} PRIVATE "${CMAKE_SOURCE_DIR}/${library}")
endforeach()
