#ifndef SIGNATUREPROCESSOR_H
#define SIGNATUREPROCESSOR_H

#include "FileReader.h"
#include "ThreadPool.h"
class SignatureProcessor
{
public:
    SignatureProcessor(std::string inFile, std::string outFile, unsigned int blockSize = 1);
    bool openFiles();
    void calcSignature();

private:
    FileReader mFileReader;
    ThreadPool mThreadPool;
};

#endif // SIGNATUREPROCESSOR_H
