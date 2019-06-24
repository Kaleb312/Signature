#include "FileReader.h"

static const unsigned int THOUSAND_BYTES = 1024;

FileReader::FileReader() :
    mBlockSize(THOUSAND_BYTES)
{
}

FileReader::FileReader(unsigned int blockSize) :
    mBlockSize(THOUSAND_BYTES * blockSize)
{
}

bool FileReader::readFile(const std::string& fileName)
{
    bool result = false;
    std::ifstream fin;
    fin.open(fileName, std::ios::in | std::ios::binary);
    fin.unsetf(std::ios::skipws);
    if (fin.is_open())
    {
        std::vector<char> readData(mBlockSize);
        fin.getline(&readData.at(0), mBlockSize);
        result = true;
    }
    return result;
}
