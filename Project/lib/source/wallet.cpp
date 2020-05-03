#include <iostream>
#include <tuple>
#include <vector>
#include <cmath>
#include <ctime>
#include <algorithm>
#include <iomanip>
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

const string Base58String = "123456789ABCDEFGHJKLMNPQRSTUVWXYZabcdefghijkmnopqrstuvwxyz";
string Base58(string s)
{
    BigInt x = BigInt::ToInt(s), base("58");
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
vector<Wallet> Wallet::walletSet;
vector<Wallet> Wallet::onlyWalletSet;

bool Wallet::CheckChain()
{
    Block *now = chain.GetLastBlock();
    /*if the chain only contains the first block*/
    if (now->GetTreeRoot() == "0")
    {
        return true;
    }
    /*check merkle tree in every block*/
    bool flag = true;
    while (now != nullptr)
    {
        pair<bool, Block::node *> check = now->CreateTreeCheck();
        if (check.first = false)
        {
            flag = false;
            LOGOUT << "Incorrect block on tx: " << check.second << endl;
        }
        now = now->preBlock;
    }
    if (!flag)
    {
        return false;
    }
    return true;
}

void Wallet::Init(int worker)
{
    RSA a;
    a.Init(worker);
    a.CreateKeys();
    tuple<BigInt, BigInt, BigInt> keys;
    keys = a.GetKey();
    *publicKey = get<0>(keys);
    privateKey = get<1>(keys);
    n = get<2>(keys);
    publicKeyHash = rmd160(sha256(publicKey.ToString() + n.ToString()));
    string versionpublicKeyHash = version + publicKeyHash;
    string tailHash = sha256(sha256(versionpublicKeyHash)).substr(0, 4);
    string finalHash = versionpublicKeyHash + tailHash;
    address = Base58(finalHash);
    chain = blockChain;
    LOGOUT << "Begin checking blockchain in this wallet..." << endl;
    if (!CheckChain())
    {
        LOGOUT << "Incorrect chain in this wallet." << endl;
    }
    else
    {
        LOGOUT << "Passed blockchain test in this wallet." << endl;
        LOGOUT << "Complete." << endl;
        LOGOUT << "Address: " << address << endl
               << "------------------------------------------" << endl;
        walletInfo.push_back(make_pair(address, publicKeyHash));
        walletSet.push_back(*this);
        onlyWalletSet.push_back(*this);
    }
}

void Wallet::InitPrint()
{
    LOGOUT << "Creating a new wallet. Please wait..." << endl;
}

void Wallet::SetWallet(BigInt _publicKey, BigInt _privateKey, BigInt _N, string _addr)
{
    publicKey = _publicKey;
    privateKey = _privateKey;
    n = _N;
    address = _addr;
    publicKeyHash = rmd160(sha256(publicKey.ToString() + n.ToString()));
    walletInfo.push_back(make_pair(address, publicKeyHash));
}

//00c94696460043b120981f922acd5f3bccefe1a5fdd6d4

bool Wallet::CreateTransaction(pair<string, string> receiverInfo, int _value)
{
    Transaction transaction(address, get<0>(receiverInfo));
    TxInput _input(_value, publicKey, n);
    TxOutput _output(_value, get<1>(receiverInfo));
    transaction.input = _input;
    transaction.output = _output;

    /* determine prevTx */
    vector<int> spentTxId = FindSpent(Transaction::txPool);
    vector<Transaction> CandidateTx = FindUTXO(spentTxId, Transaction::packedTx);

    if (!CandidateTx.size())
    {
        LOGOUT << "Transaction construction failed. No matching UTXO." << endl;
        return false;
    }
    else
    {
        for (Transaction &tx : CandidateTx)
        {
            if (tx.output.GetValue() == _value)
            {
                transaction.input.SetPrevID(tx.GetID());
                LOGOUT << "Transaction ID: " << transaction.GetID() << endl;
                //transaction.SetID();
                break;
            }
        }
        Sign(transaction, get<1>(receiverInfo), _value);
        Transaction::txPool.push_back(transaction);
        Transaction::toBePackedTx.push_back(transaction);

        LOGOUT << "Transaction constructed by " << address << endl
               << "------------------------------------------" << endl;

        LOGOUT << "Transaction log: " << endl;
        LOGOUT << "Type: Normal transaction" << endl;
        LOGOUT << "Sender Address: " << address << endl;
        LOGOUT << "Receiver Address: " << get<0>(receiverInfo) << endl;
        LOGOUT << "Value: " << transaction.output.GetValue() << endl;
        LOGOUT << "ID: " << transaction.GetID() << endl;
        LOGOUT << "PrevTx ID: " << transaction.input.GetPrevID() << endl;
        LOGOUT << "Signature: " << transaction.input.signature << endl
               << "------------------------------------------" << endl;
    }
    return true;
}

void Wallet::CreateCoinbase(int x)
{
    Transaction::mineReward = x;
    Transaction transaction("null", address); //as receiver
    TxOutput _output(Transaction::mineReward, publicKeyHash);
    transaction.output = _output;
    //transaction.SetID();
    Transaction::txPool.push_back(transaction);

    Transaction::toBePackedTx.push_back(transaction);
    LOGOUT << "Coinbase transaction constructed." << endl
           << "------------------------------------------" << endl;

    LOGOUT << "Transaction log: " << endl;
    LOGOUT << "Type: Coinbase transaction" << endl;
    LOGOUT << "Receiver Address: " << address << endl;
    LOGOUT << "Value: " << transaction.output.GetValue() << endl;
    LOGOUT << "ID: " << transaction.GetID() << endl
           //<< "PrevTx ID: " << transaction.input.GetPrevID() << endl
           << "------------------------------------------" << endl;
}

void Wallet::Sign(Transaction &tx, string receiverPublicKeyHash, int _value)
{
    if (!tx.IsCoinbase())
    {
        RSA rsa;
        /* cout << "publicKeyHash: " << publicKeyHash << endl
             << "receiverPublicKeyHash: " << receiverPublicKeyHash << endl; */
        string signStr = "0x" + publicKeyHash + receiverPublicKeyHash;
        BigInt signInfo(signStr);
        BigInt _signature = RSA::EncryptAndDecrypt(signInfo, privateKey, n);
        /* BigInt _decrypt = RSA::EncryptAndDecrypt(_signature, publicKey, n);
        bool isSame = (signInfo == _decrypt);
        cout << "------------------------------------------" << endl
             << "The decrypted signature is the same as the hash value: " << isSame << endl
             << "------------------------------------------" << endl; */
        tx.input.signature = _signature;
    }
    LOGOUT << "Digital signature created." << endl;
}

vector<int> Wallet::FindSpent(vector<Transaction> &pool) //to be iterated over
{
    vector<int> spentTxID;
    for (Transaction &tx : pool)
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

vector<Transaction> Wallet::FindUTXO(const vector<int> &spentTxId, const vector<Transaction> &pool)
{
    vector<Transaction> txPool = pool, UTXOTx;
    vector<int> spent = spentTxId;
    vector<int>::iterator ret;

    for (Transaction &tx : txPool)
    {
        ret = find(spent.begin(), spent.end(), tx.txID);
        if (ret == spentTxId.end()) //not found
        {
            UTXOTx.push_back(tx);
        }
    }
    return UTXOTx;
}

int Wallet::FindBalance()
{
    int result = 0;
    vector<Transaction> myPackedTx;
    vector<int> myPackedSpentTxID;
    vector<Transaction> balanceTx;
    for (Transaction &tx : Transaction::packedTx)
    {
        if (tx.receiverAdr == address)
        {
            myPackedTx.push_back(tx);
        }
    }

    myPackedSpentTxID = FindSpent(myPackedTx);
    balanceTx = FindUTXO(myPackedSpentTxID, myPackedTx);

    LOGOUT << "UTXO information:" << endl;
    LOGOUT << setiosflags(ios::left) << setfill(' ') << setw(20) << "Transaction ID"
           << "\t"
           << "UTXO value" << endl;
    //int count = 1;
    for (Transaction &tx : balanceTx)
    {
        result += tx.output.GetValue();
        LOGOUT << setiosflags(ios::left) << setfill(' ') << setw(20) << tx.GetID() << "\t" << tx.output.GetValue() << endl;
        //count++;
    }
    return result;
}

bool Wallet::VerifyTx(const Transaction &_tx)
{
    Transaction tx = _tx;
    if (!tx.IsCoinbase())
    {
        string senderHash = rmd160(sha256(tx.input.publicKey.ToString() + tx.input.N.ToString()));
        string receiverHash = tx.output.GetPublicHash();
        string info = "0x" + senderHash + receiverHash;
        BigInt infoInt(info);
        BigInt decrypted = RSA::EncryptAndDecrypt(tx.input.signature, tx.input.publicKey, tx.input.N);
        LOGOUT << "Signature decrypted." << endl;
        if (decrypted == infoInt)
        {
            LOGOUT << "Signature matches identity." << endl;
            LOGOUT << "Valid transaction. To be packed." << endl;
            return true;
        }
        else
        {
            LOGOUT << "Signature does not match identity. " << endl;
            LOGOUT << "Invalid transaction. Refuse to pack." << endl;
            return false;
        }
    }
    LOGOUT << "Valid coinbase transaction. To be packed." << endl;
    return true;
}
