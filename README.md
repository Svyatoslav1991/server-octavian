# serverOctavian

`serverOctavian` — это консольное серверное приложение на C++, которое принимает файлы от клиентов и сохраняет их с использованием временной метки.

## Требования

- CMake 3.14 или выше
- Компилятор с поддержкой C++17
- Операционная система Linux (или другая ОС с поддержкой сокетов POSIX)
- Библиотека потоков `pthread`

## Компиляция

Для сборки сервера выполните следующие шаги:

1. Клонируйте репозиторий или скачайте исходный код.
2. Перейдите в директорию проекта.
3. Выполните команды:

   ```bash
   mkdir build
   cd build
   cmake ..
   make

После успешной компиляции в директории build появится исполняемый файл serverOctavian.

## Использование

Запустите сервер с параметрами:
./serverOctavian -s

Сервер начинает прослушивание порта 12345 и готов принимать подключения от клиентов.

## Описание работы

Основные функции:
Обработка клиента: Сервер создаёт новый поток для каждого подключенного клиента, используя пул потоков. Данные от клиента принимаются и сохраняются в файл с временной меткой.
Сокетное соединение: Сервер прослушивает TCP-соединения на порту 12345. При каждом подключении клиента создаётся новый файл с уникальным именем, основанным на текущей дате и времени.
Пул потоков: Сервер использует пул потоков для параллельной обработки нескольких клиентов. По умолчанию размер пула потоков составляет 4.

## Пример использования

Запустите сервер:
./serverOctavian -s

Клиенты могут подключаться и отправлять файлы на сервер. Каждый файл будет сохранён с именем, содержащим временную метку (например, 20230918_123456.hex).

## Структура проекта

src/main.cpp — основной файл программы, отвечает за инициализацию сервера и работу с подключениями.
src/functions.cpp — содержит функции для обработки клиентов и создания временной метки.
include/functions.h — заголовочный файл с описанием функций и констант.
include/threadpool.h — реализация пула потоков, используемого для обработки нескольких подключений одновременно.

## Параметры запуска

-s — запуск сервера для прослушивания подключений клиентов.

## Возможные ошибки

Ошибка создания сокета — возникает, если не удалось создать сокет.
Ошибка привязки сокета — возникает, если порт уже занят или привязка не удалась.
Ошибка при прослушивании — возникает, если сервер не может начать прослушивание на указанном порту.
Ошибка принятия соединения — возникает, если клиент не может подключиться.
