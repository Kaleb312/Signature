#ifndef THREADPOOL_H
#define THREADPOOL_H

#include <functional>
#include <thread>
#include "ctpl_stl.h"

class ThreadPool
{
public:
    ThreadPool();
    std::future<size_t> processDataBlock(const std::vector<char>& input);

private:
    size_t calcHash(const std::vector<char>& input) const;

    ctpl::thread_pool mPool;
};

#endif // THREADPOOL_H
