cmake_minimum_required(VERSION 3.5)

project(piklema_test_mqtt_unittests LANGUAGES CXX)

enable_testing()

find_package(QT NAMES Qt6 Qt5 COMPONENTS Test Mqtt REQUIRED)
find_package(Qt${QT_VERSION_MAJOR} COMPONENTS Test Mqtt REQUIRED)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(piklema_test_mqtt_unittests
    tst_testmqttpublisher.cpp
    ../src/textfilemqttpublisher.h
    ../src/textfilemqttpublisher.cpp
    test.qrc
    )
add_test(NAME TestMqttPublisher COMMAND piklema_test_mqtt_unittests)

target_link_libraries(piklema_test_mqtt_unittests PRIVATE Qt${QT_VERSION_MAJOR}::Test Qt${QT_VERSION_MAJOR}::Mqtt)

