#ifndef REGISTER_H
#define REGISTER_H

#include "string"
using namespace std;

class Register {
    string memory[16];
    const int size = 16;
public:
    Register();

    string getCell(int address);

    void setCell(int address, string val);
};

#endif
