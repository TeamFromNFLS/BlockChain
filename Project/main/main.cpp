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

     a.Init(1);
     a.CreateKeys();

     return 0;
}
