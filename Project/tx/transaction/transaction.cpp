#include "txInput.h"
#include "txOutput.h"
#include "transaction.h"
#include "bigInt.h"
#include <string>
#include <iostream>
using namespace std;

int Transaction::txCount = 0;
int Transaction::mineReward = 5; //to be set

Transaction::Transaction(TxInput _input, TxOutput _output) : input(_input), output(_output)
{
    txID = input.GetPrevID() + 1;
    txCount++;
}

/* Transaction::Transaction(TxInput _input, TxOutput _output, int _reward) : input(_input), output(_output), mineReward(_reward)
{
    txID = input.GetPrevID() + 1;
    txCount++;
} */

int Transaction::GetID()
{
    return txID;
}

void Transaction::TxCoinbase(Wallet myWallet)
{
    TxInput input;
    TxOutput output(mineReward, myWallet.GetPub());
    Transaction(input, output);
}

bool Transaction::IsCoinbase()
{
    return input.GetPrevID() == -1 && input.GetValue() == 0;
}

void Transaction::Sign(Wallet myWallet, const Transaction &preTx)
{
    //TODO:examine if the transaction is coinbase
    if (!IsCoinbase())
    {
        RSA rsa;
        Transaction tx = preTx;
        string receiverPublicHash = output.getPublicHash();
        string senderPublicHash = tx.output.getPublicHash(); //get sender's publicHash from the output of the previous Tx
        string signInfo = senderPublicHash + receiverPublicHash + to_string(output.value);
        BigInt signature = rsa.EncryptByPrivate(signInfo, myWallet.GetPri(), myWallet.GetN());
        input.setSignature(signature);
    }
}

void Transaction::findUTXO()
{
    vector<Transaction> spentTx;
    for
}
