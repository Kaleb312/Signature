#ifndef FILEREADER_H
#define FILEREADER_H

#include <string>

class FileReader
{
public:
    FileReader();
    bool init(const std::string& fileName, unsigned int blockSize);
};

#endif // FILEREADER_H
