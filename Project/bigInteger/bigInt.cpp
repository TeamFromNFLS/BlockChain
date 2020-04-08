#include "bigInt.h"
#include <iostream>
#include <vector>
#include <algorithm>
#include <cmath>
#include <cstring>
#include <cstdio>
using namespace std;

/*define some const bigInt*/
const BigInt BigInt::zero("0");
const BigInt BigInt::one("1");
const BigInt BigInt::two("2");
/*carry bit*/
void BigInt::check()
{
    while (!number.empty() && !number.back())
        number.pop_back();
    if (number.empty())
    {
        return;
    }
    int len = number.size();
    for (int i = 1; i < len; ++i)
    {
        number[i] += number[i - 1] / Base;
        number[i - 1] %= Base;
    }
    while (number.back() >= Base)
    {
        number.push_back(number.back() / Base);
        number[len - 2] %= Base;
    }
    return;
}

/*simulate x / 2, x % 2 in string*/
string BigInt::DecToBin(string s)
{
    string result;
    string::iterator it;
    vector<int> tmp; // number vector
    for (it = s.end() - 1; it >= s.begin(); --it)
    {
        tmp.push_back(*it - '0');
    }
    while (!tmp.empty())
    {
        result += (tmp[0] & 1) + '0';
        for (int i = s.size() - 1; i >= 0; --i)
        {
            if (tmp[i] & 1 && i)
            {
                tmp[i - 1] += 10;
            }
            tmp[i] /= 2;
        }
        while (!tmp.back() && !tmp.empty())
        {
            tmp.pop_back();
        }
    }
    reverse(result.begin(), result.end());
    return result;
}

void BigInt::SetBin(string s, bool _sign)
{
    int len = s.length(), cnt = 0, pos = -1;
    string::iterator it = s.end() - 1;
    if (!_sign)
    {
        sign = false;
    }
    while (len)
    {
        if (!cnt)
        {
            cnt = 0;
            pos++;
            number.push_back(0);
        }
        int now = *it - '0';
        if (now)
        {
            number[pos] += pow(2, cnt);
        }
        len--;
        it--;
        cnt = (cnt + 1) % 32;
    }
    check();
    return;
}

/*I don't think there's anyone who wants to input a number with bin or hex system*/
void BigInt::SetNumber(string s)
{
    string::iterator it;
    bool _sign = true;
    if (s[0] == '-')
    {
        _sign = false;
        s.erase(0, 1);
    }
    string ss = DecToBin(s);
    SetBin(ss, _sign);
}

/*initialize, convert the input into binary*/
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
    bigInt.number.clear();
    bigInt.SetNumber(s);
    return input;
}

