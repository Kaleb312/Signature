#include <iostream>
#include "SignatureProcessor.h"

int main(int argc, char* argv[])
{
    unsigned int blockSize {1};
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
            blockSize = static_cast<unsigned int>(std::stoi(argv[3]));
            if (blockSize >= AVAILABLE_MEMORY / MEGABYTE_SIZE)
            {
                std::cout << "Block size is too big. Try smaller than 1024 Mb" << std::endl;
                return -1;
            }
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
    SignatureProcessor proc(inputFileName, outputFileName, blockSize);
    if (!proc.openFiles())
    {
        std::cout << "Error: bad input of output file" << std::endl;
        return -1;
    }
    else
    {
        proc.calcSignature();
    }
    std::cout << "\n-------FINISH----------\n" << std::endl;
    return 0;
}
