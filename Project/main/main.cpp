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
     Wallet a(1), b(1);
     a.CreateCoinbase();
     a.CreateTransaction(Wallet::walletInfo[1], Transaction::mineReward);
     /* RSA a;
     BigInt p, q;
     while (cin >> p >> q)
     {
          a.setNumber(p, q);
          a.CreateKeys();
          tuple<BigInt, BigInt, BigInt> key = a.GetKey();
          cout << get<0>(key) << " " << get<1>(key) << " " << get<2>(key) << endl;
          BigInt encrypt = a.EncryptByPrivate(123, get<1>(key), get<2>(key));
          BigInt decrypt = a.DecryptByPublic(encrypt, get<0>(key), get<2>(key));
          cout << encrypt << endl
               << decrypt;
     } */

     return 0;
}
