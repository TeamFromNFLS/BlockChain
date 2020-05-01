#include <ctime>
#include <string>
#include "transaction.h"
#include "sha256.h"

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
        cout << "Transaction log: " << endl
             << "Type: Coinbase transaction" << endl
             << "Receiver Address: " << receiverAdr << endl
             << "Value: " << output.GetValue() << endl
             << "ID: " << GetID() << endl
             << "PrevTx ID: " << input.GetPrevID() << endl
             << "------------------------------------------" << endl;
    }
    else
    {
        cout << "Transaction log: " << endl
             << "Type: Normal transaction" << endl
             << "Sender Address: " << senderAdr << endl
             << "Receiver Address: " << receiverAdr << endl
             << "Value: " << output.GetValue() << endl
             << "ID: " << GetID() << endl
             << "PrevTx ID: " << input.GetPrevID() << endl
             << "Signature: " << input.signature << endl
             << "------------------------------------------" << endl;
    }
}