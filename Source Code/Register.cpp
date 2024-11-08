#include "Register.h"

Register::Register() {
    for (int i = 0; i < size; ++i)
        memory[i] = "00";
}

string Register::getCell(int address) {
    return memory[address];
}

void Register::setCell(int address, string val) {
    memory[address] = val;
}