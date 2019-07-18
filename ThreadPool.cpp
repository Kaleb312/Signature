#include "ThreadPool.h"

const auto BUSY_THREADS = 3;

ThreadPool::ThreadPool() :
    mPool(static_cast<int>(std::thread::hardware_concurrency() - BUSY_THREADS))
{

}

size_t ThreadPool::calcHash(const std::vector<char>& input) const
{
    size_t result = 0;
    for (auto& byte : input)
    {
        size_t byteHash = std::hash<char>()(byte);
        result = result ^ (byteHash << 1);
    }
    return result;
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
