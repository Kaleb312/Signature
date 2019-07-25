#ifndef FILEWRITER_H
#define FILEWRITER_H

#include <fstream>
#include <iostream>
#include <mutex>
#include <thread>
#include <atomic>
#include <memory>
#include <queue>
#include <future>
#include "Semaphore.h"

class FileWriter
{
public:
    FileWriter(const std::string& outFile);
    ~FileWriter();
    bool openFile();
    void start();
    void stop();
    void post();
    void finish();
    void pushFutureInList(std::future<size_t>&& future);
    bool isFinished();

private:
    std::ofstream mFout;
    std::string mFileName;
    Semaphore mSem;
    std::thread mThread;
    std::mutex mMutex;
    std::atomic<bool> mStopFlag;
    std::queue<std::future<size_t>> mFutureHashList;
};

#endif // FILEWRITER_H
