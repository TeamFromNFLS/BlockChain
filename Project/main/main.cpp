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
    /*BigInt a, b, c, d;
    cin >> a >> b >> c;
    //cin >> a;
    //c = a * b;
    d = BigInt::PowMod(a, b, c);
    cout << d << endl;
    // c = a - b;
    //cout << c << endl;
    //fclose(stdout);*/
    vector<uint64_t> a_bit, d_bit, n_bit;
    //    a_bit.push_back(0x3ad150b526d1243a);
    //    a_bit.push_back(0x66da4a2f);
    //
    //
    //    d_bit.push_back(0x60fba39bfb5f89a3);
    //    d_bit.push_back(0x2faf37b3b);
    //
    //    n_bit.push_back(0x0c03034c7d74bfe0);
    //    n_bit.push_back(0x0000000bebcdeced);
    a_bit.push_back(0x4f327b4b23ca1d83);
    a_bit.push_back(0x757611c9c6f11e1a);
    a_bit.push_back(0x19bc842d18cc06f4);
    a_bit.push_back(0x32);

    d_bit.push_back(0x397deba7fedbc1af);
    d_bit.push_back(0x48e5b8866f883681);
    d_bit.push_back(0x9e13df545ad88d7f);
    d_bit.push_back(0x40);

    n_bit.push_back(0x72fbd74ffdb7835f);
    n_bit.push_back(0x91cb710cdf106d02);
    n_bit.push_back(0x3c27bea8b5b11afe);
    n_bit.push_back(0x81);
    BigInt a(a_bit), d(d_bit), n(n_bit), ans;
    cout << a << endl
         << d << endl
         << n << endl;
    ans = BigInt::PowMod(a, d, n);
    cout << ans << endl;
    return 0;
}