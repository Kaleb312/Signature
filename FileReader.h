#ifndef FILEREADER_H
#define FILEREADER_H

#include <string>
#include <fstream>
#include <iostream>
#include <vector>

class FileReader
{
public:
    FileReader();
    FileReader(unsigned int blockSize);
    bool readFile(const std::string& fileName);

private:
    std::string mFileName;
    unsigned int mBlockSize;
};

#endif // FILEREADER_H
