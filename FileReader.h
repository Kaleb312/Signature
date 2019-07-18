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
#include <list>
#include "Semaphore.h"

class FileReader
{
public:
    FileReader(const std::string& inFile, unsigned int blockSize = 1);
    ~FileReader();
    bool openFiles();
    void start();
    void stop();
    void postInput();
    void postOutput();
    std::vector<char> getDataBlock();

private:
    std::ifstream mFin;
    std::string mInputFileName;
    unsigned int mBlockSize;
    Semaphore mInputSem;
    std::thread mThread;
    std::atomic<bool> mStopFlag {false};
    std::list<std::vector<char>> mDataBlockList;
};
#endif // FILEREADER_H
