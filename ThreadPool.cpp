#include "ThreadPool.h"

ThreadPool::ThreadPool() :
    mPool(static_cast<int>(std::thread::hardware_concurrency()))
{
}

size_t ThreadPool::calcHash(const std::vector<char>& input) const
{
    return std::hash<std::string>()(std::string(input.begin(), input.end()));
}

std::future<size_t> ThreadPool::processDataBlock(const std::vector<char>& input)
{
    return mPool.push
    (
        [this, input](int)
        {
            return calcHash(input);
        }
    );
}
