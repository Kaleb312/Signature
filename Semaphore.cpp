#include "Semaphore.h"

Semaphore::Semaphore(int count)
    : mCount(count)
{
}

void Semaphore::post()
{
    std::unique_lock<std::mutex> lock(mMutex);
    mCount++;
    mCv.notify_one();
}

void Semaphore::wait()
{
    std::unique_lock<std::mutex> lock(mMutex);
    while(mCount == 0)
    {
        mCv.wait(lock);
    }
    mCount--;
}
