#ifndef FILEREADER_H
#define FILEREADER_H

#include <string>
#include <fstream>
#include <iostream>
#include <mutex>
#include <thread>
#include <atomic>
#include <memory>
#include <list>
#include "Semaphore.h"

class FileReader
{
public:
    FileReader(const std::string& inFile, unsigned int blockSize = 1);
    ~FileReader();
    bool openFile();
    void start();
    void stop();
    void post();
    bool isFinished();
    bool isDataReady();
    std::string getDataBlock();

private:
    std::ifstream mFin;
    std::string mFileName;
    unsigned int mBlockSize;
    Semaphore mSem;
    std::thread mThread;
    std::mutex mMutex;
    std::atomic<bool> mStopFlag;
    bool mIsFinised;
    std::list<std::string> mDataBlockList;
};
#endif // FILEREADER_H
