#ifndef BIG_INT_H
#define BIG_INT_H
#include <iostream>
#include <vector>
#include <algorithm>
using namespace std;
class BigInt // store in vector, easy to operate
{
public:
    static int Shrink(vector<uint64_t> &vec); // update bit, calculate the bit length of a number
    void check();                             // update
    BigInt()
    {
        number.push_back(0);
        check();
    }
    BigInt(const BigInt &bigInt)
    {
        number.assign(bigInt.number.begin(), bigInt.number.end());
        bit = bigInt.bit;
    }
    BigInt(string s);
    BigInt(const vector<uint64_t> &vec) : number(vec)
    {
        check();
    }
    ~BigInt(){};
    //operator int();
    BigInt(const int &x);                       // implicit type change from int to BigInt
    void SetNumber(string s);                   // use a string to initialize and adjust the number
    bool GetNumber(vector<uint64_t> &vec) const // use another vector to contain the number
    {
        vec.assign(number.begin(), number.end());
        reverse(vec.begin(), vec.end());
        return true;
    }
    int GetBit() const
    {
        return bit;
    }
    friend istream &operator>>(istream &input, BigInt &bigInt);
    friend ostream &operator<<(ostream &output, BigInt &bigInt);
    // overload operators
    bool operator!=(BigInt bigInt);
    bool operator==(BigInt bigInt);
    bool operator<(BigInt bigInt);
    bool operator>(BigInt bigInt);
    bool operator<=(BigInt bigInt);
    bool operator>=(BigInt bigInt);
    BigInt &operator+=(const BigInt &bigInt);
    BigInt operator+(const BigInt &bigInt);
    BigInt &operator-=(const BigInt &bigInt);
    BigInt operator-(const BigInt &bigInt);
    BigInt &operator*=(const BigInt &bigInt);
    BigInt operator*(const BigInt &bigInt);
    BigInt &operator/=(const BigInt &bigInt);
    BigInt operator/(const BigInt &bigInt);
    BigInt operator%(const BigInt &bigInt);
    BigInt &operator%=(const BigInt &bigInt);
    static BigInt PowMod(BigInt &a, BigInt &e, BigInt &mod);
    static const BigInt zero;
    static const BigInt one;
    static const BigInt two;

private:
    /*The LSB of number[0] means one place, and so on*/
    vector<uint64_t> number;                                         // stored in inverted order, 2^32 system, complement
    int bit = 0;                                                     // how many bits are there in number
    string DecToBin(string s);                                       // convert the input
    void SetBin(string s);                                           // set with a binary string
    pair<BigInt, BigInt> DivMod(const BigInt &_a, const BigInt &_b); // division with remainder
    BigInt GetResidualWithInverse(BigInt &a, BigInt &inv) const;
    BigInt Reverse(int length) const;
    BigInt RightShift(int length) const;
    void ComputeInverse();
    bool InverseComputed = false;
    vector<uint64_t> numberInverse; // numberInverse的LSB表示2^{-2p}, p = m_cnt_bits
};

#endif