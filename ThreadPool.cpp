#include "ThreadPool.h"

ThreadPool::ThreadPool() :
    mPool(static_cast<int>(std::thread::hardware_concurrency()))
{
}

std::future<size_t> ThreadPool::processDataBlock(std::string&& input)
{
    return mPool.push
    (
        [input](int)
        {
            return std::hash<std::string>()(input);
        }
    );
}
