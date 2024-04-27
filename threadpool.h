#ifndef THREADPOOL_H
#define THREADPOOL_H

#include <vector>
#include <queue>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <functional>
#include <future>
#include <type_traits>
#include <iostream>

class ThreadPool {
public:
    explicit ThreadPool(size_t numThreads = std::thread::hardware_concurrency() / 2, size_t maxQueueSize = 1000) : stop(false), maxQueueSize(maxQueueSize) {
        workers.reserve(numThreads);
        for(size_t i = 0; i < numThreads; ++i) {
            workers.emplace_back([this] {
                for (;;) {
                    std::function<void()> task;
                    {
                        std::unique_lock<std::mutex> lock(this->queueMutex);
                        this->condition.wait(lock, [this]{ return this->stop || !this->tasks.empty(); });
                        if (this->stop && this->tasks.empty()) return;
                        task = std::move(this->tasks.front());
                        this->tasks.pop();
                    }
                    try {
                        task();
                    } catch (const std::exception& e) {
                        std::cerr << "Exception caught in ThreadPool task: " << e.what() << std::endl;
                        // Consider logging additional state information here
                    } catch (...) {
                        std::cerr << "Unknown exception caught in ThreadPool task." << std::endl;
                    }
                }
            });
        }
    }
    ~ThreadPool() {
        {
            std::unique_lock<std::mutex> lock(queueMutex);
            stop = true;
        }
        condition.notify_all();
        for (std::thread &worker : workers) {
            worker.join();
        }
    }

    template<typename Func, typename... Args>
    auto enqueue(Func&& func, Args&&... args) -> std::future<typename std::invoke_result<Func, Args...>::type> {
        using return_type = typename std::invoke_result<Func, Args...>::type;

        auto task = std::make_shared<std::packaged_task<return_type()>>(
            std::bind(std::forward<Func>(func), std::forward<Args>(args)...)
            );

        std::future<return_type> res = task->get_future();
        {
            std::unique_lock<std::mutex> lock(queueMutex);
            if (stop) {
                throw std::runtime_error("enqueue on stopped ThreadPool");
            }
            if (tasks.size() >= maxQueueSize) {
                throw std::runtime_error("task queue overflow");
            }
            tasks.emplace([task]() { (*task)(); });
        }
        condition.notify_one();
        return res;
    }

private:
    std::vector<std::thread> workers;
    std::queue<std::function<void()>> tasks;
    std::mutex queueMutex;
    std::condition_variable condition;
    bool stop;
    size_t maxQueueSize;
};

#endif // THREADPOOL_H
