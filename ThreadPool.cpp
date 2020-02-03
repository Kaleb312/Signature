#include "ThreadPool.h"

ThreadPool::ThreadPool() : mStopFlag(false)
{
}

ThreadPool::~ThreadPool()
{
    mStopFlag = true;
    mCv.notify_all();
    for (auto& thread : mThreads)
    {
        thread.join();
    }
}

bool ThreadPool::init(size_t threadsNumber)
{
    size_t counter = 0;
    try
    {
        mThreads.reserve(threadsNumber);
        for (; counter < threadsNumber; ++counter)
        {
            mThreads.emplace_back([this](){threadTask();});
        }
    }
    catch (const std::exception& e)
    {
        std::cout << "\nThreadPool::mThreads.emplace_back() exception caught: " << e.what() << std::endl;
        std::cout << "Program will continue work with " << std::to_string(counter) << " threads" << std::endl;
    }
    return counter != 0;
}

void ThreadPool::threadTask()
{
    while(true)
    {
        std::function<void()> task;
        {
            std::unique_lock<std::mutex> lock(mMutex);
            mCv.wait(lock, [this]{return mStopFlag || !mTasksQueue.empty();});
            if (mStopFlag || mTasksQueue.empty())
            {
                return;
            }
            task = std::move(mTasksQueue.front());
            mTasksQueue.pop();
        }
        try
        {
            task();
        }
        catch (const std::exception& e)
        {
            std::cout << "\nThreadPool::threadTask() exception caught: " << e.what() << std::endl;
            std::cout << "Program will be stopped" << std::endl;
            mStopFlag = true;
            mCv.notify_all();
        }
    }
}

std::future<size_t> ThreadPool::processDataBlock(std::string&& inputData)
{
    auto calcHash = std::make_shared<std::packaged_task<size_t()>>([input = std::move(inputData)]()
    {
        return std::hash<std::string>()(input);
    });
    auto result = calcHash->get_future();
    {
        std::unique_lock<std::mutex> lock(mMutex);
        if (mStopFlag)
        {
            throw std::runtime_error("Adding task in stopped thread pool");
        }
        mTasksQueue.emplace([calcHash](){(*calcHash)();});
    }
    mCv.notify_one();
    return result;
}
