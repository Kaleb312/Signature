#include "ThreadPool.h"

ThreadPool::ThreadPool(size_t threadsNumber) :
    mStopFlag(false)
{
    auto threadTask = [this]
    {
        while(true)
        {
            try
            {
                std::function<void()> task;
                {
                    std::unique_lock<std::mutex> lock(mMutex);
                    mCv.wait(lock, [this]{return mStopFlag || !mTasks.empty();});
                    if (mStopFlag || mTasks.empty())
                    {
                        return;
                    }
                    task = std::move(mTasks.front());
                    mTasks.pop();
                }
                task();
            }
            catch (const std::exception& e)
            {
                mStopFlag = true;
                mCv.notify_all();
                std::cout << "\nThreadPool threadTask() function exception caught: " << e.what() << std::endl;
                std::exit(EXIT_FAILURE);
            }
        }
    };
    for (size_t i = 0; i < threadsNumber; ++i)
    {
        try
        {
            mThreads.emplace_back(threadTask);
        }
        catch (const std::exception& e)
        {
            mStopFlag = true;
            mCv.notify_all();
            std::cout << "\nThreadPool mThreads.emplace_back() function exception caught: " << e.what() << std::endl;
            std::exit(EXIT_FAILURE);
        }
    }
}

ThreadPool::~ThreadPool()
{
    {
        std::unique_lock<std::mutex> lock(mMutex);
        mStopFlag = true;
    }
    mCv.notify_all();
    for (auto& thread : mThreads)
    {
        thread.join();
    }
}

std::future<size_t> ThreadPool::processDataBlock(std::string&& inputData)
{
    std::cout << "ThreadPool: processDataBlock() start" << std::endl;
    auto calcHash = std::make_shared<std::packaged_task<size_t()>>([input = std::move(inputData)]()
    {
        std::cout << "ThreadPool: processDataBlock():calcHash start" <<std::endl;
        return std::hash<std::string>()(input);
    });
    auto result = calcHash->get_future();
    {
        std::cout << "ThreadPool: processDataBlock() fuck" <<std::endl;
        std::unique_lock<std::mutex> lock(mMutex);
        std::cout << "ThreadPool: processDataBlock():calcHash->get_future" <<std::endl;
        if (mStopFlag)
        {
            throw std::runtime_error("Adding task in stopped thread pool");
        }
        std::cout << "ThreadPool: processDataBlock():mTasks.emplace" <<std::endl;
        mTasks.emplace([calcHash](){(*calcHash)();});
    }
    mCv.notify_one();
    return result;
}
