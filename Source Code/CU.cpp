#include "CU.h"

void CU::load(int idxReg, int idxMem, Register &reg, Memory &memory) {
    reg.setCell(idxReg, memory.getCell(idxMem));
}

void CU::load(int idxReg, string val, Register &reg) {
    reg.setCell(idxReg, val);
}

void CU::store(int idxReg, int idxMem, Register &reg, Memory &memory) {
    memory.setCell(idxMem, reg.getCell(idxReg));
}

void CU::move(int idxReg1, int idxReg2, Register &reg) {
    reg.setCell(idxReg2, reg.getCell(idxReg1));
}

void CU::jump(int idxReg, int idxMem, Register &reg, int &PC) {
    if (reg.getCell(0) == reg.getCell(idxReg))
        PC = idxMem;
}

void CU::jump_directly(int idxMem, int &PC) {
    PC = idxMem;
}

void CU::halt() {
    exit(0);
}