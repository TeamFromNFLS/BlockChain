#ifndef BIG_INT_H
#define BIG_INT_H
#include <iostream>
#include <vector>
#include <algorithm>
using namespace std;
typedef vector<unsigned int> vec;
class BigInt // store in vector, easy to operate
{
public:
    void check(); //carry bit
    BigInt()
    {
        number.push_back(0);
        check();
    }
    BigInt(const BigInt &bigInt)
    {
        number.assign(bigInt.number.begin(), bigInt.number.end());
    }
    BigInt(string s);
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
    friend istream &operator>>(istream &input, BigInt &bigInt);
    friend ostream &operator<<(ostream &output, BigInt &bigInt);
    // overload operators
    bool operator!=(BigInt bigInt);
    bool operator==(BigInt bigInt);
    bool operator<(BigInt bigInt);
    bool operator>(BigInt bigInt);
    bool operator<=(BigInt bigInt);
    bool operator>=(BigInt bigInt);
    BigInt &operator+=(BigInt &bigInt);
    BigInt operator+(BigInt &bigInt);
    BigInt &operator-=(BigInt bigInt);
    BigInt operator-(BigInt bigInt);
    BigInt &operator*=(BigInt bigInt);
    BigInt operator*(BigInt bigInt);
    BigInt &operator/=(BigInt bigInt);
    BigInt operator/(BigInt bigInt);
    BigInt operator%(BigInt bigInt);
    BigInt &operator%=(BigInt bigInt);
    static BigInt PowMod(BigInt base, BigInt index, BigInt mod);
    static BigInt Pow(BigInt base, BigInt index);
    static const uint64_t Base = 4294967296;
    static const BigInt zero;
    static const BigInt one;
    static const BigInt two;

private:
    /*The LSB of number[0] means one place, and so on*/
    vector<uint64_t> number;           // stored in inverted order, 2^32 system, complement
    bool sign = true;                  // positive or negative
    string DecToBin(string s);         // convert the input
    void SetBin(string s, bool _sign); // set with a binary string
};

#endif