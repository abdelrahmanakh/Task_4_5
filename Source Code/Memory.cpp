#include "Memory.h"

using namespace std;

Memory::Memory() {
    for(int i = 0; i < size; ++i)
        memory[i] = "00";
}

string Memory::getCell(int address) {
    return memory[address];
}

void Memory::setCell(int address, string val) {
    memory[address] = val;
}