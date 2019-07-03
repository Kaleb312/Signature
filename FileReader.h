#ifndef FILEREADER_H
#define FILEREADER_H

#include <string>
#include <fstream>
#include <iostream>
#include <vector>
#include <mutex>
#include <thread>
#include <atomic>
#include "Semaphore.h"

class FileReader
{
public:
    FileReader();
    FileReader(unsigned int blockSize);
    ~FileReader();
    bool openFile(const std::string& fileName);
    void start();
    void stop();
    Semaphore* getSemPtr();

private:
    void readFile();
    static void test();
    std::ifstream mFin;
    std::string mFileName;
    unsigned int mBlockSize;
    Semaphore mSem;
    std::thread mThread;
    std::atomic<bool> mStopFlag {false};
};

#endif // FILEREADER_H
