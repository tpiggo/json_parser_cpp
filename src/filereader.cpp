#include <iostream>
#include <stdio.h>
#include "filereader.h"
#include "invalid_json.h"

FileReader::FileReader(const char *fileName)
{
    this->fileptr = fopen(fileName, "r");
    fseek(fileptr, 0, SEEK_END);
    this->filelen = ftell(this->fileptr);
    rewind(this->fileptr);
    std::cout << "File length is " << this->filelen << std::endl;
    this->i = 0;
    this->charNum = 0;
    this->line = 0;
}
FileReader::~FileReader()
{
    fclose(this->fileptr);
}
bool FileReader::hasNext()
{
    return this->i < this->filelen;
}
char FileReader::next()
{
    char *c = (char *)malloc(1);
    fread(c, 1, 1, fileptr);
    this->i++;
    this->charNum++;
    char actualChar = *c;
    if (actualChar == '\n')
    {
        this->line++;
        this->charNum = 0;
    }
    delete c;
    return actualChar;
}

std::string *FileReader::getError(std::string error)
{
    return new std::string(error + " at (Line " + std::to_string(this->line) + ", character " + std::to_string(this->charNum) + ")");
}
std::string *FileReader::getError()
{
    return this->getError("");
}