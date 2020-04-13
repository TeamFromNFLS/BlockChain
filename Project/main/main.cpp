#include <iostream>
#include <cstdio>
#include <cstring>
#include <random>
#include <ctime>
#include "rsa.h"
using namespace std;

void TestRandom();
int main()
{
     RSA a;
     auto st = clock();
     a.Init(1);
     a.CreateKeys();
     auto ed = clock();
     cout << "time:" << dec << ed - st << endl;
     return 0;
}
