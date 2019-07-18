#ifndef SIGNATUREPROCESSOR_H
#define SIGNATUREPROCESSOR_H

#include "FileReader.h"

class SignatureProcessor
{
public:
    SignatureProcessor(std::string inFile, std::string outFile, unsigned int blockSize = 1);
    bool openFiles();
    void calcSignature();

private:
    FileReader mFileReader;
};

#endif // SIGNATUREPROCESSOR_H
