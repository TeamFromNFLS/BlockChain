#ifndef WALLET_H
#define WALLET_H
#include <string>
#include <tuple>
#include "chain.h"
#include "bigInt.h"
#include "transaction.h"
#include "txInput.h"
#include "txOutput.h"

using namespace std;
class Wallet
{
public:
    Wallet(int worker) { Init(worker); }
    Wallet(){};
    virtual void Init(int worker);
    tuple<BigInt, BigInt, string> GetKey()
    {
        return make_tuple(publicKey, n, address);
    }
    string GetAddress()
    {
        return address;
    }
    static vector<pair<string, string>> walletInfo;
    void SetWallet(BigInt _publicKey, BigInt _privateKey, BigInt _N, string _addr);
    void CheckChain();

protected:
    BigInt privateKey, publicKey, n;
    string publicKeyHash;
    string address;
    string version = "00";
    Chain chain;

    /* create transaction */
public:
    void CreateTransaction(pair<string, string> receiverInfo, int _value);
    void CreateCoinbase();
    void Sign(Transaction &tx, string receiverPublicKeyHash, int _value);
    vector<int> FindSpent(vector<Transaction> &pool);
    vector<Transaction> FindUTXO(vector<int> &spentTxId, vector<Transaction> &pool);
    void FindBalance();
    static bool VerifyTx(const Transaction &_tx);
};

string Base58(string s);

#endif //WALLET_H
