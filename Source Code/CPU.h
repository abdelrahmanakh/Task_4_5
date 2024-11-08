#ifndef CPU_H
#define CPU_H

#include "string"
#include "ALU.h"
#include "Register.h"
#include "CU.h"
#include "Memory.h"

class CPU {
private:
    std::string instructionRegister;
    ALU alu;
    CU cu;

public:
    Register reg;
    int programCounter = 0;
    CPU() = default;
    void runNextStep(Memory &memory);
    void resetProgramCounter();
    void incrementProgramCounter();
};

#endif