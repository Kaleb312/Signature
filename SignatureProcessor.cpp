#include "SignatureProcessor.h"

SignatureProcessor::SignatureProcessor(std::string inFile, std::string outFile, unsigned int blockSize) :
    mFileReader(inFile, blockSize)
{

}

bool SignatureProcessor::openFiles()
{
    return mFileReader.openFiles();
}
