#include "ThreadPool.h"

ThreadPool::ThreadPool(size_t threadsNumber) :
    mStopFlag(false)
{
    for (size_t i = 0; i < threadsNumber; ++i)
    {
        mThreads.emplace_back
        (
            [this]
            {
               for(;;)
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
            }
        );
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
    auto task = std::make_shared<std::packaged_task<size_t()>>([input]{return std::hash<std::string>()(input);});
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
//    return addTask([input](){std::hash<std::string>()(input);});
//    return mPool.push
//    (
//        [input](int)
//        {
//            return std::hash<std::string>()(input);
//        }
//    );
}

//template<class F, class... Args>
//auto ThreadPool::addTask(F&& func, Args&&... args)
//    -> std::future<typename std::result_of<F(Args...)>::type>
//{
//    using returnType = typename std::result_of<F(Args...)>::type;
//    auto task = std::make_shared<std::packaged_task<returnType()>>(std::bind(std::forward<F>(func),
//        std::forward<Args>(args)...));
//    std::future<returnType> result = task->get_future();
//    {
//        std::unique_lock<std::mutex> lock(mMutex);
//        if (mStopFlag)
//        {
//            throw std::runtime_error("Adding task in stopped thread pool");
//        }
//        mTasks.emplace([task](){(*task)();});
//    }
//    mCv.notify_one();
//    return result;
//}
