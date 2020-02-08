#include "SignatureProcessor.h"

static const unsigned int ALWAYS_BUSY_THREADS_NUMBER = 3;

SignatureProcessor::SignatureProcessor(const std::string& inFile, const std::string& outFile, unsigned int blockSize) :
    mFileReader(inFile, blockSize),
    mFileWriter(outFile, mFileReader)
{
    const unsigned int notBusyCores = std::thread::hardware_concurrency() - ALWAYS_BUSY_THREADS_NUMBER;
    if (notBusyCores > ALWAYS_BUSY_THREADS_NUMBER) // thread writer, thread reader, main thread are busy; the rest cores can be used in thread pool
    {
        mThreadPool.init(notBusyCores);
    }
    else
    {
        mThreadPool.init(1);
    }
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
