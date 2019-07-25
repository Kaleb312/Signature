#include <iostream>
#include <chrono>
#include "SignatureProcessor.h"

int main(int argc, char* argv[])
{
    auto blockSize {1};
    std::string inputFileName;
    std::string outputFileName;
    inputFileName = "in2.txt";
    outputFileName = "out.txt";
    const auto defaultArgumentNumber {3};
    const auto argumentNumberWithBlockSize {4};
//    if (argc == defaultArgumentNumber)
//    {
//        blockSize = 1;
//    }
//    else if (argc == argumentNumberWithBlockSize)
//    {
//        try
//        {
//            blockSize = std::stoi(argv[3]);
//        }
//        catch (const std::exception& e)
//        {
//            std::cout << "Error in block size!" << e.what() << std::endl;
//            return -1;
//        }
//    }
//    else
//    {
//        std::cout << "Wrong number of arguments! Please insert <input file> <output file> <block size>(default 1Mb)"
//            << std::endl;
//        return -1;
//    }
//    inputFileName = argv[1];
//    outputFileName = argv[2];


    SignatureProcessor proc(inputFileName, outputFileName);
    if (!proc.openFiles())
    {
        std::cout << "Error: bad input of output file" << std::endl;
    }
    else
    {
        proc.calcSignature();
    }
    std::cout << "\n-------FINISH----------\n" << std::endl;
    return 0;
}
