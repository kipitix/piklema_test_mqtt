# Text File to MQTT publisher

## Сборка

### Допущения

1. Используется Qt community edition версии 5.15.2.
2. Каталог установки Qt: '~/Qt/5.15.2'.

### Сборка и установка QtMQTT

Перед сборкой основного приложения требуется установить в директорию QT, под которой будет производится сборка, библиотеку QtMqtt. Она не входит в QT community edition.

Для этого потребуется выполнить следующие шаги.

Клонируем репозиторий с переключением на ветку совместимой с версией QT, с которой предполагается сборка (например Qt 5.15.2):

```shell
git clone https://github.com/qt/qtmqtt.git --branch 5.15.2
```

Создаём директорию для сборки и переходим в неё:

```shell
mkdir qtmqtt_build
cd qtmqtt_build
```

Далее выполняем:

```shell
~/Qt/5.15.2/gcc_64/bin/qmake ../qtmqtt
make
make install
```

Если Qt установлен в директории `/usr/local/Qt` (там, куда нужен доступ из под пользователя `root`), то последнюю команду нужно выполнять под `sudo`.

```shell
sudo make install
```

Выходим из каталога для сборки `QtMQTT`:

```shell
cd ..
```

### Сборка приложения

Клонируем репозиторий:

```shell
git clone https://github.com/kipitix/piklema_test_mqtt
```

Создаём рядом с репозиторием каталог для сборки:

```shell
mkdir piklema_test_mqtt_build
cd piklema_test_mqtt_build
```

Выполняем `CMake`:

```shell
QT_DIR=~/Qt/5.15.2/gcc_64/lib/cmake/Qt5 Qt5_DIR=~/Qt/5.15.2/gcc_64/lib/cmake/Qt5 cmake ../piklema_test_mqtt/
```

Далее выполняем `make`:

```shell
make
```

Результат сборки - исполняемые файлы `piklema_test_mqtt` - основное приложение и `test/piklema_test_mqtt_unittests` - юнит-тесты.

Выходим из каталога для сборки:

```shell
cd ..
```

### Сборка в docker под платформу arm64

Для начала соберём контейнер с библиотекой QtMQTT:

```shell
docker build . -f arm64.dockerfile -t piklema/qt5.15.2_arm64_qtmqtt:v1
```

Для сборки приложения и тестов нужно выполнить следующие команды:

```shell
docker run -v ${PWD}:/root/app -it piklema/qt5.15.2_arm64_qtmqtt:v1 -c 'cd /root/app; ./build_arm64.sh'
```

## Запуск тестов

Для запуска тестов просто запустите исполняемый файл в каталоге сборки `test/piklema_test_mqtt_unittests`.

## Linter

В корне проекта расположен [файл с настройками линтера clang](.clang-format).

Для запуска автоматического форматирования нужно в корне проекта запустить:

```shell
find . -iname '*.h' -o -iname '*.cpp' | xargs clang-format -i
```

## TODO

Осталась нерешённая проблема с линковкой бинарного файла под платформу aarch64 при сборке в докере.
