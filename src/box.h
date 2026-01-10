#ifndef BOX_H
#define BOX_H

#include <iostream>
#include <vector>



class ArrayBox;
class StringBox;
class ObjectBox;
class NumericBox;

class BoxVisitor
{
public:
    void readString(StringBox &p);
    void readObjectBox(ObjectBox &p);
    void readArray(ArrayBox &p);
    void readNumeric(NumericBox &p);
};

// Used as a wildcard
class Box
{
public:
    virtual void accept(BoxVisitor &v) = 0;
};

class StringBox : public Box
{
private:
    std::string value;

public:
    StringBox(std::string aValue)
    {
        value = aValue;
    }

    std::string getValue()
    {
        return value;
    }

    void accept(BoxVisitor &p)
    {
        p.readString(*this);
    }
};

class ObjectBox : public Box
{
private:
    std::map<std::string, Box *> *value;

public:
    ObjectBox(std::map<std::string, Box *> *aValue)
    {
        value = aValue;
    }
    ~ObjectBox()
    {
        for (auto &element : *value)
        {
            delete &(element.second);
        }
    }

    std::map<std::string, Box *> *getValue()
    {
        return value;
    }

    void accept(BoxVisitor &p)
    {
        p.readObjectBox(*this);
    }
};

class ArrayBox : public Box
{
private:
    std::vector<Box *> *value;

public:
    ArrayBox(std::vector<Box *> *aValue)
    {
        value = aValue;
    }

    ~ArrayBox()
    {
        for (auto &element : *value)
        {
            delete element;
        }
    }
    std::vector<Box *> *getValue()
    {
        return value;
    }

    void accept(BoxVisitor &p)
    {
        p.readArray(*this);
    }
};

class NumericBox : public Box
{
private:
    std::string value;
    bool hasComma;
    bool isFloat;

public:
    NumericBox(char aValue)
    {
        value = std::string("" + aValue);
        isFloat = false;
    }

    void addValue(char nextValue)
    {
        const bool isComma = nextValue == ',';
        isFloat = nextValue == '.';

        if (isComma)
        {
            this->hasComma = isComma;
            return;
        }
        value += nextValue;
    }

    int getValue()
    {
        return stoi(value);
    }

    void accept(BoxVisitor &p)
    {
        p.readNumeric(*this);
    }
};

void BoxVisitor::readString(StringBox & p)
{
    std::cout << "Reading " << p.getValue() << std::endl;
}

void BoxVisitor::readObjectBox(ObjectBox &p)
{
    for (auto &element : *(p.getValue()))
    {
        std::cout << element.first << ": ";
        element.second->accept(*this);
    }
}


void BoxVisitor::readArray(ArrayBox &p)
{
    for (auto &element : *(p.getValue()))
    {
        element->accept(*this);
    }
}
void BoxVisitor::readNumeric(NumericBox &p)
{
    std::cout << "Reading " << p.getValue() << std::endl;
}
#endif