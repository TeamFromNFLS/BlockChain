#ifndef WALLET_H
#define WALLET_H
#include <string>
#include <tuple>
#include "bigInt.h"
#include "transaction.h"
#include "txInput.h"
#include "txOutput.h"

using namespace std;
class Wallet
{
public:
    Wallet(int worker);
    Wallet(){};
    void Init(int worker);
    tuple<BigInt, BigInt, string> GetKey()
    {
        return make_tuple(publicKey, n, address);
    }
    string GetAddress()
    {
        return address;
    }
    static vector<pair<string, string>> walletInfo;

protected:
    BigInt privateKey, publicKey, n;
    string publicKeyHash;
    string address;
    string version = "00";

    /* create transaction */
public:
    void CreateTransaction(pair<string, string> receiverInfo, int _value);
    void CreateCoinbase();
    void Sign(Transaction &tx, string receiverPublicKeyHash, int _value);
    vector<int> FindSpent();
    vector<Transaction> FindUTXO(vector<int> spentTxId);
};

string Base58(string s);

#endif //WALLET_H