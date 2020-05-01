#include <ctime>
#include <string>
#include "transaction.h"
#include "sha256.h"
#include "log.h"

int Transaction::mineReward = 5; //to be set
int Transaction::cnt = -1;
vector<Transaction> Transaction::txPool;
vector<Transaction> Transaction::toBePackedTx;
vector<Transaction> Transaction::packedTx;

void Transaction::SetID()
{
    txID = input.GetPrevID() + 1;
}

string Transaction::GetTxHash()
{
    string time = ctime(&_time);
    string hashInfo = time + senderAdr + receiverAdr + to_string(input.GetValue());
    string txHash = sha256(hashInfo);
    return txHash;
}

bool Transaction::IsCoinbase()
{
    if (input.GetPrevID() == -1 && input.GetValue() == 0)
    {
        return true;
    }
}

void Transaction::Show()
{
    if (senderAdr == "null")
    {
        LOGOUT << "Transaction log: " << endl;
        LOGOUT << "Type: Coinbase transaction" << endl;
        LOGOUT << "Receiver Address: " << receiverAdr << endl;
        LOGOUT << "Value: " << output.GetValue() << endl;
        LOGOUT << "ID: " << GetID() << endl;
        LOGOUT << "PrevTx ID: " << input.GetPrevID() << endl
               << "------------------------------------------" << endl;
    }
    else
    {
        LOGOUT << "Transaction log: " << endl;
        LOGOUT << "Type: Normal transaction" << endl;
        LOGOUT << "Sender Address: " << senderAdr << endl;
        LOGOUT << "Receiver Address: " << receiverAdr << endl;
        LOGOUT << "Value: " << output.GetValue() << endl;
        LOGOUT << "ID: " << GetID() << endl;
        LOGOUT << "PrevTx ID: " << input.GetPrevID() << endl;
        LOGOUT << "Signature: " << input.signature << endl
               << "------------------------------------------" << endl;
    }
}