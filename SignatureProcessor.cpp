#include "SignatureProcessor.h"

SignatureProcessor::SignatureProcessor(const std::string& inFile, const std::string& outFile, unsigned int blockSize) :
    mFileReader(inFile, blockSize),
    mFileWriter(outFile)
{

}

bool SignatureProcessor::openFiles()
{
    return mFileReader.openFile();
}

void SignatureProcessor::calcSignature()
{
    mFileReader.start();
    mFileWriter.start();
    while (!mFileReader.isFinished())
    {
        mFileWriter.pushFutureInList(mThreadPool.processDataBlock(mFileReader.getDataBlock()));
        mFileWriter.post();
    }
}