char _hex[16] = {'0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'a', 'b', 'c', 'd', 'e', 'f'};
ostream &operator<<(ostream &output, BigInt &bigInt)
{
    //freopen("out.txt", "w", stdout);
    if (bigInt.number.empty())
    {
        output << 0;
        return output;
    }
    vector<int> resultBin;
    int cnt = 0, now = 0;
    resultBin.resize(64 * bigInt.number.size());
    for (int i = 0; i < bigInt.number.size(); ++i)
    {
        uint32_t tmp = bigInt.number[i];
        while (tmp)
        {
            resultBin[i * 32 + cnt] = tmp & 1;
            tmp >>= 1;
            cnt = (cnt + 1) % 32;
        }
    }
    reverse(resultBin.begin(), resultBin.end());
    bool flag = false; // whether it is the zero in front of the number
    cnt = 0;
    output << "Bin: ";
    if (!bigInt.sign)
    {
        output << '-';
    }
    for (int i = 0; i < resultBin.size(); ++i)
    {
        if (cnt == 4)
        {
            output << ends;
            cnt = 0;
        }
        output << resultBin[i];
        cnt++;
    }
    output << endl
           << "Hex: ";
    cnt = 0;
    if (!bigInt.sign)
    {
        output << '-';
    }
    output << "0x";
    for (int i = 0; i < resultBin.size(); ++i)
    {
        if (cnt == 4)
        {
            if (now || flag)
            {
                flag = true;
                output << _hex[now];
            }
            now = 0;
            cnt = 0;
        }
        if (resultBin[i])
        {
            now += pow(2, 3 - cnt);
        }
        cnt++;
    }
    if (now || flag)
    {
        output << _hex[now];
    }
    //fclose(stdout);
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
    int len = number.size();
    for (int i = len - 1; i >= 0; --i)
    {
        if (number[i] != bigInt.number[i])
        {
            return number[i] < bigInt.number[i];
        }
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

inline void __asm_add(vector<uint64_t> &a, vector<uint64_t> &b, vector<uint64_t> &c)
{
    //默认a, b长度相等, c.size() = a.size() + 1
    uint64_t *pt_a = &a[0],
             *pt_b = &b[0],
             *pt_c = &c[0];
    uint64_t len = a.size();
    asm volatile(
        "movq %0, %%r8;"
        "movq %1, %%r9;"
        "movq %2, %%r10;"
        "movzx %3, %%rdx;"
        "movq $0, %%rcx;"
        "clc;"
        "lahf;"
        "loop_add:"
        "sahf;" //从寄存器中读取flag
        "movq (%%r8, %%rcx, 8), %%rax;"
        "movq (%%r9, %%rcx, 8), %%rbx;"
        "adc %%rax, %%rbx;"
        "lahf;" //保存flag,后面的inc可能会破坏carry
        "movq %%rbx, (%%r10, %%rcx, 8);"
        "inc %%rcx;"
        "cmp %%rcx, %%rdx;"
        "jne loop_add;"
        "movq $0, (%%r10, %%rcx, 8);"
        "sahf;"
        "adc $0, (%%r10, %%rcx, 8);"
        :
        : "m"(pt_a), "m"(pt_b), "m"(pt_c), "m"(len)
        : "memory", "cc", "r8", "r9", "r10", "rdx", "rcx", "rax", "rbx");
}

BigInt BigInt::operator+(BigInt &bigInt)
{
    /*BigInt result;
    if (number.size() < bigInt.number.size())
    {
        number.resize(bigInt.number.size());
    }
    else if (number.size() > bigInt.number.size())
    {
        bigInt.number.resize(number.size());
    }
    for (int i = 0; i < number.size(); ++i)
    {
        result.number.push_back(number[i] + bigInt.number[i]);
    }
    result.check();
    */
    vector<uint64_t> a = number, b = bigInt.number;
    int maxLen = max(a.size(), b.size());
    a.resize(maxLen, 0);
    b.resize(maxLen, 0);
    vector<uint64_t> result(maxLen + 1, 0);
    __asm_add(a, b, result);
    return BigInt(result);
}

BigInt &BigInt::operator+=(BigInt &bigInt)
{
    *this = *this + bigInt;
    return *this;
}

BigInt BigInt::operator-(BigInt &bigInt)
{
    /*BigInt tmp = *this, result;
    if (*this < bigInt)
        swap(*this, bigInt);
    for (int i = 0; i < number.size(); ++i)
    {
        number[i] -= bigInt.number[i];
        if (number[i] < 0)
        {
            int j = i + 1;
            while (!number[j])
            {
                ++j;
            }
            while (j > i)
            {
                --number[j];
                number[--j] += 10;
            }
        }
    }
    check();
    result = *this;
    *this = tmp;*/
    vector<uint64_t> a = number, b = bigInt.number;
    int maxLen = max(a.size(), b.size());
    a.resize(maxLen, 0);
    b.resize(maxLen, 0);
    vector<uint64_t> result(maxLen, 0);
    uint64_t *pt_a_bits = &a[0],
             *pt_b_bits = &b[0],
             *pt_result = &result[0];
    asm volatile(
        "movq %0, %%r8;" //减数放在r8
        "movq %1, %%r9;" //被减数r9
        "movq %2, %%r10;"
        "movzx %3, %%rdx;"
        "movq $0, %%rcx;"
        "clc;"
        "lahf;"
        "1:"
        "sahf;" //从寄存器中读取flag
        "movq (%%r8, %%rcx, 8), %%rax;"
        "movq (%%r9, %%rcx, 8), %%rbx;"
        "sbb %%rax, %%rbx;"
        "lahf;" //保存flag,后面的inc可能会破坏carry
        "movq %%rbx, (%%r10, %%rcx, 8);"
        "inc %%rcx;"
        "cmp %%rcx, %%rdx;"
        "jne 1b;"
        :
        : "m"(pt_b_bits), "m"(pt_a_bits), "m"(pt_result), "m"(maxLen)
        : "memory", "cc", "r8", "r9", "r10", "rdx", "rcx", "rax", "rbx");
    return BigInt(result);
}

BigInt &BigInt::operator-=(BigInt bigInt)
{
    *this = *this - bigInt;
    return *this;
}
/*
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
*/