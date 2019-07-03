#include "Semaphore.h"

Semaphore::Semaphore(int count)
    : mCount(count)
{
}

void Semaphore::notify(int tid)
{
    std::unique_lock<std::mutex> lock(mMutex);
    mCount++;
//    std::cout << "thread " << tid <<  " notify" << std::endl;
    //notify the waiting thread
    mCv.notify_one();
}

void Semaphore::wait(int tid)
{
    std::unique_lock<std::mutex> lock(mMutex);
    while(mCount == 0)
    {
//        std::cout << "thread " << tid << " wait" << std::endl;
        //wait on the mutex until notify is called
        mCv.wait(lock);
//        std::cout << "thread " << tid << " run" << std::endl;
    }
    mCount--;
}
