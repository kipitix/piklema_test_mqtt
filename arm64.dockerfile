FROM carlonluca/qt-dev:5.15.2
# FROM dockcross/linux-arm64-full:latest
WORKDIR /qtmqtt_lib
RUN apt update; apt install -y g++-aarch64-linux-gnu
RUN git clone https://github.com/qt/qtmqtt.git --branch 5.15.2
RUN cd qtmqtt; /opt/Qt-arm64-5.15.2/bin/qmake; make -j4; make install