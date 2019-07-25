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
        try
        {
            while (true)
            {
                if (mStopFlag)
                {
                    break;
                }
                if (!mFutureHashList.empty())
                {
                    size_t hash = 0;
                    {
                        std::lock_guard<std::mutex> lock(mMutex);
                        hash = mFutureHashList.front().get();
                        mFutureHashList.pop();
                    }
                    mFout << hash;
                    std::cout << hash << std::endl;
                }
                mSem.wait();
            }
        }
        catch (const std::exception& e)
        {
            mStopFlag = true;
            std::cout << "\nFileWriter write() function exception caught: " << e.what() <<std::endl;
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

void FileWriter::finish()
{
    {
        std::lock_guard<std::mutex> lock(mMutex);
        if (mFutureHashList.empty())
        {
            mStopFlag = true;
        }
    }
    mSem.post();
    mThread.join();
}

void FileWriter::pushFutureInList(std::future<size_t>&& future)
{
    std::lock_guard<std::mutex> lock(mMutex);
    mFutureHashList.push(std::move(future));
}

bool FileWriter::isFinished()
{
    std::lock_guard<std::mutex> lock(mMutex);
    return mFutureHashList.empty();
}
