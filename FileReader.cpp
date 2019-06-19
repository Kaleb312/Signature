#include "FileReader.h"

static const unsigned int THOUSAND_BYTES = 1024;

FileReader::FileReader() :
    mBlockSize(0)
{
}

bool FileReader::init(const std::string& fileName, int blockSize)
{
    bool result = false;
    std::ifstream fin;
    fin.open(fileName, std::ios::in | std::ios::binary);
    fin.unsetf(std::ios::skipws);
    if (!fin.is_open())
    {
        std::cout << "Bad input file!" << std::endl;
    }
    else
    {

        result = true;
    }
    return result;
}
