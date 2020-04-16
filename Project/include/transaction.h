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
    Transaction(TxInput _input, TxOutput _output); //regular tx
    //Transaction(TxInput _input, TxOutput _output, int _reward); //coinbase tx
    void SetID();
    int GetID();
    static void TxCoinbase(Wallet);
    bool IsCoinbase();
    void Sign(Wallet myWallet, const Transaction &preTx);
    void Clone();
    void findUTXO();
    int txID;
    TxInput input;
    TxOutput output;
    string senderAdr;
    string receiverAdr;
    static int mineReward;
    static int txCount;
    static vector<Transaction> txPool;
};

#endif