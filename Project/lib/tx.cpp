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

void WalletCreator(Wallet *wallet);

/* Assume transactions are packed immediately after they are constructed.
Packed transactions are stored in Transaction::packedTx */
void TestTx()
{
    vector<Wallet *> wallets;
    vector<thread> threads;
    for (int i = 0; i < 1; ++i)
    {
        Wallet *tmp = new Wallet;
        wallets.push_back(tmp);
        threads.emplace_back(WalletCreator, tmp);
    }
    for (auto &t : threads)
    {
        if (t.joinable())
        {
            t.join();
        }
    }
    wallets[0]->CreateCoinbase();
    wallets[0]->CreateTransaction(Wallet::walletInfo[1], Transaction::mineReward);
}
