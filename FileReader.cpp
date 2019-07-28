#include "FileReader.h"

FileReader::FileReader(const std::string& inFile, unsigned int blockSize) :
    mFileName(inFile),
    mBlockSize(MEGABYTE_SIZE * blockSize),
    mSem((AVAILABLE_MEMORY / (MEGABYTE_SIZE * blockSize))),
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
                mSem.wait();
                std::string readData;
                readData.assign(mBlockSize, 0);
                mFin.read(&readData.at(0), static_cast<int>(mBlockSize));
                {
                    std::lock_guard<std::mutex> lock(mMutex);
                    mDataBlockQueue.push(std::move(readData));
                }


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
    return mIsFinised && mDataBlockQueue.empty();
}

bool FileReader::getDataBlock(std::string& dataBlock)
{
    bool returnValue = false;
    try
    {
        std::lock_guard<std::mutex> lock(mMutex);
        if (!mDataBlockQueue.empty())
        {
            dataBlock = std::move(mDataBlockQueue.front());
            mDataBlockQueue.pop();
            returnValue = true;
        }
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

