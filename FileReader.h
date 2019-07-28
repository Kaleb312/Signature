#ifndef FILEREADER_H
#define FILEREADER_H

#include <string>
#include <fstream>
#include <iostream>
#include <mutex>
#include <thread>
#include <atomic>
#include <queue>
#include "Semaphore.h"

static constexpr auto MEGABYTE_SIZE = 1024 * 1024;
static constexpr auto AVAILABLE_MEMORY = 1024 * MEGABYTE_SIZE;

class FileReader
{
public:
    FileReader(const std::string& inFile, unsigned int blockSize = 1);
    ~FileReader();
    bool openFile();
    void start();
    void stop();
    void post();
    void finish();
    bool isFinished();
    bool getDataBlock(std::string& dataBlock);

private:
    std::ifstream mFin;
    std::string mFileName;
    unsigned int mBlockSize;
    Semaphore mSem;
    std::thread mThread;
    std::mutex mMutex;
    std::atomic<bool> mStopFlag;
    bool mIsFinised;
    std::queue<std::string> mDataBlockQueue;
};
#endif // FILEREADER_H
