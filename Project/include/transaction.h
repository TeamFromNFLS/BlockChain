#ifndef TRANSACTION_H
#define TRANSACTION_H
#include "bigInt.h"
#include "txInput.h"
#include "txOutput.h"
#include <iostream>
#include <string>
#include <vector>
#include <ctime>
using namespace std;

class Transaction
{
public:
    Transaction(string _senderAdr, string _receiverAdr) : senderAdr(_senderAdr), receiverAdr(_receiverAdr)
    {
        _time = std::time(0);
        txID = ++cnt;
    }
    Transaction(string _receiverAdr) : senderAdr("null"), receiverAdr(_receiverAdr)
    {
        _time = std::time(0);
        txID = ++cnt;
    }
    void Show();
    void SetID();
    bool IsCoinbase();
    int GetID() const { return txID; }
    string GetTxHash();

    int txID;
    time_t _time;
    TxInput input;
    TxOutput output;
    string senderAdr;
    string receiverAdr;
    static int mineReward;
    static int cnt;
    static vector<Transaction> txPool;
    static vector<Transaction> toBePackedTx;
    static vector<Transaction> packedTx;
};

#endif