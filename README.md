# Модель файловой системы

## Общее
> [!WARNING]
> Работа модели файловой системы гарантируется только на ubuntu 22.04

Требуемая ОС: ubuntu 22.04

Количество ядер: 1

Перед компиляцией и запуском надо выполнить следующие шаги:

1. ` git clone [https://github.com/NovaNexusMephi/file_system](https://github.com/NovaNexusMephi/file_system)`

2.  `cd file_system`

3.  `chmod 755 configure.sh`

4. ` ./configure.sh`


Таким образом будут скачаны все обновления, и установлены все зависимости. Вот список зависимостей:

| Пакет | Версия |
|---------|------|
| [make](https://www.gnu.org/software/make/) | >= 4.3 |
| [cmake](https://cmake.org/) | >= 3.22 |
|  [gcc](https://gcc.gnu.org/) | >= 13.0 |
| [g++](https://gcc.gnu.org/) | >= 13.0 |
| [pkg-config](https://www.freedesktop.org/wiki/Software/pkg-config/) | >= 0.29 |
| [libconfig++-dev](https://hyperrealm.github.io/libconfig/) | >= 1.5 |
|  [libgtest-dev](https://google.github.io/googletest/) | >= 1.14 |
|  [nlohmann-json3-dev](https://github.com/nlohmann/json?tab=readme-ov-file) | >= 3.12 |
|  [clang](https://clang.llvm.org/) | >= 14.0 |


## Описание Makefile

В `Makefile` написаны команды для сборки и запуска модели файловой системы.

С помощью команды `make build` будет осуществлена сборка всего проекта, все исполняемые файлы лежат в папке `build`.

С помощью команды `make run` будет осуществлен запуск всей системы.

Для запуска unit тестов необходимо перейти в папку `build/tests`. В этой папке будут следующие исполняемые файлы с тестами:

- command_builder_test

- command_parser_test

- commands_test

- config_test

- validator_tests

Все эти тесты доступны при сборке проекта в ветке **dev**.

## Описание dev ветки

В данной ветке находится неполная версия модели файловой системы, а также тесты, которые описаны более подробно в предыдущей главе.

## Описание main ветки

В данной ветке находится последняя стабильная версия модели файловой системы, а также по данной ветке генерируется документация, которую можно найти на [сайте](https://novanexusmephi.github.io/file_system/)
