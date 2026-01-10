
#ifndef INVALIDJSON_H
#define INVALIDJSON_H
#include <iostream>

class InvalidJson : public std::runtime_error
{
public:
    InvalidJson(const char *what) : std::runtime_error(what)
    {
    }
};
#endif