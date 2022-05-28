#pragma once
#include <functional>
#include <vector>
#include <queue>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <future>
#include <memory>
#include <utility>

class ThreadPool {
 public:
    using size_type = std::size_t;
 private:
    size_type size;
    std::vector<std::thread> workers;
    std::atomic<bool> is_running;
    std::mutex queue_mutex;
    std::queue<std::function<void()>> tasks_queue;
    std::condition_variable is_new_task;
    void worker_body();
 public:
    explicit ThreadPool(size_type n);
    ThreadPool(const ThreadPool&) = delete;
    ~ThreadPool();
    template <class Func, class... Args>
    auto exec(Func func, Args... args) -> std::future<decltype(func(args...))>;
};

template <class Func, class... Args>
auto ThreadPool::exec(Func func, Args... args) -> std::future<decltype(func(args...))> {
    auto new_task = std::make_shared<std::packaged_task<decltype(func(args...))()>>(
                    [func, args...]() {
                              return func(args...);
                    });
    std::future<decltype(func(args...))> task_future = new_task->get_future();
    {
        std::lock_guard<std::mutex> lock(queue_mutex);
        tasks_queue.template emplace([new_task]() {
            (*new_task)();
        });
        is_new_task.notify_one();
    }
    return task_future;
}
