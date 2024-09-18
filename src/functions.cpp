#include "functions.h"
#include <iostream>
#include <fstream>
#include <ctime>
#include <cstring>
#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>

// Функция для получения временной метки
std::string getTimestamp() {
    std::time_t now = std::time(nullptr);
    char buffer[100];
    std::strftime(buffer, sizeof(buffer), "%Y%m%d_%H%M%S", std::localtime(&now));
    return std::string(buffer);
}

//------------------------------------------------------------------------------------------

// Функция обработки подключения клиента
void handleClient(int clientSocket) {
    std::string filename = getTimestamp() + ".hex";
    std::ofstream outputFile(filename, std::ios::binary);

    if (!outputFile.is_open()) {
        std::cerr << "Ошибка открытия файла для записи\n";
        close(clientSocket);
        return;
    }

    std::cout << "Начало передачи файла...\n";

    char buffer[BUFFER_SIZE];
    ssize_t bytesRead;

    while ((bytesRead = read(clientSocket, buffer, sizeof(buffer))) > 0) {
        outputFile.write(buffer, bytesRead);
    }

    if (bytesRead < 0) {
        std::cerr << "Ошибка передачи данных\n";
    }

    outputFile.close();
    close(clientSocket);
    std::cout << "Файл успешно передан и сохранен как " << filename << "\n";
}
