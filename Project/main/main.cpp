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

void test();
int main()
{
     mutex mutex;
     int worker = 4;
     vector<thread> threads(worker);
     for (int i = 0; i < worker; ++i)
     {
          threads.emplace_back(test);
     }
     for (auto &t : threads)
     {
          if (t.joinable())
          {
               t.join();
          }
     }
     return 0;
}
