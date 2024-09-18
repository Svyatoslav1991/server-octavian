#ifndef FUNCTIONS_H
#define FUNCTIONS_H

#include <string>

// Константы

/// @brief Порт для связи
constexpr int PORT = 12345;

/// @brief Размер буфера для передачи данных
constexpr int BUFFER_SIZE = 1024;

/// @brief Размер пула потоков
constexpr int THREAD_POOL_SIZE = 4;

//------------------------------------------------------------------------------

// функции

/// @brief  Функция для получения временной метки
std::string getTimestamp();

/// @brief  Функция обработки подключения клиента
void handleClient(int clientSocket);

#endif // FUNCTIONS_H
