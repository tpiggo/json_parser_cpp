#include "reader.h"
#include <map>
#include <iostream>
#include "box.h"
#include "filereader.h"

using namespace std;

void testReader()
{
    FileReader df("test.json");

    while (df.hasNext())
    {
        char c = df.next();
        cout << c << endl;
    }
    cout << "Done reading" << endl;
    cout << "Has next" << df.hasNext() << " and index " << df.getError() << endl;
}



int main()
{
    const char *name = "";
    Box *item = readFromFile("test.json");
    BoxVisitor b = BoxVisitor();
    cout << "Done reading" << endl;
    item->accept(b);
    delete item;
    return 0;
}