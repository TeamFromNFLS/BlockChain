#include <iostream>
#include <cstdio>
#include <cstring>
#include <random>
#include <ctime>
#include <thread>
#include <mutex>
#include <vector>
#include "rsa.h"
#include "wallet.h"
#include "miner.h"
#include "sha256.h"
#include "ripemd160.h"
#include "block.h"
using namespace std;

void TestPowMod()
{
    vector<uint64_t> a_bit, d_bit, n_bit;
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
}