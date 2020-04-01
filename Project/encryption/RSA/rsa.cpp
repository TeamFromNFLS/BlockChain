#include "rsa.h"
#include "bigInt.h"
#include <iostream>
#include <string>
#include <sstream>
#include <stdlib.h>
#include <ctime>
using namespace std;

bool RSA::IsPrime(const BigInt &num, const int k = 100)
{
    BigInt Zero("0"), One("1"), Two("2");
    BigInt n = num;
    if (n == Zero)
        return false;
    else if (n == Two)
        return true;
    else
    {
        BigInt numMinus = n - One, d, a;
        d = numMinus;
        int r = 0;
        //write num-1 as 2^r * d
        while (d % Two == Zero)
        {
            r++;
            d /= Two;
        }
        //test if there exists a number a such that a^d mod num != 1
        //and a^(2^j*d) mod num != -1
        for (int i = 0; i < k; i++)
        {
            a = CreateRandomNum(n); //create a random number between 1 and num
            //test a^d mod num
            BigInt reMod = BigInt::PowMod(a, d, n);
            if (reMod == One) //probably prime
                continue;

            //test a^(2^j*d) mod num for 0 <= j <= r-1
            for (int j = 0; j <= r - 1; j++)
            {
                if (reMod != numMinus) //composite
                    return false;
                reMod = BigInt::PowMod(reMod, Two, n);
            }
        }
        return true;
    }
}

BigInt RSA::CreateOdd()
{
    const int digit[] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9};
    stringstream ss;

    srand((unsigned)time(NULL)); //generate seed
    for (int i = 0; i < length - 1; i++)
    {
        ss << digit[rand() % 10];
    }
    ss << digit[2 * (rand() % 5) + 1];
    BigInt Odd(ss.str());
    return Odd;
}

BigInt RSA::CreatePrime()
{
    BigInt Prime = CreateOdd(), Two("2");
    while (!IsPrime(Prime))
    {
        Prime += Two; //if not prime, then plus 2
    }
    return Prime;
}