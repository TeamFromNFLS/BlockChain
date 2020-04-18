#include <iostream>
#include <tuple>
#include <vector>
#include <cmath>
#include <ctime>
#include <algorithm>
#include "wallet.h"
#include "rsa.h"
#include "bigInt.h"
#include "sha256.h"
#include "ripemd160.h"
#include "transaction.h"
#include "txInput.h"
#include "txOutput.h"
using namespace std;

BigInt ToInt(string s)
{
    vector<uint64_t> result;
    int len = s.length();
    uint64_t cnt = 0;
    for (int i = len - 1; i >= 0; --i)
    {
        if (!cnt)
        {
            result.push_back(0);
        }
        uint64_t now;
        if (s[i] >= 'a' && s[i] <= 'z')
        {
            now = s[i] - 'a' + 10;
        }
        else
        {
            now = s[i] - '0';
        }
        result.back() += now * (uint64_t)pow(UINT64_C(16), cnt);
        cnt = (cnt + 1) % 16;
    }
    return BigInt(result);
}

const string Base58String = "123456789ABCDEFGHJKLMNPQRSTUVWXYZabcdefghijkmnopqrstuvwxyz";
string Wallet::Base58(string s)
{
    BigInt x = ToInt(s), base("58");
    string result;
    vector<uint64_t> now;
    while (x > BigInt::zero)
    {
        /*BigInt tmp = x % base;
        cout << tmp << endl;*/
        (x % base).GetNumber(now);
        result += Base58String[now[0]];
        x /= base;
    }
    string::iterator it = s.begin();
    while (*it == '0')
    {
        result += Base58String[0];
        it++;
    }
    reverse(result.begin(), result.end());
    return result;
}

vector<pair<string, string>> Wallet::walletInfo;

void Wallet::Init(int worker)
{
    RSA a;
    a.Init(worker);
    a.CreateKeys();
    tuple<BigInt, BigInt, BigInt> keys;
    keys = a.GetKey();
    publicKey = get<0>(keys);
    privateKey = get<1>(keys);
    n = get<2>(keys);
    publicKeyHash = rmd160(sha256(publicKey.ToString() + n.ToString()));
    string versionpublicKeyHash = version + publicKeyHash;
    string tailHash = sha256(sha256(versionpublicKeyHash)).substr(0, 4);
    string finalHash = versionpublicKeyHash + tailHash;
    address = Base58(finalHash);
    walletInfo.push_back(make_pair(address, publicKeyHash));
    cout << address << endl;
}

Wallet::Wallet(int worker)
{
    Init(worker);
}

//00c94696460043b120981f922acd5f3bccefe1a5fdd6d4

void Wallet::CreateTransaction(pair<string, string> receiverInfo, int _value)
{
    Transaction transaction(address, get<0>(receiverInfo));
    TxInput _input(_value, publicKey);
    TxOutput _output(_value, get<1>(receiverInfo));
    transaction.input = _input;
    transaction.output = _output;
    Sign(transaction, get<1>(receiverInfo), _value);
    //TODO: find prevTx and set ID
    vector<Transaction> UTXOTx;
    for (Transaction tx : UTXOTx)
    {
        if (tx.output.GetValue() == _value)
        {
            transaction.input.SetPrevID(tx.GetID());
            transaction.SetID(tx.GetID() + 1);
            break;
        }
    }

    Transaction::txPool.push_back(transaction);
    Transaction::toBePackedTx.push_back(transaction);
}

void Wallet::Sign(Transaction &tx, string receiverPublicKeyHash, int _value)
{
    if (!tx.IsCoinbase())
    {
        RSA rsa;
        BigInt signInfo(publicKeyHash + receiverPublicKeyHash + to_string(_value));
        BigInt _signature = rsa.EncryptByPrivate(signInfo, privateKey, n);
        tx.input.signature = _signature;
    }
}

vector<int> Wallet::FindSpent()
{
    vector<int> spentTxID;
    for (Transaction tx : Transaction::txPool)
    {
        if (tx.IsCoinbase())
        {
            continue;
        }

        if (tx.senderAdr == address)
        {
            spentTxID.push_back(tx.input.GetPrevID());
        }
    }
    return spentTxID;
}

vector<Transaction> FindUTXO(vector<int> spentTxId)
{
    vector<Transaction> UTXOTxId;
    //TODO: search blockchain
}

string GetTxHash(const Transaction &transaction)
{
    Transaction tx = transaction;
    string time = ctime(&tx._time);
    string hashInfo = time + tx.senderAdr + tx.receiverAdr + to_string(tx.input.GetValue());
    string txHash = sha256(hashInfo);
    return txHash;
}