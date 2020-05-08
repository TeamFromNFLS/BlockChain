#include <iostream>
#include <cstdio>
#include <cstring>
#include <random>
#include <ctime>
#include <thread>
#include <mutex>
#include <vector>
#include <utility>
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
void WalletCreator(Wallet *wallet)
{
    wallet->Init(1);
}

int balance()
{
    vector<Wallet *> wallets;
    vector<thread> threads;
    Wallet *tmp;
    for (int i = 0; i < 3; ++i)
    {
        tmp = new Wallet;
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
    vector<Wallet *> newWallets;
    for (int i = 0; i < 3; ++i)
    {
        for (int j = 0; j < 3; ++j)
        {
            if (Wallet::walletInfo[i].first == wallets[j]->GetAddress())
            {
                newWallets.push_back(wallets[j]);
            }
        }
    }
    Miner tmpMiner(1);
    newWallets[1]->CreateCoinbase(5);
    tmpMiner.PoW();
    newWallets[2]->CreateCoinbase(5);
    tmpMiner.Reset();
    tmpMiner.PoW();
    newWallets[1]->CreateTransaction(Wallet::walletInfo[0], Transaction::mineReward);
    newWallets[2]->CreateTransaction(Wallet::walletInfo[0], Transaction::mineReward);
    newWallets[0]->CreateCoinbase(5);
    tmpMiner.Reset();
    tmpMiner.PoW();
    newWallets[0]->FindBalance();
    return 0;
}
