#include "SignatureProcessor.h"

SignatureProcessor::SignatureProcessor(const std::string& inFile, const std::string& outFile, unsigned int blockSize) :
    mFileReader(inFile, blockSize),
    mFileWriter(outFile, mFileReader),
    mThreadPool(std::thread::hardware_concurrency() - 1) // one thread is for work with files
{
}

bool SignatureProcessor::openFiles()
{
    return mFileReader.openFile() && mFileWriter.openFile();
}

ReturnValue SignatureProcessor::calcSignature()
{
    ReturnValue result = ReturnValue::FAILURE;
    mFileReader.start();
    mFileWriter.start();
    try
    {
        while (!mFileReader.isFinished() || !mFileWriter.isFinished())
        {
            std::string dataBlock;
            if (mFileReader.getDataBlock(dataBlock))
            {
                auto futureHash = mThreadPool.processDataBlock(std::move(dataBlock));
                mFileWriter.pushFutureInQueue(std::move(futureHash));
                mFileWriter.post();
            }
        }
        mFileWriter.finish();
        mFileReader.finish();
        result = ReturnValue::SUCCESS;
    }
    catch (const std::exception& e)
    {
        mFileReader.stop();
        mFileWriter.stop();
        std::cout << "\nSignatureProcessor calcSignature() exception caught: " << e.what() <<std::endl;
    }
    return result;
}
