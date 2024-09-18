#ifndef THREADPOOL_H
#define THREADPOOL_H

#include <vector>
#include <thread>
#include <queue>
#include <functional>
#include <mutex>
#include <condition_variable>
#include <atomic>

class ThreadPool {
public:
    ThreadPool(size_t numThreads);
    ~ThreadPool();

    /// @brief Добавляем задачу в очередь
    template<class F>
    void enqueue(F&& task);

private:
    /// @brief Рабочий поток
    void worker();

    /// @brief Вектор потоков
    std::vector<std::thread> workers;

    /// @brief Очередь задач
    std::queue<std::function<void()>> tasks;

    /// @brief Мьютекс для синхронизации очереди
    std::mutex queueMutex;

    /// @brief Условная переменная для ожидания
    std::condition_variable condition;

    /// @brief Флаг для остановки пула потоков
    std::atomic<bool> stop;
};

//-------------------------------------------------------------------------------------------------

// Конструктор: создает пул потоков и запускает рабочие потоки
ThreadPool::ThreadPool(size_t numThreads) : stop(false) {
    for (size_t i = 0; i < numThreads; ++i) {
        workers.emplace_back([this] { worker(); });
    }
}

//-------------------------------------------------------------------------------------------------

// Деструктор: останавливает пул и завершает работу всех потоков
ThreadPool::~ThreadPool() {
    {
        std::unique_lock<std::mutex> lock(queueMutex);
        stop = true;
    }
    condition.notify_all();
    for (std::thread &worker : workers) {
        worker.join(); // Ждем завершения всех потоков
    }
}

//-------------------------------------------------------------------------------------------------

// Метод для добавления задачи в очередь
template<class F>
void ThreadPool::enqueue(F&& task) {
    {
        std::unique_lock<std::mutex> lock(queueMutex);
        tasks.emplace(std::forward<F>(task)); // Добавляем задачу в очередь
    }
    condition.notify_one(); // Уведомляем один поток о новой задаче
}

//-------------------------------------------------------------------------------------------------

// Рабочий поток: постоянно ждет задачи и выполняет их
void ThreadPool::worker() {
    while (true) {
        std::function<void()> task;
        {
            std::unique_lock<std::mutex> lock(queueMutex);
            condition.wait(lock, [this] { return stop || !tasks.empty(); });

            if (stop && tasks.empty()) return;

            task = std::move(tasks.front());
            tasks.pop();
        }
        task(); // Выполняем задачу
    }
}

#endif // THREADPOOL_H
