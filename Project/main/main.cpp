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

void TestPowMod();
void TestMine();
int main()
{
     RSA a;

     a.Init(1);
     a.CreateKeys();

     Wallet b(1);
     return 0;
}
