#include "bigInt.h"
#include <iostream>
#include <sstream>
#include <iomanip>
#include <vector>
#include <algorithm>
#include <cmath>
#include <cstring>
#include <utility>
#include <assert.h>
/*store carry flag into r15*/
#define ASM_LOAD "lahf; shr $8, %%ax; mov %%al, %%r15b;"
#define ASM_RESTORE "mov %%r15b, %%al; sal $8, %%ax; sahf;"

using namespace std;

/*define some const bigInt*/
const BigInt BigInt::zero("0");
const BigInt BigInt::one("1");
const BigInt BigInt::two("2");

/*calculate the bit length of a number*/
int BigInt::Shrink(vector<uint64_t> &vec)
{
    int cntBits = 64 * vec.size();
    /*delete zeros in front of the number*/
    while (cntBits > 0)
    {
        // now block, wait to be checked
        uint64_t now = vec[(cntBits - 1) / 64];
        // << to check whether this bit is zero
        uint64_t comp = static_cast<uint64_t>(0x1) << static_cast<uint64_t>((cntBits - 1) % 64);
        if (now & comp) // the first one in the bit
        {
            break;
        }
        cntBits--;
        /*remove zero block in the end*/
        if (!(cntBits % 64))
        {
            vec.pop_back();
        }
    }
    cntBits = !cntBits ? 1 : cntBits; // if number is zero then the bit is 1
    return cntBits;
}

/*update*/
void BigInt::check()
{
    while (!number.empty() && !number.back())
        number.pop_back();
    bit = Shrink(number);
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

void BigInt::SetBin(string s)
{
    int len = s.length(), cnt = 0, pos = -1;
    BigInt::bit = len;
    string::iterator it = s.end() - 1;
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
            number[pos] += UINT64_C(1) << cnt;
        }
        len--;
        it--;
        cnt = (cnt + 1) % 64;
    }
    check();
    return;
}

/*I don't think there's anyone who wants to input a number with hex system*/
void BigInt::SetNumber(string s)
{
    string::iterator it;
    bool inputDec = false; // whether input is in dec system
    for (it = s.begin(); it != s.end(); ++it)
    {
        if (*it != '0' || *it != '1')
        {
            inputDec = true;
            break;
        }
    }
    if (inputDec)
    {
        string ss = DecToBin(s);
        SetBin(ss);
    }
    else
    {
        SetBin(s);
    }
    return;
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

string BigInt::ToString()
{
    stringstream ss;
    for (int i = number.size() - 1; i >= 0; --i)
    {
        ss << hex << number[i];
    }
    string result = ss.str();
    return result;
}

BigInt BigInt::ToInt(string s)
{
    vector<uint64_t> result;
    int len = s.length();
    uint64_t cnt = 0;
    for (int i = len - 1; i >= 0; --i)
    {
        if (!cnt)
        {
            result.push_back(0);
        }
        uint64_t now;
        if (s[i] >= 'a' && s[i] <= 'z')
        {
            now = s[i] - 'a' + 10;
        }
        else
        {
            now = s[i] - '0';
        }
        result.back() += now * (uint64_t)pow(UINT64_C(16), cnt);
        cnt = (cnt + 1) % 16;
    }
    return BigInt(result);
}

istream &operator>>(istream &input, BigInt &bigInt)
{
    string s;
    input >> s;
    bigInt.number.clear();
    bigInt.SetNumber(s);
    return input;
}

ostream &operator<<(ostream &output, BigInt &bigInt)
{
    //freopen("out.txt", "w", stdout);
    output << "0x";
    for (int i = bigInt.number.size() - 1; i >= 0; i--)
    {
        output << setw(16) << setfill('0') << hex << bigInt.number[i];
    }
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
    const BigInt tmp = *this;
    return bigInt < tmp;
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
    uint64_t *ptA = &a[0],
             *ptB = &b[0],
             *ptC = &c[0];
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
        : "m"(ptA), "m"(ptB), "m"(ptC), "m"(len)
        : "memory", "cc", "r8", "r9", "r10", "rdx", "rcx", "rax", "rbx");
}

BigInt BigInt::operator+(const BigInt &bigInt)
{
    vector<uint64_t> a = number, b = bigInt.number;
    int maxLen = max(a.size(), b.size());
    a.resize(maxLen, 0);
    b.resize(maxLen, 0);
    vector<uint64_t> result(maxLen + 1, 0);
    __asm_add(a, b, result);
    return BigInt(result);
}

