#include <iostream>
#include <vector>
#include "bigInt.h"
#include "rsa.h"
using namespace std;
const long MAX = 100000;

/* long prime_ls[MAX + 1]; //store prime number
long generatePrime()
{
    long p[MAX + 1];
    long cnt = 0; //#prime number
    long i, j;
    for (i = 0; i <= MAX; i++)
    {
        p[i] = 1;
    }

    p[0] = p[1] = 0;
    for (i = 2; i <= MAX; i++)
    {
        if (p[i] == 1)
        {
            prime_ls[cnt++] = i;
            for (j = i; j <= MAX; j = j + i)
            {
                p[j] = 0;
            }
        }
    }
    return cnt;
}
 */
int main()
{
    //long i, cnt = generatePrime();
    /* for (i = 0; i < cnt; i++)
    {
        cout << prime_ls[i] << " ";
    } */
    RSA rsa;
    //BigInt prime("2030783851"), product;
    //cout << rsa.IsPrime(prime) << endl;
    rsa.Init();
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