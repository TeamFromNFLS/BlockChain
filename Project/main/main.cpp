#include <iostream>
#include <vector>
#include "bigInt.h"
#include "rsa.h"
using namespace std;

int main()
{
    //long i, cnt = generatePrime();
    /* for (i = 0; i < cnt; i++)
    {
        cout << prime_ls[i] << " ";
    } */
    //RSA rsa;
    //BigInt prime("2030783851"), product;
    //cout << rsa.IsPrime(prime) << endl;
    //rsa.Init();
    BigInt a("1000000000000000000000005");
    a = a - 2;
    cout << a << endl;
    /* while (cin >> prime)
    {
        bool isPrime = rsa.IsPrime(prime);
        cout << isPrime << endl;
        if (!prime)
            break;
    }  */
    /* BigInt a = 22, b = 7, n = 29, reMod;
    reMod = BigInt::PowMod(a, b, n);
    cout << reMod << endl;
    cout << 2048 % 23 << endl; */
    return 0;
}