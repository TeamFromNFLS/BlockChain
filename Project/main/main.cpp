#include <iostream>
#include <cstdio>
#include <cstring>
#include <random>
#include <ctime>
#include "rsa.h"
#include "wallet.h"
#include "sha256.h"
#include "ripemd160.h"
using namespace std;

void TestRandom();
int main()
{
     RSA a;

     a.Init(1);
     a.CreateKeys();

     Wallet b(1);
     return 0;
}
