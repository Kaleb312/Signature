#ifndef FILEWRITER_H
#define FILEWRITER_H

#include <fstream>
#include <iostream>
#include <mutex>
#include <thread>
#include <atomic>
#include <queue>
#include <future>
#include "Semaphore.h"
#include "FileReader.h"

class FileWriter
{
public:
    FileWriter() = delete;
    FileWriter(const std::string& outFile, FileReader& fileReaderPtr);
    ~FileWriter();
    bool openFile();
    void start();
    void stop();
    void post();
    void finish();
    void pushFutureInQueue(std::future<size_t>&& future);
    bool isFinished();

private:
    std::ofstream mFout;
    std::string mFileName;
    Semaphore mSem;
    std::thread mThread;
    std::mutex mMutex;
    std::atomic<bool> mStopFlag;
    std::queue<std::future<size_t>> mFutureHashQueue;
    FileReader& mFileReaderPtr;
};

#endif // FILEWRITER_H