BigInt &BigInt::operator+=(const BigInt &bigInt)
{
    *this = *this + bigInt;
    return *this;
}

inline void __asm_sub(vector<uint64_t> &a, vector<uint64_t> &b, vector<uint64_t> &c)
{
    uint64_t *ptA = &a[0],
             *ptB = &b[0],
             *ptC = &c[0];
    uint64_t len = a.size();
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
        : "m"(ptB), "m"(ptA), "m"(ptC), "m"(len)
        : "memory", "cc", "r8", "r9", "r10", "rdx", "rcx", "rax", "rbx");
}

BigInt BigInt::operator-(const BigInt &bigInt)
{
    vector<uint64_t> a = number, b = bigInt.number;
    if (a.empty())
    {
        a.push_back(0);
    }
    int maxLen = max(a.size(), b.size());
    a.resize(maxLen, 0);
    b.resize(maxLen, 0);
    vector<uint64_t> result(maxLen, 0);
    __asm_sub(a, b, result);
    return BigInt(result);
}

BigInt &BigInt::operator-=(const BigInt &bigInt)
{
    *this = *this - bigInt;
    return *this;
}

BigInt BigInt::operator*(const BigInt &bigInt)
{
    vector<uint64_t> a = number, b = bigInt.number;
    if (a.empty())
    {
        a.push_back(0);
    }
    vector<uint64_t> result(a.size() + b.size(), 0);
    vector<uint64_t> tmp(a.size() + b.size(), 0); // store temp for each multi
    uint64_t *ptA = &a[0],
             *ptB = &b[0],
             *ptResult = &result[0],
             *ptTmp = &tmp[0];
    uint64_t lenA = a.size(), tenTmp = tmp.size();
    for (int i = 0; i < b.size(); ++i)
    {
        uint64_t *ptTmpI = ptTmp + i, *ptResultI = ptResult + i;
        asm volatile(
            "movq $0, %%rdx;"
            "movq $0, %%r14;" //r14保存上一次乘法的溢出结果
            "movq $0, %%rcx;" //rcx用于计数
            "movq %1, %%rsi;" //rsi保存大数的首地址
            "movq %3, %%rdi;" //rdi保存中间结果（就是tmp）的首地址
            "movq %0, %%r8;"  //r8保存小乘数,维持不变(寄存器访问速度更快)
            "clc;" ASM_LOAD   //carry保存在r15中
            "1:"
            "mov %%r8, %%rax;"
            "mov (%%rsi, %%rcx, 8), %%rbx;"
            "mul %%rbx;" //乘法完成,结果保留在rdx:rax中
            "mov %%rax, %%rbx;" ASM_RESTORE
            "adc %%r14, %%rbx;" //连带carry外加上一次的乘法溢出一起加到本次结果上
            ASM_LOAD
            "mov %%rdx, %%r14;"             //保存本次溢出
            "mov %%rbx, (%%rdi, %%rcx, 8);" //本次计算结果保存到tmp数组
            "inc %%rcx;"
            "cmp %%rcx, %2;" //是否已经执行了length_a次
            "jne 1b;" ASM_RESTORE
            "mov $0, %%rax;"
            "adc %%r14, %%rax;"
            "mov %%rax, (%%rdi, %%rcx, 8);"
            :
            : "m"(ptB[i]), "m"(ptA), "m"(lenA), "m"(ptTmpI)
            : "memory", "cc", "rax", "rbx", "rcx", "rdx", "rsi", "rdi", "r8", "r14", "r15");
        //tmp更后面的结果赋值为0,避免每次对tmp重新赋值
        fill(tmp.begin(), tmp.begin() + i, 0);
        //这段汇编完成的是将tmp的值加到result上面,注意两个向量长度相等
        uint64_t la1 = lenA + 1;
        asm volatile(
            "movq %0, %%r8;"
            "movq %1, %%r9;"
            "mov %2, %%rdx;"
            "movq $0, %%rcx;"
            "clc;"
            "lahf;"
            "1:"
            "sahf;"
            "movq (%%r8, %%rcx, 8), %%rax;"
            "adc %%rax, (%%r9, %%rcx, 8);" //将r8指向的数加到r9上
            "lahf;"
            "inc %%rcx;"
            "cmp %%rcx, %%rdx;"
            "jne 1b;"
            "sahf;"
            "adc $0, (%%r9, %%rcx, 8);"
            :
            : "m"(ptTmpI), "m"(ptResultI), "m"(la1)
            : "memory", "cc", "r8", "r9", "rdx", "rcx", "rax", "rbx");
    }
    return BigInt(result);
}

