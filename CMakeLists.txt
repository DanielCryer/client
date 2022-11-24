cmake_minimum_required(VERSION 3.14)

project(client VERSION 0.1 LANGUAGES CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Core Network Quick QuickControls2)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Core Network Quick QuickControls2)

set(LIB_SOURCES_DIR ${CMAKE_CURRENT_LIST_DIR}/libs)

set(LIBRARY_SOURCES
    ${LIB_SOURCES_DIR}/clientcontroller.h
    ${LIB_SOURCES_DIR}/clientcontroller.cpp
    ${LIB_SOURCES_DIR}/request.h
    ${LIB_SOURCES_DIR}/request.cpp
    ${LIB_SOURCES_DIR}/deal.h
    ${LIB_SOURCES_DIR}/deal.cpp
    ${LIB_SOURCES_DIR}/userdata.h
    ${LIB_SOURCES_DIR}/userdata.cpp
    ${LIB_SOURCES_DIR}/filters.h
    ${LIB_SOURCES_DIR}/filters.cpp
    ${LIB_SOURCES_DIR}/requestlistmodel.h
    ${LIB_SOURCES_DIR}/requestlistmodel.cpp
    ${LIB_SOURCES_DIR}/deallistmodel.h
    ${LIB_SOURCES_DIR}/deallistmodel.cpp
)

set(PROJECT_SOURCES
        main.cpp
        qml.qrc
        ${LIBRARY_SOURCES}
)


add_subdirectory(test)

add_executable(client
  ${PROJECT_SOURCES}
)


target_link_libraries(client
  PRIVATE Qt${QT_VERSION_MAJOR}::Core Qt${QT_VERSION_MAJOR}::Network Qt${QT_VERSION_MAJOR}::Quick Qt${QT_VERSION_MAJOR}::QuickControls2)

