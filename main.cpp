#include <iostream>
#include "FileReader.h"

int main(int argc, char* argv[])
{
    auto blockSize {0};
    std::string inputFileName;
    std::string outputFileName;
    const auto defaultArgumentNumber {3};
    const auto argumentNumberWithBlockSize {4};
    if (argc == defaultArgumentNumber)
    {
        blockSize = 1;
    }
    else if (argc == argumentNumberWithBlockSize)
    {
        try
        {
            blockSize = std::stoi(argv[3]);
        }
        catch (const std::exception& e)
        {
            std::cout << "Error in block size!" << e.what() << std::endl;
            return -1;
        }
    }
    else
    {
        std::cout << "Wrong number of arguments! Please insert <input file> <output file> <block size>(default 1Mb)"
            << std::endl;
        return -1;
    }
    inputFileName = argv[1];
    outputFileName = argv[2];
    FileReader fileReader;
    fileReader.init(inputFileName, blockSize);
    std::cout << "sd" << std::endl;
    return 0;
}
