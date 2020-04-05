#include <iostream>
#include <vector>
#include "bigInt.h"
#include "rsa.h"
using namespace std;

int main()
{
    /* BigInt p("965020263285415169225506842409"), q("415924872698612520126277439911");
    RSA rsa;
    rsa.length = 30;
    rsa.setNumber(p, q);
    cout << rsa.product << endl;
    rsa.CreateKeys();
    cout << rsa.publicKey << endl
         << rsa.privateKey << endl;
    */
    int a;

    BigInt prime;
    while (cin >> a)
    {
        RSA rsa;
        rsa.length = a;
        prime = rsa.CreatePrime();
        cout << prime << endl;
        if (!a)
            break;
    }

    return 0;
}