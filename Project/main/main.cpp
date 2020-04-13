#include <iostream>
#include <vector>
#include "bigInt.h"
#include "rsa.h"
#include <ctime>
#include <cstdlib>
using namespace std;
void TestRandom();
int main()
{
     RSA a;
     BigInt p = 17, q = 19;
     a.setNumber(p, q);
     a.CreateKeys();
     cout << a.Euler << endl;
     cout << a.publicKey << " " << a.privateKey << endl;
     return 0;
}