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
            if (blockSize < 1)
            {
                std::cout << "Error: minimal allowed block size is 1 Mb" << std::endl;
                return -1;
            }
            blockSize = static_cast<unsigned int>(std::stoi(argv[3]));
            if (blockSize >= AVAILABLE_MEMORY / MEGABYTE_SIZE)
            {
                std::cout << "Error: block size is too big. Try smaller than 1024 Mb" << std::endl;
                return -1;
            }
        }
        catch (const std::exception& e)
        {
            std::cout << "Error: block size exception: " << e.what() << std::endl;
            return -1;
        }
    }
    else
    {
        std::cout << "Error: wrong number of arguments. "
            "Please insert <input file> <output file> <block size>(default 1Mb)" << std::endl;
        return -1;
    }
    inputFileName = argv[1];
    outputFileName = argv[2];
    SignatureProcessor signatureProcessor(inputFileName, outputFileName, blockSize);
    int result = -1;
    if (!signatureProcessor.openFiles())
    {
        std::cout << "Error: bad input or output file" << std::endl;
    }
    else
    {
        result = static_cast<int>(signatureProcessor.calcSignature());
    }
    std::cout << "\nFinished: " << (result ? "FAILURE" : "SUCCESS") << std::endl;
    return 0;
}
