#ifndef MEMORY_H
#define MEMORY_H

#include <string>

using namespace std;

class Memory {
    string memory[256];
    const int size = 256;
public:
    Memory();

    string getCell(int address);

    void setCell(int address, string val);
};

#endif
