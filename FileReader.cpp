#include "FileReader.h"

static constexpr auto MEGABYTE_SIZE = 1024 * 1024;
static constexpr auto AVAILABLE_MEMORY = 1000 * MEGABYTE_SIZE;

FileReader::FileReader(const std::string& inFile, unsigned int blockSize) :
    mFileName(inFile),
    mBlockSize(MEGABYTE_SIZE * blockSize),
    mSem(AVAILABLE_MEMORY / (MEGABYTE_SIZE * blockSize)),
    mStopFlag(false),
    mIsFinised(false)
{
}

FileReader::~FileReader()
{
    mStopFlag = true;
    if (mThread.joinable())
    {
        mSem.post();
        mThread.join();
    }
}

bool FileReader::openFile()
{
    mFin.open(mFileName, std::ios::in | std::ios::binary);
    mFin.unsetf(std::ios::skipws);
    return mFin.is_open();
}

void FileReader::start()
{
    auto read = [this]()
    {
        std::vector<char> readData(mBlockSize);
        while (mFin)
        {
            if (mStopFlag)
            {
                break;
            }
            readData.assign(mBlockSize, 0);
            mFin.read(&readData.at(0), static_cast<int>(mBlockSize));
            {
                std::lock_guard<std::mutex> lock(mMutex);
                mDataBlockList.push_back(readData);
            }
            mSem.wait();
        }
        mIsFinised = true;
    };
    mThread = std::thread(read);
}

void FileReader::stop()
{
    mStopFlag = true;
    if (mThread.joinable())
    {
        mSem.post();
        mThread.join();
    }
}

void FileReader::post()
{
    mSem.post();
}

bool FileReader::isFinished()
{
    std::lock_guard<std::mutex> lock(mMutex);
    return mIsFinised && mDataBlockList.empty();
}

bool FileReader::isDataReady()
{
    std::lock_guard<std::mutex> lock(mMutex);
    return !mDataBlockList.empty();
}

std::vector<char> FileReader::getDataBlock()
{
    std::lock_guard<std::mutex> lock(mMutex);
    if (!mDataBlockList.empty())
    {
        auto returnValue = mDataBlockList.front();
        mDataBlockList.pop_front();
        mSem.post();
        return returnValue;
    }
    else
    {
        return std::vector<char>();
    }
}
