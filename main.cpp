#include <iostream>
#include <chrono>
#include "FileReader.h"

int main(int argc, char* argv[])
{
    auto blockSize {1};
    std::string inputFileName;
    std::string outputFileName;
    inputFileName = "in.txt";
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
    FileReader fileReader;
    if (!fileReader.openFile(inputFileName))
    {
        std::cout << "Bad input file!" << std::endl;
    }
    else
    {
        fileReader.start();
//        for (int i = 0; i < 100; ++i)
//        {
//            std::this_thread::sleep_for(std::chrono::seconds(1));
//            fileReader.getSemPtr()->post();
//        }
//        fileReader.stop();

        std::this_thread::sleep_for(std::chrono::seconds(1));
        fileReader.post();
        std::this_thread::sleep_for(std::chrono::seconds(1));
        fileReader.post();
        std::this_thread::sleep_for(std::chrono::seconds(1));
        fileReader.post();
        std::this_thread::sleep_for(std::chrono::seconds(1));
        fileReader.post();
        std::this_thread::sleep_for(std::chrono::seconds(1));
        fileReader.post();

    }
    std::cout << "\n-------FINISH----------\n" << std::endl;
    return 0;
}
