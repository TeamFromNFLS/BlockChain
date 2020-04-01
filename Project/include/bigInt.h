#ifndef BIG_INT_H
#define BIG_INT_H
#include <iostream>
#include <vector>
#include <cstring>
using namespace std;

class BigInt
{
public:
    void check();
    BigInt()
    {
        number.push_back(0);
        check();
    }
    void SetNumber(string s);
    string GetNumber();
    friend istream &operator>>(istream &input, BigInt &bigInt);
    friend ostream &operator<<(ostream &output, BigInt &bigInt);
    bool operator!=(BigInt bigInt);
    bool operator==(BigInt bigInt);
    bool operator<(BigInt bigInt);
    bool operator>(BigInt bigInt);
    bool operator<=(BigInt bigInt);
    bool operator>=(BigInt bigInt);
    void operator+=(BigInt bigInt);
    void operator-=(BigInt bigInt);
    void operator*=(BigInt bigInt);
    void operator/=(BigInt bigInt);
    BigInt operator+(BigInt bigInt);
    BigInt operator-(BigInt bigInt);
    BigInt operator*(BigInt bigInt);
    BigInt operator/(BigInt bigInt);
    BigInt operator%(BigInt bigInt);
    BigInt DivWithMod(BigInt bigInt);

private:
    vector<int> number;
};

#endif