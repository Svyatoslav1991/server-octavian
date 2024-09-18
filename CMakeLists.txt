cmake_minimum_required(VERSION 3.14)

project(serverOctavian LANGUAGES CXX)

# Настройка стандарта C++
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Указываем, где находятся заголовочные файлы
include_directories(include)

# Указываем исходные файлы
add_executable(serverOctavian
  src/main.cpp
  src/functions.cpp
  include/threadpool.h
  include/functions.h
)

# Линкуем с библиотекой потоков pthread
target_link_libraries(serverOctavian pthread)
