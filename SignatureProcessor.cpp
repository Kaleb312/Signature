#include "SignatureProcessor.h"

SignatureProcessor::SignatureProcessor(const std::string& inFile, const std::string& outFile, unsigned int blockSize) :
    mFileReader(inFile, blockSize),
    mFileWriter(outFile),
    mThreadPool(std::thread::hardware_concurrency())
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
    while (!mFileReader.isFinished() || !mFileWriter.isFinished())
    {
        if (mFileReader.isDataReady())
        {
            mFileWriter.pushFutureInList(mThreadPool.processDataBlock(mFileReader.getDataBlock()));
            mFileWriter.post();
        }
    }
    mFileReader.finish();
    mFileWriter.finish();
}
