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
#include "chain.h"
using namespace std;

/* Assume transactions are packed immediately after they are constructed.
Packed transactions are stored in Transaction::packedTx */
int main()
{
     /*Wallet a(1), b(1);
     a.CreateCoinbase();
     a.CreateTransaction(Wallet::walletInfo[1], Transaction::mineReward);
     cout << Transaction::toBePackedTx.size() << endl;
     Block c(1, 2, Transaction::toBePackedTx);
     c.show();*/
     BigInt a("0x123456");
     cout << a << endl;
     return 0;
}
