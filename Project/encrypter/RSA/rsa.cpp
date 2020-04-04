#include "rsa.h"
#include "bigInt.h"
#include <iostream>
#include <string>
#include <sstream>
#include <cstdlib>
#include <ctime>
#include <windows.h>
using namespace std;

void RSA::Init()
{
    //产生大素数p,q
    /*Since it might be confused when there are more than one operators in one calculation with bigInt, I just separate them apart*/
    auto st = clock();
    p = CreatePrime();
    Sleep(1000); // Wait to create another seed
    q = CreatePrime();
    product = p * q;
    BigInt one("1");
    p = p - one, q = q - one;
    //欧拉数
    Euler = p * q;
    p = p + one, q = q + one;
    auto ed = clock();
    cout << p << endl
         << q << endl
         << Euler << endl
         << "time:" << ed - st << endl;
}

bool RSA::IsPrime(const BigInt &num, int k)
{
    BigInt n = num;
    if (n == 1)
    {
        return false;
    }
    else if (n == 2)
    {
        return true;
    }
    else
    {
        BigInt numMinus, d, a, reMod;
        numMinus = n - 1;
        d = numMinus;
        BigInt tmp;
        tmp = d % 2;
        int r = 0;
        //write num-1 as 2^r * d
        while (tmp.number.empty())
        {
            r++;
            d = d / 2;
            //cout << d << endl;
            tmp = d % 2;
        }
        //test if there exists a number a such that a^d mod num != 1
        //and a^(2^j*d) mod num != -1

        for (int i = 0; i < k; i++)
        {
            a = CreateRandomSmaller(n); //create a random number between 1 and num
            //test a^d mod num
            reMod = BigInt::PowMod(a, d, n);
            if (reMod == 1) //probably prime
                continue;

            //test a^(2^j*d) mod num for 0 <= j <= r-1
            //composite \Leftrightarrow reMod 恒\neq numMinus
            bool maybeComposite = true;
            for (int j = 0; j <= r - 1; j++)
            {
                if (reMod == numMinus) //probably prime
                {
                    maybeComposite = false;
                    break; //enter the outer for loop
                }
                reMod = BigInt::PowMod(reMod, 2, n);
            }
            if (maybeComposite)
            {
                return false;
            }
        }
        return true; //pass all test
    }
}

BigInt RSA::CreateRandom(int isOdd)
{
    const int digit[] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9};
    stringstream ss;
    srand((unsigned)time(NULL)); //generate seed
    int tmp = rand() % 10;
    while (!tmp)
    {
        tmp = rand() % 10;
    }
    ss << digit[tmp];
    switch (isOdd)
    {
    case 1: //create random odd
        for (int i = 1; i < length - 1; i++)
        {
            ss << digit[rand() % 10];
        }
        ss << digit[2 * (rand() % 5) + 1];
        break;

    case 0: //create random
        for (int i = 0; i < length; i++)
        {
            ss << digit[rand() % 10];
        }
        break;
    }
    BigInt Random(ss.str());
    return Random;
}

BigInt RSA::CreatePrime()
{
    BigInt Prime = CreateRandom(1);
    bool flag = IsPrime(Prime);
    //BigInt Prime = 2030783801;
    while (!IsPrime(Prime))
    {
        Prime += 2;
    }
    return Prime;
}

BigInt RSA::CreateRandomSmaller(const BigInt &num)
{
    BigInt n = num, Random;
    const int digit[] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9};
    stringstream ss;
    bool zeroOrOne = true;

    while (zeroOrOne)
    {
        Random = CreateRandom(0);
        Random %= n;
        if (!(Random.number.empty()) && !((Random - 1).number.empty()))
        {
            zeroOrOne = false;
        }
    }
    return Random;
}

void RSA::CreateKeys()
{
    BigInt _publicKey, gcd;
    BigInt old_r, r, old_s, s, old_t, t, q, tmp;
    do
    {
        _publicKey = CreateRandomSmaller(Euler);
        old_r = _publicKey;
        r = Euler;
        old_s = 1;
        s = 0;
        old_t = 0;
        t = 1;
        while (!r.number.empty())
        {
            q = old_r / r;
            tmp = r;
            r = old_r % tmp;
            old_r = tmp;

            tmp = s;
            s = old_s - tmp * q;
            old_s = tmp;

            tmp = t;
            t = old_t - tmp * q;
            old_t = tmp;
        }
        gcd = old_r;
    } while (gcd != 1);
    publicKey = _publicKey;
    privateKey = (product + old_s % product) % product;
}

BigInt RSA::EncryptByPublic(const BigInt &num)
{
    BigInt B4encrypt = num;
    BigInt Encrypted = BigInt::PowMod(B4encrypt, publicKey, product);
    return Encrypted;
}

BigInt RSA::EncryptByPrivate(const BigInt &num)
{
    return DecryptByPrivate(num);
}

BigInt RSA::DecryptByPrivate(const BigInt &num)
{
    BigInt B4decrypt = num;
    BigInt Decrypted = BigInt::PowMod(B4decrypt, privateKey, product);
    return Decrypted;
}

BigInt RSA::DecryptByPublic(const BigInt &num)
{
    return EncryptByPublic(num);
}

void RSA::setNumber(BigInt a, BigInt b)
{
    p = a;
    q = b;
    product = p * q;
    Euler = (p - 1) * (q - 1);
}