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
     vector<int> a;
     vector<int> b(8, 0);
     a.insert(a.end(), b.begin(), b.end());
     cout << a.size() << endl;

     return 0;
}
