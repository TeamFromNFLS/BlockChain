#include <iostream>
#include <cstdio>
#include <cstring>
#include <random>
#include <ctime>
#include <thread>
#include <mutex>
#include <vector>
#include "rsa.h"
#include "bigInt.h"
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
     auto st = clock();
     freopen("ans.txt", "w", stdout);
     balance();
     //TestTx();
     //TestMine();
     auto ed = clock();
     cout << dec << "Total Time: " << ed - st << endl;
     return 0;
}
