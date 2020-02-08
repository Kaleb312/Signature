#ifndef SEMAPHORE_H
#define SEMAPHORE_H

#include <mutex>
#include <condition_variable>

class Semaphore
{
public:
    Semaphore ();
    Semaphore (int count);
    void post();
    void wait();

private:
    std::mutex mMutex;
    std::condition_variable mCv;
    int mCount = 0;
};

#endif // SEMAPHORE_H
