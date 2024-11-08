#include "ALU.h"
#include "Register.h"
#include <cmath>

int ALU::value(char c) {
    if (isalpha(c))
        return c - 'A' + 10;
    return c - '0';
}

int ALU::hexToDec(string hex) {
    return value(hex[0]) * 16 + value(hex[1]);
}

int ALU::TWOs_comp(int R, Register &reg) {
    int dec = hexToDec(reg.getCell(R));
    int ans = dec & 127;
    if (dec & 128) {
        ans -= 128;
    }
    return ans;
}

string ALU::decToHex(int dec) {
    string hex;
    dec %= 256;
    if (dec / 16 < 10)
        hex += dec / 16 + '0';
    else
        hex += dec / 16 + 'A' - 10;

    if (dec % 16 < 10)
        hex += dec % 16 + '0';
    else
        hex += dec % 16 + 'A' - 10;

    return hex;
}

bool ALU::isValid(string hex) {
    if (hex.size() != 2)
        return false;
    for (char c: hex)
        if ((c < '0' || c > '9') && (c < 'A' || c > 'F'))
            return false;
    return true;
}

int ALU::d_fp(double x) {
    int FP = 0;
    if (x < 0) {
        FP |= 1 << 7;
        x *= -1;
    }

    int exponent = bias;
    while (x >= 1) {
        x /= 2;
        exponent++;
    }
    FP |= exponent << 4;

    int mantissa = x * 100;
    while (mantissa >= 16)
        mantissa /= 10;

    FP |= mantissa;
    return FP;
}

double ALU::fp_d(int FP) {
    int sign = FP & 128;
    int exponent = (FP & 112) >> 4;
    int mantissa = FP & 15;

    double M = mantissa;
    while (M >= 1)
        M /= 10;

    return pow(-1, sign) * M * pow(2, exponent - bias);
}

void ALU::addInt(int R, int S, int T, Register &reg) {
    string hex1 = reg.getCell(S);
    string hex2 = reg.getCell(T);

    int dec1 = hexToDec(hex1);
    int dec2 = hexToDec(hex2);
    reg.setCell(R, decToHex(dec1 + dec2));
}

void ALU::addFloat(int R, int S, int T, Register &reg) {
    string hex1 = reg.getCell(S);
    string hex2 = reg.getCell(T);

    int dec1 = hexToDec(hex1);
    int dec2 = hexToDec(hex2);
    reg.setCell(R, decToHex(d_fp(fp_d(dec1) + fp_d(dec2))));
}

void ALU::OR(int R, int S, int T, Register &reg) {
    string hex1 = reg.getCell(S);
    string hex2 = reg.getCell(T);

    int dec1 = hexToDec(hex1);
    int dec2 = hexToDec(hex2);
    reg.setCell(R, decToHex(dec1 | dec2));
}

void ALU::AND(int R, int S, int T, Register &reg) {
    string hex1 = reg.getCell(S);
    string hex2 = reg.getCell(T);

    int dec1 = hexToDec(hex1);
    int dec2 = hexToDec(hex2);
    reg.setCell(R, decToHex(dec1 & dec2));
}

void ALU::XOR(int R, int S, int T, Register &reg) {
    string hex1 = reg.getCell(S);
    string hex2 = reg.getCell(T);

    int dec1 = hexToDec(hex1);
    int dec2 = hexToDec(hex2);
    reg.setCell(R, decToHex(dec1 ^ dec2));
}

void ALU::rotate(int R, int x, Register &reg) {
    string hex = reg.getCell(R);
    int dec = hexToDec(hex);
    x %= 8;
    if (x == 0)
        return;
    int t = dec & ((1 << x) - 1);
    dec >>= x;
    dec |= t << (8 - x);
    reg.setCell(R, decToHex(dec));
}

