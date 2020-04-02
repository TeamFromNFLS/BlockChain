#include "rsa.h"
#include "bigInt.h"
#include <iostream>
#include <string>
#include <sstream>
#include <stdlib.h>
#include <ctime>
using namespace std;

void RSA::Init()
{
    //产生大素数p,q
    p = CreatePrime();
    q = CreatePrime();
    product = p * q;
    //欧拉数
    Euler = (p - 1) * (q - 1);
}

bool RSA::PrimePair(const BigInt &num1, const BigInt &num2)
{
    BigInt product = num1, key = num2, tmp; //candidates for key is generated by CreateRandomSmaller
    while (key != product.number.empty())
    {
        tmp = product % key;
        product = key;
        key = tmp;
    }
    return product == 1;
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
    switch (isOdd)
    {
    case 1: //create random odd
        for (int i = 0; i < length - 1; i++)
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
    while (!IsPrime(Prime))
    {
        Prime += 2; //if not prime, then plus 2
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
        if (Random && (Random - 1))
        {
            zeroOrOne = false;
        }
    }
    return Random;
}

void RSA::CreatePublic()
{
    BigInt _publicKey;
    do
    {
        _publicKey = CreateRandomSmaller(product);
    } while (!PrimePair(product, publicKey));
    publicKey = _publicKey;
}

void RSA::CreatePrivate()
{
    BigInt _privateKey;
    do
    {
        /* code */
    } while ((publicKey * _privateKey) % product != 1);
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
