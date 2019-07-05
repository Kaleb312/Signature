#include "FileReader.h"

static constexpr auto MEGABYTE_SIZE = 20 /*1024 * 1024*/;
static constexpr auto AVAILABLE_MEMORY = 4 * MEGABYTE_SIZE;

FileReader::FileReader(const std::string& inFile, const std::string& outFile, unsigned int blockSize) :
    mInputFileName(inFile),
    mOutputFileName(outFile),
    mBlockSize(MEGABYTE_SIZE * blockSize),
    mInputSem(AVAILABLE_MEMORY / (MEGABYTE_SIZE * blockSize))
{

}

FileReader::~FileReader()
{
    mStopFlag = true;
    if (mThread.joinable())
    {
        mInputSem.post();
        mThread.join();
    }
}

bool FileReader::openFiles()
{
    mFin.open(mInputFileName, std::ios::in | std::ios::binary);
    mFin.unsetf(std::ios::skipws);
    mFout.open(mOutputFileName, std::ios::out | std::ios::binary);
    return mFin.is_open() && mFout.is_open();
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
            mFin.read(&readData.at(0), mBlockSize);
            std::cout << "Data read: ";
            for (std::vector<char>::const_iterator i = readData.begin(); i != readData.end(); ++i)
            {
                std::cout << *i;
            }
            std::cout << std::endl << "--------------" << std::endl;
            mDataQueue.push(readData);
            mInputSem.wait();
        }
    };
    mThread = std::thread(read);
}

void FileReader::stop()
{
    mStopFlag = true;
    if (mThread.joinable())
    {
        mInputSem.post();
        mThread.join();
    }
}

void FileReader::postInput()
{
    mInputSem.post();
}

void FileReader::postOutput()
{
    mOutputSem.post();
}

std::vector<char> FileReader::getDataBlock()
{
    mInputSem.post();
    std::vector<char> returnValue;
    if (!mDataQueue.empty())
    {
        returnValue = mDataQueue.front();
        mDataQueue.pop();
    }
    return returnValue;
}
