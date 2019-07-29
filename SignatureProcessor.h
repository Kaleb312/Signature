#ifndef SIGNATUREPROCESSOR_H
#define SIGNATUREPROCESSOR_H

#include "FileReader.h"
#include "ThreadPool.h"
#include "FileWriter.h"

class SignatureProcessor
{
public:
    SignatureProcessor() = delete;
    SignatureProcessor(const std::string& inFile, const std::string& outFile, unsigned int blockSize = 1);
    bool openFiles();
    void calcSignature();

private:
    FileReader mFileReader;
    FileWriter mFileWriter;
    ThreadPool mThreadPool;
};

#endif // SIGNATUREPROCESSOR_H
