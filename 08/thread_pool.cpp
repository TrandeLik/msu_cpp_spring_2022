#include "thread_pool.hpp"

ThreadPool::ThreadPool(size_type n): size(n), is_running(true) {
    workers.resize(size);
    for (size_type i = 0; i < size; ++i) {
        workers[i] = std::move(std::thread([this](){
            worker_body();
        }));
    }
}

void ThreadPool::worker_body() {
    while (is_running) {
        std::unique_lock<std::mutex> lock(queue_mutex);
        if (tasks_queue.empty()) {
            is_new_task.wait(lock);
        } else {
            std::function<void()> new_task = std::move(tasks_queue.front());
            tasks_queue.pop();
            lock.unlock();
            new_task();
        }
    }
}

ThreadPool::~ThreadPool() {
    is_running = false;
    is_new_task.notify_all();
    for (size_type i = 0; i < size; ++i) {
        workers[i].join();
    }
}
