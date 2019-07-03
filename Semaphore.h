#ifndef SEMAPHORE_H
#define SEMAPHORE_H

#include <mutex>
#include <iostream>
#include <condition_variable>

class Semaphore
{
public:
    Semaphore (int count = 0);
    void post();
    void wait();

private:
    std::mutex mMutex;
    std::condition_variable mCv;
    int mCount;
};

#endif // SEMAPHORE_H
