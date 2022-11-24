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

set(LIBRARY_SOURCES
    ${CMAKE_CURRENT_LIST_DIR}/lib/clientcontroller.h
    ${CMAKE_CURRENT_LIST_DIR}/lib/clientcontroller.cpp
    ${CMAKE_CURRENT_LIST_DIR}/lib/request.h
    ${CMAKE_CURRENT_LIST_DIR}/lib/request.cpp
    ${CMAKE_CURRENT_LIST_DIR}/lib/deal.h
    ${CMAKE_CURRENT_LIST_DIR}/lib/deal.cpp
    ${CMAKE_CURRENT_LIST_DIR}/lib/userdata.h
    ${CMAKE_CURRENT_LIST_DIR}/lib/userdata.cpp
    ${CMAKE_CURRENT_LIST_DIR}/lib/filters.h
    ${CMAKE_CURRENT_LIST_DIR}/lib/filters.cpp
    ${CMAKE_CURRENT_LIST_DIR}/lib/requestlistmodel.h
    ${CMAKE_CURRENT_LIST_DIR}/lib/requestlistmodel.cpp
    ${CMAKE_CURRENT_LIST_DIR}/lib/deallistmodel.h
    ${CMAKE_CURRENT_LIST_DIR}/lib/deallistmodel.cpp
)

set(PROJECT_SOURCES
        main.cpp
        qml.qrc
        ${LIBRARY_SOURCES}
)

add_subdirectory(tests)

if(${QT_VERSION_MAJOR} GREATER_EQUAL 6)
    qt_add_executable(client
        MANUAL_FINALIZATION
        ${PROJECT_SOURCES}
    )
else()
    add_executable(client
      ${PROJECT_SOURCES}
    )
endif()

target_link_libraries(client
  PRIVATE Qt${QT_VERSION_MAJOR}::Core Qt${QT_VERSION_MAJOR}::Network Qt${QT_VERSION_MAJOR}::Quick Qt${QT_VERSION_MAJOR}::QuickControls2)

set_target_properties(client PROPERTIES
    MACOSX_BUNDLE_GUI_IDENTIFIER my.example.com
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
)

install(TARGETS client
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR})

if(QT_VERSION_MAJOR EQUAL 6)
    qt_import_qml_plugins(client)
    qt_finalize_executable(client)
endif()
