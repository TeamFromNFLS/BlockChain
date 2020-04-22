#include <iostream>
#include <cstdio>
#include <cstring>
#include <random>
#include <ctime>
#include <windows.h>
#include <thread>
#include <mutex>
#include <vector>
#include "rsa.h"
#include "wallet.h"
#include "miner.h"
#include "sha256.h"
#include "ripemd160.h"
#include "block.h"
#include "chain.h"
using namespace std;

/* void TestPowMod();
void TestMine(); */
int main()
{
     //Wallet a(1), b(1);
     RSA rsa1, rsa2;
     rsa1.Init(1);
     rsa2.Init(1);

     /* a.CreateCoinbase();
     cout << get<0>(Wallet::walletInfo[1]) << endl
          << get<1>(Wallet::walletInfo[1]);
     a.CreateTransaction(Wallet::walletInfo[1], Transaction::mineReward); */

     return 0;
}
