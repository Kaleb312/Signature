#include "SignatureProcessor.h"

SignatureProcessor::SignatureProcessor(const std::string& inFile, const std::string& outFile, unsigned int blockSize) :
    mFileReader(inFile, blockSize),
    mFileWriter(outFile, &mFileReader),
    mThreadPool(std::thread::hardware_concurrency() - 1) // one thread is for work with files
{
}

bool SignatureProcessor::openFiles()
{
    return mFileReader.openFile() && mFileWriter.openFile();
}

void SignatureProcessor::calcSignature()
{
    mFileReader.start();
    mFileWriter.start();
    try
    {
        while (!mFileReader.isFinished() || !mFileWriter.isFinished())
        {
            std::string dataBlock;
            if (mFileReader.getDataBlock(dataBlock))
            {
                std::cout << "SignatureProcessor: dataBlock.size = " << dataBlock.size() <<std::endl;
                auto futureHash = mThreadPool.processDataBlock(std::move(dataBlock));
                std::cout << "SignatureProcessor: got future of hash" <<std::endl;
                mFileWriter.pushFutureInQueue(std::move(futureHash));
                std::cout << "SignatureProcessor: future pushed to queue" <<std::endl;
                mFileWriter.post();
                std::cout << "SignatureProcessor: FileWriter.post()" <<std::endl;
            }
        }
        mFileWriter.finish();
        std::cout << "SignatureProcessor: FileWriter.finish()" <<std::endl;
        mFileReader.finish();
        std::cout << "SignatureProcessor: FileReader.finish()" <<std::endl;
    }
    catch (const std::exception& e)
    {
        mFileReader.stop();
        mFileWriter.stop();
        std::cout << "\nSignatureProcessor calcSignature() function exception caught: " << e.what() <<std::endl;
        std::exit(EXIT_FAILURE);
    }
}