BigInt &BigInt::operator*=(const BigInt &bigInt)
{
    *this = *this * bigInt;
    return *this;
}

inline int GetBitAt(const vector<uint64_t> &v, int m)
{
    return (v[m / 64] >> static_cast<uint64_t>(m % 64)) & UINT64_C(1);
}

//借助汇编实现逻辑右移
inline uint64_t __asm_shr(uint64_t a, int s)
{
    uint64_t r;
    asm volatile(
        "mov %1, %0;"
        "shr %%cl, %0;"
        : "=r"(r)
        : "r"(a), "c"(s)
        :);
    return s == 64 ? 0 : r;
}

inline void __asm_sub_from(vector<uint64_t> &a, vector<uint64_t> &b)
{
    //默认a和b一样长,将a-b的结果保存在a中
    uint64_t *ptA = &a[0],
             *ptB = &b[0],
             len = a.size();
    asm volatile(
        "movq %0, %%r8;" //减数放在r8
        "movq %1, %%r9;" //被减数r9
        "mov %2, %%rdx;" //保存最大计数
        "movq $0, %%rcx;"
        "clc;"
        "lahf;"
        "1:"
        "sahf;" //从寄存器中读取flag
        "movq (%%r8, %%rcx, 8), %%rax;"
        "sbb %%rax, (%%r9, %%rcx, 8);"
        "lahf;" //保存flag,后面的inc可能会破坏carry
        "inc %%rcx;"
        "cmp %%rcx, %%rdx;"
        "jne 1b;"
        :
        : "m"(ptB), "m"(ptA), "m"(len)
        : "memory", "cc", "r8", "r9", "rdx", "rcx", "rax", "rbx");
}

/*return a pair of bigInt, first is quotient, second is remainder*/
pair<BigInt, BigInt> BigInt::DivMod(const BigInt &_a, const BigInt &_b)
{
    vector<uint64_t> a, b;
    _a.GetNumber(a), _b.GetNumber(b);
    int l1 = _a.GetBit(), l2 = _b.GetBit();
    vector<uint64_t> q(max((l1 - l2) / 64, 0) + 1, 0), r(l2, 0);
    vector<uint64_t> tmp(a.size(), 0); //保存中间移位得到的结果
    while (l1 >= l2)
    {
        int m = l2 - 1;
        int posQ; //首先确定本轮除法商的位置
        while (m >= 0 && GetBitAt(b, m) == GetBitAt(a, l1 - l2 + m))
            --m;
        if (m < 0 || GetBitAt(b, m) < GetBitAt(a, l1 - l2 + m))
            posQ = l1 - l2;
        else if (l1 > l2) //此时已经有m >= 0 && GetBitAt(b_bits, m) > GetBitAt(a_bits, l1 - l2 + m)
            posQ = l1 - l2 - 1;
        else
            break;
        int displaceEntry = posQ / 64, displaceBit = posQ % 64;
        q[displaceEntry] |= static_cast<uint64_t>(1) << static_cast<uint64_t>(displaceBit); //设置商
        //计算移位,这里合并了两个循环可以加快减少数据依赖
        tmp[displaceEntry] = b[0] << static_cast<uint64_t>(displaceBit);
        for (int i = 1; i < b.size(); ++i)
        {
            tmp[i + displaceEntry] = b[i] << static_cast<uint64_t>(displaceBit);
            tmp[i + displaceEntry] |= __asm_shr(b[i - 1], 64 - displaceBit);
        }
        tmp[b.size() + displaceEntry] |= __asm_shr(b.back(), 64 - displaceBit);
        //将tmp从a_bits中减去
        __asm_sub_from(a, tmp);
        //更新l1的值,缩小a
        l1 = BigInt::Shrink(a);
        tmp.resize(a.size());
        fill(tmp.begin(), tmp.end(), 0);
    }
    r = a;
    return make_pair(BigInt(q), BigInt(r));
}

