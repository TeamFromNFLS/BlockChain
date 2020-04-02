#include <iostream>
#include <vector>
#include "bigInt.h"
#include "rsa.h"
using namespace std;

int main()
{
    BigInt a = 10;
    BigInt c;
    c = a - 8;
    cout << c << endl;

    RSA rsa;
    BigInt prime("100001651");
    bool isPrime = rsa.IsPrime(prime);
    cout << isPrime << endl;
    return 0;
}