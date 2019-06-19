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
    bool init(const std::string& fileName, int blockSize = 1);

private:
    std::string mFileName;
    int mBlockSize;
};

#endif // FILEREADER_H
