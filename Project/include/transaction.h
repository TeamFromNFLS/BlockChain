#ifndef TRANSACTION_H
#define TRANSACTION_H
#include "bigInt.h"
#include "txInput.h"
#include "txOutput.h"
#include <iostream>
#include <string>
#include <vector>

using namespace std;
class Transaction
{
public:
    Transaction(string _senderAdr, string _receiverAdr) : senderAdr(_senderAdr), receiverAdr(_receiverAdr){};
    void SetID(int ID) { txID = ID; }
    bool IsCoinbase() { return true; }
    int GetID() { return txID; }

    int txID;
    TxInput input;
    TxOutput output;
    string senderAdr;
    string receiverAdr;
    static int mineReward;
    static vector<Transaction> txPool;
    static vector<Transaction> toBePackedTx;
};

#endif