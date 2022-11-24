cmake_minimum_required(VERSION 3.5)

project(tests LANGUAGES CXX)

enable_testing()

find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Test Network)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Test Network)

#set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(TEST_SOURCES
    main.cpp
    testrequest.h
    testrequest.cpp
    testdeal.h
    testdeal.cpp
    testuserdata.h
    testuserdata.cpp
    testrequestlistmodel.h
    testrequestlistmodel.cpp
    testdeallistmodel.h
    testdeallistmodel.cpp
    ${LIBRARY_SOURCES}
)

add_executable(tests
    ${TEST_SOURCES}
)
add_test(NAME tests COMMAND test)

target_link_libraries(tests PRIVATE Qt${QT_VERSION_MAJOR}::Test Qt${QT_VERSION_MAJOR}::Network)

