#include <iostream>
#include <chrono>
#include "SignatureProcessor.h"
#include "ctpl_stl.h"

void first(int id) {
    std::cout << "hello from " << id << ", function\n";
}

void aga(int id, int par) {
    std::cout << "hello from " << id << ", function with parameter " << par <<'\n';
}

struct Third {
    Third(int v) { this->v = v; std::cout << "Third ctor " << this->v << '\n'; }
    Third(Third && c) { this->v = c.v; std::cout<<"Third move ctor\n"; }
    Third(const Third & c) { this->v = c.v; std::cout<<"Third copy ctor\n"; }
    ~Third() { std::cout << "Third dtor\n"; }
    int v;
};

int main(int argc, char* argv[])
{

    ctpl::thread_pool p(2 /* two threads in the pool */);
        std::future<void> qw = p.push(std::ref(first));  // function
        p.push(first);  // function
        p.push(aga, 7);  // function
        p.push([](int id){  // lambda
                std::this_thread::sleep_for(std::chrono::milliseconds(2000));
//                std::cout << "hello from " << id << ' ' << s << '\n';
            });
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
    SignatureProcessor proc(inputFileName, outputFileName);

    if (!proc.openFiles())
    {
        std::cout << "Error: bad input of output file" << std::endl;
    }
    else
    {
//        for (int i = 0; i < 100; ++i)
//        {
//            std::this_thread::sleep_for(std::chrono::seconds(1));
//            fileReader.getSemPtr()->post();
//        }
//        fileReader.stop();

//        std::this_thread::sleep_for(std::chrono::seconds(1));
//        fileReader.post();
//        std::this_thread::sleep_for(std::chrono::seconds(1));
//        fileReader.post();
//        std::this_thread::sleep_for(std::chrono::seconds(1));
//        fileReader.post();
//        std::this_thread::sleep_for(std::chrono::seconds(1));
//        fileReader.post();
//        std::this_thread::sleep_for(std::chrono::seconds(1));
//        fileReader.post();

    }
    std::cout << "\n-------FINISH----------\n" << std::endl;
    return 0;
}
