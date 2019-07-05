#ifndef FILEREADER_H
#define FILEREADER_H

#include <string>
#include <fstream>
#include <iostream>
#include <vector>
#include <mutex>
#include <thread>
#include <atomic>
#include <memory>
#include <queue>
#include "Semaphore.h"

class FileReader
{
public:
    FileReader(const std::string& inFile, const std::string& outFile, unsigned int blockSize = 1);
    ~FileReader();
    bool openFiles();
    void start();
    void stop();
    void postInput();
    void postOutput();
    std::vector<char> getDataBlock();

private:
    std::ifstream mFin;
    std::ifstream mFout;
    std::string mInputFileName;
    std::string mOutputFileName;
    unsigned int mBlockSize;
    Semaphore mInputSem;
    Semaphore mOutputSem;
    std::thread mThread;
    std::atomic<bool> mStopFlag {false};
    std::queue<std::vector<char>> mDataQueue;
};
#endif // FILEREADER_H
