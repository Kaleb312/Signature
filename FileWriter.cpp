#include "FileWriter.h"

FileWriter::FileWriter(const std::string& outFile, FileReader& fileReaderPtr) :
    mFileName(outFile),
    mFileReaderPtr(fileReaderPtr)
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
    if (mFout.is_open())
    {
        mFout.close();
    }
}

bool FileWriter::openFile()
{
    mFout.open(mFileName, std::ios::out | std::ios::binary);
    return mFout.is_open();
}

void FileWriter::start()
{
    mThread = std::thread([this](){write();});
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
        if (mFutureHashQueue.empty())
        {
            mStopFlag = true;
        }
    }
    mSem.post();
    mThread.join();
}

void FileWriter::pushFutureInQueue(std::future<size_t>&& future)
{
    std::lock_guard<std::mutex> lock(mMutex);
    mFutureHashQueue.push(std::move(future));
}

bool FileWriter::isFinished()
{
    std::lock_guard<std::mutex> lock(mMutex);
    return mFutureHashQueue.empty();
}

void FileWriter::write()
{
    try
    {
        while (true)
        {
            if (mStopFlag)
            {
                break;
            }
            if (!mFutureHashQueue.empty())
            {
                size_t hash = 0;
                {
                    std::lock_guard<std::mutex> lock(mMutex);
                    hash = mFutureHashQueue.front().get();
                    mFutureHashQueue.pop();
                }
                mFout << hash;
                mFileReaderPtr.post();
                std::cout << hash << std::endl;
            }
            mSem.wait();
        }
    }
    catch (const std::exception& e)
    {
        mStopFlag = true;
        std::cout << "\nFileWriter write() exception caught: " << e.what() <<std::endl;
    }
}
