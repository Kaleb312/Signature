#include "FileReader.h"

static constexpr auto MEGABYTE_SIZE = 20 /*1024 * 1024*/;
static constexpr auto AVAILABLE_MEMORY = 4 * MEGABYTE_SIZE;

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
            for (std::vector<char>::const_iterator i = readData.begin(); i != readData.end(); ++i)
            {
                std::cout << *i;
            }
            std::cout << std::endl << "--------------" << std::endl;
            {
                std::lock_guard<std::mutex> lock(mMutex);
                mDataBlockList.push_back(readData);
            }
            mSem.wait();
        }
    };
    mThread = std::thread(read);
    mIsFinised = true;
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
    return mIsFinised;
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
