#ifndef THREADPOOL_H
#define THREADPOOL_H

#include <functional>
#include <thread>
#include "ctpl_stl.h"

class ThreadPool
{
public:
    ThreadPool();
    std::future<size_t> processDataBlock(std::string&& input);

private:
    ctpl::thread_pool mPool;
};

#endif // THREADPOOL_H
