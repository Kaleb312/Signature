#include "FileReader.h"

static constexpr auto THOUSAND_BYTES = 20;
//static constexpr auto AVAILABLE_MEMORY = 3 * THOUSAND_BYTES * THOUSAND_BYTES * THOUSAND_BYTES;
static constexpr auto AVAILABLE_MEMORY = 4 * THOUSAND_BYTES;

FileReader::FileReader() :
    mBlockSize(THOUSAND_BYTES),
    mSem(AVAILABLE_MEMORY / THOUSAND_BYTES)
{
}

FileReader::FileReader(unsigned int blockSize) :
    mBlockSize(THOUSAND_BYTES * blockSize),
    mSem(AVAILABLE_MEMORY / (THOUSAND_BYTES * blockSize))
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

bool FileReader::openFile(const std::string& fileName)
{
    bool result = false;
    mFin.open(fileName, std::ios::in | std::ios::binary);
    mFin.unsetf(std::ios::skipws);
    result = mFin.is_open();
    return result;
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
            mSem.wait();
        }
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

std::vector<char> FileReader::getDataBlock()
{
    mSem.post();
    std::vector<char> returnValue = mDataQueue.front();
    return std::move(returnValue);
}
