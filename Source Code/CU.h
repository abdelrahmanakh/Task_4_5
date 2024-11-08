#ifndef CU_H
#define CU_H

#include "string"
#include "Register.h"
#include "Memory.h"

class CU {
public:
    void load(int idxReg, int idxMem, Register &reg, Memory &memory);

    void load(int idxReg, string val, Register &reg);

    void move(int idxReg1, int idxReg2, Register &reg);

    void store(int idxReg, int idxMem, Register &reg, Memory &memory);

    void jump(int idxReg, int idxMem, Register &reg, int &PC);

    void jump_directly(int idxMem, int &PC);

    void halt();
};


#endif
