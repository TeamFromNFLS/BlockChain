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

void MineWorker(vector<Miner *> *miners, int *worker, mutex *mutex, int *i, bool *foundFlag, int *checkCnt, Block *toCheck, int *output)
{
    Block check;
    int id, numberWorker;
    mutex->lock();
    id = *i;
    Miner *now = (*miners)[id];
    numberWorker = *worker;
    mutex->unlock();
    int nonce = now->GetNonce();
    now->Load(Transaction::toBePackedTx);
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
                mutex->lock();
                LOGOUT << "Miner " << id << " is working now." << endl;
                if (now->Check(check))
                {
                    checkFlag = true;
                    int tmp = *checkCnt;
                    tmp++;
                    *checkCnt = tmp;
                    mutex->unlock();
                }
                else
                {
                    *foundFlag = false;
                    mutex->unlock();
                }
            }
        }
        if (now->TestPoW(nonce))
        {
            mutex->lock();
            Block found = now->GetBlock();
            *foundFlag = true;
            *toCheck = found;
            *output = id;
            mutex->unlock();
        }
        nonce = (nonce + 1) % INT32_MAX;
    }
}

int Mine()
{
    mutex mutex;
    Block toCheck;
    bool foundFlag = false, outputFlag = false;
    int checkCnt = 0;
    int worker = Miner::minerSet.size();
    int output;
    vector<thread> threads(worker);
    for (int i = 0; i < worker; ++i)
    {
        threads.emplace_back(MineWorker, &Miner::minerSet, &worker, &mutex, &i, &foundFlag, &checkCnt, &toCheck, &output);
    }
    for (auto &t : threads)
    {
        if (t.joinable())
        {
            t.join();
        }
    }
    toCheck.Pack();
    toCheck.Show();
    return output;
}
