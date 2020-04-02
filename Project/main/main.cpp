#include <iostream>
#include <vector>
#include "bigInt.h"
#include "rsa.h"
using namespace std;

int main()
{

    RSA rsa;
    BigInt prime = rsa.CreatePrime();
    cout << prime << endl;
    return 0;
}