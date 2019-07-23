#ifndef THREADPOOL_H
#define THREADPOOL_H

#include <vector>
#include <queue>
#include <memory>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <future>
#include <functional>
#include <stdexcept>

class ThreadPool
{
public:
    ThreadPool(size_t threadsNumber);
    ~ThreadPool();
    std::future<size_t> processDataBlock(std::string&& input);

    ThreadPool() = delete;
    ThreadPool(const ThreadPool&) = delete;
    ThreadPool(ThreadPool&&) = delete;
    ThreadPool& operator=(const ThreadPool&) = delete;
    ThreadPool& operator=(ThreadPool&&) = delete;

private:
//    template<class F, class... Args>
//    auto addTask(F&& func, Args&&... args)
//        -> std::future<typename std::result_of<F(Args...)>::type>;

    std::vector<std::thread> mThreads;
    std::queue<std::function<void()>> mTasks;
    std::mutex mMutex;
    std::condition_variable mCv;
    std::atomic<bool> mStopFlag;
};

#endif // THREADPOOL_H
