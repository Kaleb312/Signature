#include "FileWriter.h"

FileWriter::FileWriter(const std::string& outFile) :
    mFileName(outFile),
    mSem(0),
    mStopFlag(false)
{
}

FileWriter::~FileWriter()
{
    mStopFlag = true;
    if (mThread.joinable())
    {
        mSem.post();
        mThread.join();
    }
}

bool FileWriter::openFile()
{
    mFout.open(mFileName, std::ios::out | std::ios::binary);
    return mFout.is_open();
}

void FileWriter::start()
{
    auto write = [this]()
    {
        while (true)
        {
            if (mStopFlag)
            {
                break;
            }
            if (!mFutureHashList.empty())
            {
                std::lock_guard<std::mutex> lock(mMutex);
                size_t hash = mFutureHashList.front().get();
                mFutureHashList.pop_front();
                std::cout << "Result hash: " << hash << std::endl;
            }
            mSem.wait();
        }
    };
    mThread = std::thread(write);
}

void FileWriter::stop()
{
    mStopFlag = true;
    if (mThread.joinable())
    {
        mSem.post();
        mThread.join();
    }
}

void FileWriter::post()
{
    mSem.post();
}

void FileWriter::pushFutureInList(std::future<size_t>&& future)
{
    std::lock_guard<std::mutex> lock(mMutex);
    mFutureHashList.push_back(std::move(future));
}
