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
                    if (mStopFlag && mTasks.empty())
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

std::future<size_t> ThreadPool::processDataBlock(std::string&& input)
{
    auto task = std::make_shared<std::packaged_task<size_t()>>([input]
    {
        return std::hash<std::string>()(input);
    });
    auto result = task->get_future();
    {
        std::unique_lock<std::mutex> lock(mMutex);
        if (mStopFlag)
        {
            throw std::runtime_error("Adding task in stopped thread pool");
        }
        mTasks.emplace([task](){(*task)();});
    }
    mCv.notify_one();
    return result;
}
