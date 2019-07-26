#include "FileReader.h"

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
    if (mFin.is_open())
    {
        mFin.close();
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
        try
        {
            while (mFin)
            {
                if (mStopFlag)
                {
                    break;
                }
                std::string readData;
                readData.assign(mBlockSize, 0);
                mFin.read(&readData.at(0), static_cast<int>(mBlockSize));
                {
                    std::lock_guard<std::mutex> lock(mMutex);
                    mDataBlockList.push(std::move(readData));
                }
                mSem.wait();
            }
        }
        catch (const std::exception& e)
        {
            mStopFlag = true;
            std::cout << "\nFileReader read() function exception caught: " << e.what() <<std::endl;
            std::exit(EXIT_FAILURE);
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

void FileReader::finish()
{
    mSem.post();
    mThread.join();
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

std::string FileReader::getDataBlock()
{
    std::string returnValue;
    try
    {
        std::lock_guard<std::mutex> lock(mMutex);
        returnValue = std::move(mDataBlockList.front());
        mDataBlockList.pop();
        mSem.post();
    }
    catch (const std::exception& e)
    {
        mStopFlag = true;
        mIsFinised = true;
        std::cout << "\nFileReader getDataBlock() function exception caught: " << e.what() <<std::endl;
        std::exit(EXIT_FAILURE);
    }
    return returnValue;
}
