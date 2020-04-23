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
void TxTest();

int main()
{
     Miner miner(1);
     blockChain.SetDifficulty("0000");
     TxTest();
     miner.PoW(Transaction::toBePackedTx);
     return 0;
}