BigInt BigInt::operator/(const BigInt &bigInt)
{
    BigInt result;
    result = result.DivMod(number, bigInt.number).first;
    return result;
}

BigInt &BigInt::operator/=(const BigInt &bigInt)
{
    *this = *this / bigInt;
    return *this;
}

BigInt BigInt::operator%(const BigInt &bigInt)
{
    BigInt result;
    result = result.DivMod(number, bigInt.number).second;
    return result;
}

BigInt &BigInt::operator%=(const BigInt &bigInt)
{
    *this = *this % bigInt;
    return *this;
}

BigInt BigInt::PowMod(BigInt &a, BigInt &e, BigInt &mod)
{
    BigInt aMod(zero);
    if (a == mod)
    {
        return zero;
    }
    else
    {
        if (a > mod)
        {
            aMod = a % mod;
        }
        else
        {
            aMod = a;
        }
    }
    if (!mod.InverseComputed)
    {
        mod.ComputeInverse();
    }
    BigInt inverse(mod.numberInverse), basis = aMod, result = one;
    int eLen = e.GetBit();
    vector<uint64_t> eBits;
    e.GetNumber(eBits);
    for (int i = 0; i < eLen; ++i)
    {
        if (GetBitAt(eBits, i))
        {
            BigInt tmp = basis * result;
            result = mod.GetResidualWithInverse(tmp, inverse);
        }
        BigInt tmp = basis * basis;
        basis = mod.GetResidualWithInverse(tmp, inverse);
    }
    return result;
}

BigInt BigInt::GetResidualWithInverse(BigInt &a, BigInt &inv) const
{
    //a <= this^2
    BigInt res = (a * inv).RightShift(2 * bit) * *this;
    //assert(a >= res);
    res = a - res;
    while (res >= *this)
    {
        res -= *this;
    }
    return res;
}

BigInt BigInt::Reverse(int length) const
{
    vector<uint64_t> reversed((length - 1) / 64 + 1, 0);
    int minLength = min(length, bit);
    for (int i = 0; i < minLength; ++i)
    {
        reversed[(length - i - 1) / 64] |= static_cast<uint64_t>(GetBitAt(number, i)) << static_cast<uint64_t>((length - i - 1) % 64);
    }
    return BigInt(reversed);
}

BigInt BigInt::RightShift(int length) const
{
    if (bit <= length)
        return zero;
    int entryDiscarded = length / 64, bitDiscarded = length % 64;
    vector<uint64_t> v(number.size(), 0);
    int i = 0;
    for (; i < number.size() - entryDiscarded - 1; ++i)
    {
        uint64_t a = __asm_shr(number[i + entryDiscarded], bitDiscarded);
        uint64_t b = !bitDiscarded ? 0 : number[i + entryDiscarded + 1] << static_cast<uint64_t>(64 - bitDiscarded);
        v[i] = a | b;
    }
    v[i] = __asm_shr(number[i + entryDiscarded], bitDiscarded);
    return BigInt(v);
}

void BigInt::ComputeInverse()
{
    vector<uint64_t> xInitBit(number.size() + 1, 0);
    //初始值设置成2^{-p},p = m_cnt_bits;
    xInitBit[bit / 64] |= UINT64_C(1) << static_cast<uint64_t>(bit % 64);
    BigInt x(xInitBit);
    vector<uint64_t> oneBit(2 * bit / 64 + 1, 0); //one_with_decimal一共2p+1位, LSB是2^{-2p}, 从而MSB表示1
    oneBit.back() |= UINT64_C(1) << static_cast<uint64_t>((2 * bit) % 64);
    BigInt oneWithDecimal(oneBit);

    while (true)
    {
        BigInt tmp = x * *this;
        assert(oneWithDecimal >= tmp);
        BigInt delta = oneWithDecimal - tmp;
        //当x与(1 - nx)相乘的时候,需要把LSB变成1才能使用通常意义上的乘法,乘完之后再次反向也就完成了舍入
        BigInt deltaX = (x.Reverse(2 * bit + 1) * delta.Reverse(2 * bit + 1)).Reverse(2 * bit + 1);
        if (deltaX.number.empty())
        {
            break;
        }
        x += deltaX;
    }
    x.GetNumber(numberInverse);
    InverseComputed = true;
}
