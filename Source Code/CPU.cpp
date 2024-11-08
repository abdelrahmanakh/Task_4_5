#include "CPU.h"
#include "ALU.h"

using namespace std;

void CPU::runNextStep(Memory &memory) {
    instructionRegister = memory.getCell(programCounter) + memory.getCell(programCounter + 1);
    char opType = instructionRegister[0];
    string r = string(1, instructionRegister[1]);
    string x = string(1, instructionRegister[2]);
    string y = string(1, instructionRegister[3]);

    int sr = alu.hexToDec("0" + r);
    int sx = alu.hexToDec("0" + x);
    int sy = alu.hexToDec("0" + y);
    int ss = alu.hexToDec(x + y);

    if (opType == '1')
        cu.load(sr, ss, reg, memory);
    else if (opType == '2')
        cu.load(sr, x + y, reg);
    else if (opType == '3')
        cu.store(sr, ss, reg, memory);
    else if (opType == '4')
        cu.move(sx, sy, reg);
    else if (opType == '5')
        alu.addInt(sr, sx, sy, reg);
    else if (opType == '6')
        alu.addFloat(sr, sx, sy, reg);
    else if (opType == '7')
        alu.OR(sr, sx, sy, reg);
    else if (opType == '8')
        alu.AND(sr, sx, sy, reg);
    else if (opType == '9')
        alu.XOR(sr, sx, sy, reg);
    else if (opType == 'A')
        alu.rotate(sr, sy, reg);
    else if (opType == 'B')
        cu.jump(sr, ss, reg, programCounter);
    else if (opType == 'C')
        cu.halt();
    else if (opType == 'D') {
        int d1 = alu.TWOs_comp(0, reg);
        int d2 = alu.TWOs_comp(sr, reg);
        if (d2 > d1)
            cu.jump_directly(ss, programCounter);
    }
}

void CPU::resetProgramCounter() {
    programCounter = 0;
}

void CPU::incrementProgramCounter() {
    programCounter += 2;
}