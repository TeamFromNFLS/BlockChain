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
#include "test.h"
using namespace std;

/* Assume transactions are packed immediately after they are constructed.
Packed transactions are stored in Transaction::packedTx */

int main()
{
     //freopen("ans.txt", "w", stdout);
     blockChain.SetDifficulty("0000");
     //TestTx();
     Transaction tmp("123", "456");
     Transaction::toBePackedTx.push_back(tmp);
     TestMine();
     return 0;
}
