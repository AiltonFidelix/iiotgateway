cmake_minimum_required(VERSION 3.20)

project(IIoTGateway VERSION 1.0.0  LANGUAGES CXX)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(SOURCES main.cpp)
set(COMPONENTS Core)
set(LIBRARIES Application Protocols)
set(PROJECT_SOURCES ${SOURCES} ${RESOURCES})

foreach(library IN LISTS LIBRARIES)
    add_subdirectory(${library})
endforeach()

find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS ${COMPONENTS})
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS ${COMPONENTS})

add_executable(IIoTGateway ${PROJECT_SOURCES})

target_link_libraries(IIoTGateway Qt${QT_VERSION_MAJOR}::Core)

foreach(component IN LISTS COMPONENTS)
    target_link_libraries(IIoTGateway Qt${QT_VERSION_MAJOR}::${component})
endforeach()

foreach(library IN LISTS LIBRARIES)
    target_link_libraries(IIoTGateway ${library})
endforeach()

include(GNUInstallDirs)
install(TARGETS ${PROJECT_NAME}
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)
