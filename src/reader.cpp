#include <iostream>
#include <fstream>
#include <map>
#include <vector>
#include <numeric>
#include "filereader.h"
#include "stack.h"
#include "reader.h"
#include "invalid_json.h"

using namespace std;

enum Token
{
    OPEN_C = '{',
    OPEN_S = '[',
    CLOSE_S = ']',
    CLOSE_C = '}',
    STRING_START = '"',
    COLON = ':',
    COMMA = ',',
};

enum Type
{
    STRING,
    DOUBLE,
    INT,
    OBJECT,
    ARRAY,
};

string readString(FileReader *fr)
{
    string characters = string("");
    while (fr->hasNext())
    {
        char c = fr->next();
        if (c == STRING_START)
        {
            return characters;
        }
        characters.push_back(c);
    }
    throw InvalidJson(fr->getError("Invalid JSON, invalid shape")->c_str());
}

const map<char, char> token_mapping = {
    {CLOSE_C, OPEN_C},
    {CLOSE_S, OPEN_S},
    {STRING, STRING},
};
char nextToken(char t)
{

    if (token_mapping.count(t))
    {
        return token_mapping.at(t);
    }
    return t;
}

char findNext(FileReader *fr)
{
    while (fr->hasNext())
    {
        char ch = fr->next();
        if (ch != ' ' && ch != char(13) && ch != char(10) && ch != '\t')
        {
            return ch;
        }
    }
    throw InvalidJson("unexpected ending");
}

// Declared to be able to call them
Box *readValue(Stack<char> *s, FileReader *fr);
ObjectBox *readJson(Stack<char> *s, FileReader *fr);

ObjectBox *readJson(Stack<char> *s, FileReader *fr)
{
    auto json = map<string, Box *>();
    while (fr->hasNext())
    {
        char ch = findNext(fr);
        if (ch != STRING_START)
        {
            const char *msg = fr->getError("Invalid JSON, invalid shape, keys must be strings at char ")->c_str();
            throw InvalidJson(msg);
        }
        string foundString = readString(fr);
        ch = fr->next();
        if (ch != COLON)
        {
            const char *msg = fr->getError("Invalid JSON, invalid shape, key must be followed by colon ")->c_str();
            throw InvalidJson(msg);
        }
        Box *value = readValue(s, fr);
        ch = findNext(fr);
        json.insert({foundString, value});
        if (nextToken(ch) == s->peek())
        {
            s->pop();
            return new ObjectBox(json);
        }
        if (ch != COMMA && nextToken(ch) != s->peek())
        {
            // invalid structure
            const char *msg = fr->getError("Invalid JSON, invalid shape, expecting comma or closing bracket of some kind")->c_str();
            throw InvalidJson(msg);
        }
    }
    throw InvalidJson("Invalid JSON, invalid shape: Json never terminated");
}

NumericBox *readNumeric(NumericBox *box, FileReader *fr)
{
    while (fr->hasNext())
    {
        char ch = fr->next();
        if ((ch >= '0' && ch <= '9') || ch == '.')
        {
            box->addValue(ch);
        }
        else if (ch == COMMA)
        {
            return box;
        }
    }
    const char *msg = fr->getError("Invalid JSON, invalid shape, file ended ")->c_str();
    throw InvalidJson(msg);
}

ArrayBox *readArray(Stack<char> *s, FileReader *fr)
{
    vector<Box *> *boxes = new vector<Box *>();
    while (fr->hasNext())
    {
        Box *value = readValue(s, fr);
        if (!fr->hasNext())
        {
            const char *msg = fr->getError("Invalid JSON, invalid shape, did not start with open curly or square bracket")->c_str();
            throw InvalidJson(msg);
        }
        char ch = fr->next();
        if (ch != COMMA && s->peek() != nextToken(ch))
        {
            const char *msg = fr->getError("Invalid JSON, invalid shape, expecting comma or square bracket closure")->c_str();
            throw InvalidJson(msg);
        }
        boxes->push_back(value);
        if (s->peek() == nextToken(ch))
        {
            s->pop();
            return new ArrayBox(boxes);
        }
    }
    string msg = fr->getError("Invalid JSON, invalid shape: file ended without closure")->c_str();
    throw InvalidJson(msg.c_str());
}

Box *readValue(Stack<char> *s, FileReader *fr)
{
    if (!fr->hasNext())
    {
        const char *msg = fr->getError("Invalid JSON, invalid shape, file ended")->c_str();
        throw InvalidJson(msg);
    }
    char ch = findNext(fr);
    if (ch >= '0' && ch <= '9')
    {
        return readNumeric(new NumericBox(ch), fr);
    }
    if (ch == STRING_START)
    {

        return new StringBox(readString(fr));
    }
    else if (ch == OPEN_C)
    {
        s->push(ch);
        return readJson(s, fr);
    }
    else if (ch == OPEN_S)
    {
        s->push(ch);
        return readArray(s, fr);
    }
    const char *msg = fr->getError("Invalid JSON, invalid character")->c_str();
    throw InvalidJson(msg);
}

Box *readFromFile(const char *fileName)
{
    Stack<char> s = Stack<char>();
    FileReader fr(fileName);
    while (fr.hasNext())
    {
        if (s.isEmpty())
        {
            char ch = fr.next();
            if (ch != OPEN_C && ch != OPEN_S)
            {
                const char *msg = fr.getError("Invalid JSON, invalid shape, did not start with open curly or square bracket")->c_str();
                throw InvalidJson(msg);
            }
            s.push(ch);
        }
        else if (s.peek() == OPEN_C)
        {
            return readJson(&s, &fr);
        }
        else if (s.peek() == OPEN_S)
        {
            return readArray(&s, &fr);
        }
    }
    const char *msg = fr.getError("Invalid JSON, invalid shape: file ended without closure")->c_str();
    throw InvalidJson(msg);
}

Box *readFromFile(string fileName)
{
    return readFromFile(fileName.c_str());
}
