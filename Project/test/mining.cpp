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
#include "Transaction.h"
using namespace std;

void MineWorker(int *worker, mutex *mutex, int *i, bool *foundFlag, int *checkCnt, Block *toCheck, int *output)
{
    Miner now;
    Block check;
    int nonce = now.GetNonce();
    now.Load(Transaction::toBePackedTx);
    int id, numberWorker;
    mutex->lock();
    id = *i;
    numberWorker = *worker;
    mutex->unlock();
    bool flag, cntFlag;
    bool checkFlag = false;
    while (true)
    {
        mutex->lock();
        flag = *foundFlag;
        int cnt = *checkCnt;
        mutex->unlock();
        if (cnt == numberWorker)
        {
            break;
        }
        if (flag)
        {
            mutex->lock();
            check = *toCheck;
            mutex->unlock();
            if (!checkFlag)
            {
                if (now.Check(check))
                {
                    checkFlag = true;
                    mutex->lock();
                    int tmp = *checkCnt;
                    tmp++;
                    *checkCnt = tmp;
                    mutex->unlock();
                }
                else
                {
                    mutex->lock();
                    *foundFlag = false;
                    mutex->unlock();
                }
            }
        }
        if (now.TestPoW(nonce))
        {
            mutex->lock();
            Block found = now.GetBlock();
            found.Pack();
            *foundFlag = true;
            *toCheck = found;
            *output = id;
            mutex->unlock();
        }
        nonce = (nonce + 1) % INT32_MAX;
        //cout << nonce << endl;
    }
}
void TestMine()
{
    mutex mutex;
    Block toCheck;
    bool foundFlag = false;
    int checkCnt = 0;
    int worker;
    cout << "Type in the number of workers：";
    cin >> worker;
    for (int i = 0; i < worker; ++i)
    {
        Miner tmp(1);
    }
    int output;
    vector<thread> threads(worker);
    for (int i = 0; i < worker; ++i)
    {
        threads.emplace_back(MineWorker, &worker, &mutex, &i, &foundFlag, &checkCnt, &toCheck, &output);
    }
    for (auto &t : threads)
    {
        if (t.joinable())
        {
            t.join();
        }
    }
    cout << "Found miner address: " << Miner::minerSet[output]->GetAddress() << endl;
    toCheck.Show();
}
