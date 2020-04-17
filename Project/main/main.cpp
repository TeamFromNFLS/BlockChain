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

void TestPowMod();
void TestMine();
int main()
{
     Block a(1), b(2), c(3);
     Chain chain;
     chain.AddBlock(a);
     chain.AddBlock(b);
     chain.AddBlock(c);
     chain.print();

     return 0;
}
