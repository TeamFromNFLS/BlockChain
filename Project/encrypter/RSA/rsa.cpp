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
    Product = p * q;
    //欧拉数
    Euler = (p - 1) * (q - 1);
}

bool RSA::IsPrime(const BigInt &num, int k)
{
    BigInt n = num;
    if (n == n.number.empty())
        return false;
    else if (n == 2)
        return true;
    else
    {
        BigInt numMinus = n - 1, d, a;
        d = numMinus;
        int r = 0;
        //write num-1 as 2^r * d
        while (d % 2 == n.number.empty())
        {
            r++;
            d /= 2;
        }
        //test if there exists a number a such that a^d mod num != 1
        //and a^(2^j*d) mod num != -1
        for (int i = 0; i < k; i++)
        {
            a = CreateRandomSmaller(n); //create a random number between 1 and num
            //test a^d mod num
            BigInt reMod = BigInt::PowMod(a, d, n);
            if (reMod == 1) //probably prime
                continue;

            //test a^(2^j*d) mod num for 0 <= j <= r-1
            for (int j = 0; j <= r - 1; j++)
            {
                if (reMod != numMinus) //composite
                    return false;
                reMod = BigInt::PowMod(reMod, 2, n);
            }
        }
        return true;
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
    bool zero = true;

    while (zero)
    {
        Random = CreateRandom(0);
        Random %= n;
        if (Random)
        {
            zero = false;
        }
    }
    return Random;
}

BigInt RSA::EncryptByPublic(const BigInt &num)
{
    BigInt B4encrypt = num;
    BigInt Encrypted = BigInt::PowMod(B4encrypt, PublicKey, Product);
    return Encrypted;
}

BigInt RSA::EncryptByPrivate(const BigInt &num)
{
    return DecryptByPrivate(num);
}

BigInt RSA::DecryptByPrivate(const BigInt &num)
{
    BigInt B4decrypt = num;
    BigInt Decrypted = BigInt::PowMod(B4decrypt, PrivateKey, Product);
    return Decrypted;
}

BigInt RSA::DecryptByPublic(const BigInt &num)
{
    return EncryptByPublic(num);
}
