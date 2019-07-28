#ifndef SIGNATUREPROCESSOR_H
#define SIGNATUREPROCESSOR_H

#include "FileReader.h"
#include "ThreadPool.h"
#include "FileWriter.h"

class SignatureProcessor
{
public:
    SignatureProcessor(const std::string& inFile, const std::string& outFile, unsigned int blockSize = 1);
    SignatureProcessor() = delete;
    bool openFiles();
    void calcSignature();

private:
    FileWriter mFileWriter;
    FileReader mFileReader;
    ThreadPool mThreadPool;
};

#endif // SIGNATUREPROCESSOR_H
