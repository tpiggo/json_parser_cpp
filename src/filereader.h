#ifndef FILEREADER_H
#define FILEREADER_H
#include <stdio.h>

class FileReader
{
private:
    FILE *fileptr;
    long filelen;
    int i;
    int charNum;
    int line;

public:
    FileReader(const char *fileName);
    ~FileReader();
    bool hasNext();
    char next();
    std::string *getError(std::string error);
    std::string *getError();
};
#endif