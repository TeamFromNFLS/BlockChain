#include "bigInt.h"
#include <iostream>
#include <vector>
#include <algorithm>
#include <cmath>
#include <cstring>
using namespace std;
vector<int>::iterator it;
void BigInt::check() // carry bit
{
    while (!number.empty() && !number.back())
        number.pop_back();
    if (number.empty())
    {
        return;
    }
    for (it = number.begin() + 1; it != number.end(); ++it)
    {
        *it += *(it - 1) / 10;
        *(it - 1) %= 10;
    }
    while (number.back() >= 10)
    {
        number.push_back(number.back() / 10);
        it = number.begin() + number.size() - 2;
        *it %= 10;
    }
    return;
}

/*BigInt::operator int()
{
    int result;
    for (it = number.begin(); it != number.end(); ++it)
    {
        result += *it * pow(10, (it - number.begin()));
    }
    return result;
}*/

void BigInt::SetNumber(string s)
{
    string::iterator _it;
    number.clear();
    for (_it = s.end() - 1; _it >= s.begin(); --_it)
        number.push_back(*_it - '0');
    check();
    return;
}

BigInt::BigInt(string s)
{
    SetNumber(s);
}

BigInt::BigInt(const int &x)
{
    string s = to_string(x);
    SetNumber(s);
}

istream &operator>>(istream &input, BigInt &bigInt)
{
    string s;
    input >> s;
    string::iterator _it;
    bigInt.number.clear();
    for (_it = s.end() - 1; _it >= s.begin(); --_it)
        bigInt.number.push_back(*_it - '0');
    return input;
}

ostream &operator<<(ostream &output, BigInt &bigInt)
{
    if (bigInt.number.empty())
        output << 0;
    for (it = bigInt.number.end() - 1; it >= bigInt.number.begin(); --it)
        output << *it;
    return output;
}

bool BigInt::operator!=(BigInt bigInt)
{
    return !(number == bigInt.number);
}

bool BigInt::operator==(BigInt bigInt)
{
    return number == bigInt.number;
}

bool BigInt::operator<(BigInt bigInt)
{
    if (number.size() != bigInt.number.size())
        return number.size() < bigInt.number.size();
    vector<int>::iterator _it;
    for (it = number.end() - 1, _it = bigInt.number.end() - 1; it >= number.begin(), _it >= bigInt.number.begin(); --it, --_it)
    {
        if (*it != *_it)
            return *it < *_it;
    }
    return false;
}

bool BigInt::operator>(BigInt bigInt)
{
    return bigInt < *this;
}

bool BigInt::operator<=(BigInt bigInt)
{
    return !(*this > bigInt);
}

bool BigInt::operator>=(BigInt bigInt)
{
    return !(*this < bigInt);
}

BigInt BigInt::operator+(BigInt bigInt)
{
    BigInt result;
    if (number.size() < bigInt.number.size())
    {
        number.resize(bigInt.number.size());
    }
    else if (number.size() > bigInt.number.size())
    {
        bigInt.number.resize(number.size());
    }
    vector<int>::iterator _it;
    for (it = number.begin(), _it = bigInt.number.begin(); it != number.end(), _it != bigInt.number.end(); ++it, ++_it)
    {
        result.number.push_back(*it + *_it);
    }
    result.check();
    return result;
}

BigInt &BigInt::operator+=(BigInt bigInt)
{
    *this = *this + bigInt;
    return *this;
}

BigInt BigInt::operator-(BigInt bigInt)
{
    BigInt minuend, subtrahend;
    minuend = *this;
    subtrahend = bigInt;
    if (minuend < subtrahend)
        swap(minuend, subtrahend);
    vector<int>::iterator _it;
    for (_it = subtrahend.number.begin(); _it != subtrahend.number.end(); ++_it)
    {
        it = minuend.number.begin() + (_it - subtrahend.number.begin());
        *it -= *_it;
        if (*it < 0)
        {
            vector<int>::iterator tmp = it + 1;
            while (!(*tmp))
            {
                ++tmp;
            }
            while (tmp > it)
            {
                --(*tmp);
                *(--tmp) += 10;
            }
        }
    }
    minuend.check();
    return minuend;
}

BigInt &BigInt::operator-=(BigInt bigInt)
{
    *this = *this - bigInt;
    return *this;
}

BigInt BigInt::operator*(BigInt bigInt)
{
    BigInt result;
    result.number.assign(number.size() + bigInt.number.size() - 1, 0);
    vector<int>::iterator _it, __it;
    for (it = number.begin(); it != number.end(); ++it)
    {
        for (_it = bigInt.number.begin(); _it != bigInt.number.end(); ++_it)
        {
            __it = result.number.begin() + (it - number.begin()) + (_it - bigInt.number.begin());
            *__it += *it * *_it;
        }
    }
    result.check();
    return result;
}

BigInt &BigInt::operator*=(BigInt bigInt)
{
    *this = *this * bigInt;
    return *this;
}

BigInt BigInt::operator/(BigInt bigInt) // Divide with mod, using minus to realise
{
    BigInt result;
    BigInt substitute = *this;
    for (int i = number.size() - bigInt.number.size(); substitute >= bigInt; --i)
    {
        BigInt tmp; // pow(bigInt, 10)
        tmp.number.assign(i + 1, 0);
        tmp.number.back() = 1;
        BigInt _tmp = bigInt * tmp; // ready to minus
        int cnt = 1;
        while (substitute >= _tmp)
        {
            substitute -= _tmp;
            result += tmp;
        }
    }
    result.check();
    return result;
}

BigInt &BigInt::operator/=(BigInt bigInt)
{
    *this = *this / bigInt;
    return *this;
}

BigInt BigInt::operator%(BigInt bigInt)
{
    BigInt substitute = *this;
    for (int i = number.size() - bigInt.number.size(); substitute >= bigInt; --i)
    {
        BigInt tmp; // pow(bigInt, 10)
        tmp.number.assign(i + 1, 0);
        tmp.number.back() = 1;
        BigInt _tmp = bigInt * tmp; // ready to minus
        while (substitute >= _tmp)
        {
            substitute -= _tmp;
        }
    }
    substitute.check();
    return substitute;
}

BigInt &BigInt::operator%=(BigInt bigInt)
{
    *this = *this % bigInt;
    return *this;
}

BigInt BigInt::PowMod(BigInt base, BigInt index, BigInt mod)
{
    BigInt result = 1;
    while (!index.number.empty())
    {
        BigInt tmp;
        tmp = index % 2;
        if (!tmp.number.empty())
        {
            result *= base;
        }
        base = base * base;
        base %= mod;
        index /= 2;
        result %= mod;
    }
    return result;
}

BigInt BigInt::Pow(BigInt base, BigInt index)
{
    BigInt result = 1;
    while (!index.number.empty())
    {
        BigInt tmp;
        tmp = index % 2;
        if (!tmp.number.empty())
        {
            result *= base;
        }
        base = base * base;
        index /= 2;
    }
    return result;
}