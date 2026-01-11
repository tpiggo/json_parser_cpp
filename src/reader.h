
#ifndef READER_H
#define READER_H
#include <map>
#include <iostream>
#include "box.h"
Box *readFromFile(const char *fileName);
Box *readFromFile(std::string fileName);
#endif