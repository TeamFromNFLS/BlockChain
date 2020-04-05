#include <iostream>
#include <vector>
#include "bigInt.h"
#include "rsa.h"
using namespace std;

int main()
{
<<<<<<< HEAD
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

    RSA rsa;
    BigInt prime;
    while (cin >> a)
    {
        prime = rsa.CreatePrime();
        cout << prime << endl;
        if (!a)
            break;
    }

=======
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
>>>>>>> dc796e2845c8307b6bfa3df8088404858f705003
    return 0;
}