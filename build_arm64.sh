#!/bin/bash

mkdir arm64_build
cd arm64_build
QT_DIR=/opt/Qt-arm64-5.15.2/lib/cmake/Qt5 Qt5_DIR=/opt/Qt-arm64-5.15.2/lib/cmake/Qt5 cmake -DCMAKE_CXX_COMPILER=/usr/bin/aarch64-linux-gnu-g++ ..
# QT_DIR=/opt/Qt-arm64-5.15.2/lib/cmake/Qt5 Qt5_DIR=/opt/Qt-arm64-5.15.2/lib/cmake/Qt5 cmake ..
# LDFLAGS='-Wl,-rpath-link,/qtmqtt_lib/aarch64--glibc--stable-2018.11-1/lib64' make -j4
LDFLAGS='-Wl,--allow-shlib-undefined' make -j4
cd ..

#mkdir unittests_build
#cd unittests_build
#QT_DIR=/opt/Qt-arm64-5.15.2/lib/cmake/Qt5 Qt5_DIR=/opt/Qt-arm64-5.15.2/lib/cmake/Qt5 CMAKE_CXX_COMPILER=/usr/bin/aarch64-linux-gnu-g++ cmake ../test
#make -j4
