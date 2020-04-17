#ifndef TRANSACTION_H
#define TRANSACTION_H
#include "wallet.h"
#include "bigInt.h"
#include "txInput.h"
#include "txOutput.h"
#include <string>
#include <vector>

class Transaction
{
public:
    Transaction(string _senderAdr, string _receiverAdr) : senderAdr(_senderAdr), receiverAdr(_receiverAdr) {}
    void SetID(int ID) { txID = ID; }
    bool IsCoinbase();
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