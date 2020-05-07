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
    virtual void InitPrint();
    tuple<BigInt, BigInt, string> GetKey()
    {
        return make_tuple(publicKey, n, address);
    }
    string GetAddress()
    {
        return address;
    }
    static vector<Wallet> walletSet;
    static vector<Wallet> onlyWalletSet;
    static vector<pair<string, string>> walletInfo;
    void SetWallet(BigInt _publicKey, BigInt _privateKey, BigInt _N, string _addr);
    bool CheckChain();

protected:
    BigInt privateKey, publicKey, n;
    string publicKeyHash;
    string address;
    string version = "00";
    Chain chain;

    /* create transaction */
public:
    bool CreateTransaction(pair<string, string> receiverInfo, int _value);
    void CreateCoinbase(int x);
    void Sign(Transaction &tx, string receiverPublicKeyHash, int val);
    vector<int> FindSpent(vector<Transaction> &pool);
    vector<Transaction> FindUTXO(const vector<int> &spentTxId, const vector<Transaction> &pool);
    int FindBalance();
    Chain GetChain()
    {
        return chain;
    }
    void SetChain(Chain &newChain)
    {
        chain = newChain;
    }
    static bool VerifyTx(const Transaction &_tx);
};

string Base58(string s);

#endif //WALLET_H
