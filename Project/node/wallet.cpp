#include <iostream>
#include <tuple>
#include <vector>
#include <cmath>
#include <ctime>
#include <algorithm>
#include "wallet.h"
#include "rsa.h"
#include "chain.h"
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
string Base58(string s)
{
    BigInt x = ToInt(s), base("58");
    string result;
    vector<uint64_t> now;
    while (x > BigInt::zero)
    {
        /*BigInt tmp = x % base;
        cout << tmp << endl;*/
        (x % base).GetNumber(now);
        if (now.empty())
        {
            now.push_back(0);
        }
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
    cout << "Complete. Address: " << address << endl;
}

Wallet::Wallet(int worker)
{
    cout << "Creating a new wallet. Please wait..." << endl;
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

    /* determine prevTx */
    vector<int> spentTxId = FindSpent();
    vector<Transaction> CandidateTx = FindUTXO(spentTxId);
    if (!CandidateTx.size())
    {
        cout << "Transaction construction failed. No available coins." << endl;
    }
    else
    {
        for (Transaction tx : CandidateTx)
        {
            if (tx.output.GetValue() == _value)
            {
                transaction.input.SetPrevID(tx.GetID());
                transaction.SetID(tx.GetID() + 1);
                break;
            }
        }
        Sign(transaction, get<1>(receiverInfo), _value);
        Transaction::txPool.push_back(transaction);
        Transaction::toBePackedTx.push_back(transaction);
        cout << "Transaction constructed at " << transaction._time << endl;
    }
}

void Wallet::CreateCoinbase()
{
    Transaction transaction(address); //as receiver
    TxOutput _output(Transaction::mineReward, publicKeyHash);
    Transaction::txPool.push_back(transaction);
    Transaction::toBePackedTx.push_back(transaction);
    cout << "Coinbase transaction constructed at " << transaction._time << endl;
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
    cout << "Digital signature created." << endl;
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

vector<Transaction> Wallet::FindUTXO(vector<int> spentTxId)
{
    vector<Transaction> UTXOTx;
    vector<Transaction> chainTx = Chain::GetTransaction();
    vector<int>::iterator ret;
    for (Transaction tx : chainTx)
    {
        ret = find(spentTxId.begin(), spentTxId.end(), tx.txID);
        if (ret == spentTxId.end()) //not found
        {
            UTXOTx.push_back(tx);
        }
    }
    return UTXOTx;
}
