#include "functions.h"
#include "threadpool.h"  // Включаем файл с реализацией пула потоков

#include <iostream>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <cstring>

int main(int argc, char* argv[]) {
    // Проверка аргументов командной строки для запуска сервера
    if (argc != 2 || std::strcmp(argv[1], "-s") != 0) {
        std::cerr << "Неверные параметры запуска. Используйте '-s' для запуска сервера.\n";
        return 1;
    }

    // Создаем серверный сокет
    int serverSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (serverSocket < 0) {
        std::cerr << "Ошибка создания сокета\n";
        return 1;
    }

    sockaddr_in serverAddr{};
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_addr.s_addr = INADDR_ANY;     // Принимаем подключения с любых адресов
    serverAddr.sin_port = htons(PORT);           // Используем заранее заданный порт

    // Привязываем сокет к адресу и порту
    if (bind(serverSocket, reinterpret_cast<sockaddr*>(&serverAddr), sizeof(serverAddr)) < 0) {
        std::cerr << "Ошибка привязки сокета\n";
        close(serverSocket);
        return 1;
    }

    // Начинаем прослушивание входящих подключений
    if (listen(serverSocket, 5) < 0) {
        std::cerr << "Ошибка при прослушивании\n";
        close(serverSocket);
        return 1;
    }

    std::cout << "Ожидание подключения клиентов...\n";

    // Создаем пул потоков
    ThreadPool pool(THREAD_POOL_SIZE);

    while (true) {
        sockaddr_in clientAddr{};
        socklen_t clientAddrLen = sizeof(clientAddr);
        int clientSocket = accept(serverSocket, reinterpret_cast<sockaddr*>(&clientAddr), &clientAddrLen);
        if (clientSocket < 0) {
            std::cerr << "Ошибка принятия соединения\n";
            continue;
        }

        std::cout << "Клиент подключен\n";

        // Отправляем задачу на обработку подключения клиента в пул потоков
        pool.enqueue([clientSocket] {
            handleClient(clientSocket);
        });
    }

    // Закрываем серверный сокет (по факту, программа бесконечно слушает подключения)
    close(serverSocket);
    return 0;
}
