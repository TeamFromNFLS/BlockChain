#include "rsa.h"
#include "bigInt.h"

//Input1: an integer to be tested
//Input2: a parameter which determines the accuracy
bool RSA::IsPrime(const BigInt &num, const int k = 100)
{
    BigInt Zero("0"), One("1"), Two("2");
    if (num == Zero)
        return false;
    else if (num == Two)
        return true;
    else
    {
        BigInt numMinus = num - One, d, a;
        d = numMinus;
        int r = 0;
        //write num-1 as 2^r * d
        while (d % Two == Zero)
        {
            r++;
            d /= BigInt Two;
        }
        //test if there exists a number a such that a^d mod num != 1
        //and a^(2^j*d) mod num != -1
        for (int i = 0; i < k; i++)
        {
            a = CreateRandomNum(num); //create a random number between 1 and num
            //test a^d mod num
            BigInt reMod = BigInt::PowMod(a, d, num);
            if (reMod == One) //probably prime
                continue;

            //test a^(2^j*d) mod num for 0 <= j <= r-1
            for (int j = 0; j <= r - 1; j++)
            {
                if (reMod != numMinus) //composite
                    return false;
                reMod = BigInt::PowMod(reMod, 2, num);
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

    return BigInt Odd(ss.str());
}

BigInt RSA::CreatePrime()
{
    BigInt Prime = CreateOdd();
    while (!IsPrime(Odd))
    {
        Prime += BigInt Two("2"); //if not prime, then plus 2
    }
    return Prime;
}