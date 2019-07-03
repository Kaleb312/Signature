#include "FileReader.h"

static const unsigned int THOUSAND_BYTES = 5;

FileReader::FileReader() :
    mBlockSize(THOUSAND_BYTES)
{
}

FileReader::~FileReader()
{
    mThread.join();
}

FileReader::FileReader(unsigned int blockSize) :
    mBlockSize(THOUSAND_BYTES * blockSize)
{
}

bool FileReader::openFile(const std::string& fileName)
{
    bool result = false;
    mFin.open(fileName, std::ios::in | std::ios::binary);
    mFin.unsetf(std::ios::skipws);
    if (mFin.is_open())
    {
//        std::vector<char> readData(mBlockSize);
//        mFin.getline(&readData.at(0), mBlockSize);
        result = true;
    }
    return result;
}

void FileReader::start()
{
    auto read = [this]()
    {
        mSem.wait(11);
        std::vector<char> readData(mBlockSize);
        while (mFin && !mStopFlag)
        {

            mFin.read(&readData.at(0), mBlockSize);
            for (std::vector<char>::const_iterator i = readData.begin(); i != readData.end(); ++i)
            {
                std::cout << *i << ' ';
            }
            std::cout << std::endl;
            mSem.notify(11);
        }
    };
    mThread = std::thread(read);
}

void FileReader::stop()
{
    mStopFlag = true;
    mSem.notify(11);
    mThread.join();
}

void FileReader::readFile()
{
//    std::vector<char> readData(mBlockSize);
//    while (mFin.read(&readData.at(0), mBlockSize))
//    {
//        mSem.wait(11);
//        for (std::vector<char>::const_iterator i = readData.begin(); i != readData.end(); ++i)
//        {
//            std::cout << *i << ' ';
//        }
//        std::cout << std::endl;
    //    }
}

void FileReader::test()
{
    std::cout << "werg" << std::endl;
}

Semaphore* FileReader::getSemPtr()
{
    return &mSem;
}
