#include <iostream>
#include <vector>
#include "bigInt.h"
#include "rsa.h"
using namespace std;

int main()
{
    //freopen("out.txt", "w", stdout);
    /* BigInt p("965020263285415169225506842409"), q("415924872698612520126277439911");
    RSA rsa;
    rsa.length = 30;
    rsa.setNumber(p, q);
    cout << rsa.product << endl;
    rsa.CreateKeys();
    cout << rsa.publicKey << endl
         << rsa.privateKey << endl;
    */
    BigInt a, b, c;
    cin >> a >> b;
    c = a - b;
    cout << c << endl;
    // c = a - b;
    //cout << c << endl;
    //fclose(stdout);
    return 0;
}