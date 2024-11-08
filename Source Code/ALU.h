#ifndef ALU_H
#define ALU_H

#include "string"
#include "Register.h"

using namespace std;

class ALU {
    const int bias = 4;


    bool isValid(string hex);

    int d_fp(double x);

    double fp_d(int FP);

public:
    int value(char c);

    int hexToDec(string hex);

    string decToHex(int dec);

    int TWOs_comp(int R, Register &reg);

    void addInt(int R, int S, int T, Register &reg);

    void addFloat(int R, int S, int T, Register &reg);

    void OR(int R, int S, int T, Register &reg);

    void AND(int R, int S, int T, Register &reg);

    void XOR(int R, int S, int T, Register &reg);

    void rotate(int R, int x, Register &reg);
};

#endif