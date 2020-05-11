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
    publicKey = get<0>(keys);
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
            if (tx.output.GetValue() >= _value)
            {
                Transaction transaction(address, get<0>(receiverInfo));
                TxInput _input(tx.output.GetValue(), publicKey, n);
                TxOutput _output(_value, get<1>(receiverInfo));
                transaction.input = _input;
                transaction.output = _output;
                transaction.input.SetPrevID(tx.GetID());
                LOGOUT << "Transaction ID: " << transaction.GetID() << endl;
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

                if (tx.output.GetValue() > _value)
                {
                    int remainder = tx.output.GetValue() - _value;
                    Transaction self(address, address);
                    TxInput _selfInput(tx.output.GetValue(), publicKey, n);
                    TxOutput _selfOutput(remainder, publicKeyHash);
                    self.input = _selfInput;
                    self.output = _selfOutput;
                    self.input.SetPrevID(tx.GetID());
                    LOGOUT << "Transaction ID: " << self.GetID() << endl;
                    Sign(self, publicKeyHash, remainder);
                    Transaction::txPool.push_back(self);
                    Transaction::toBePackedTx.push_back(self);

                    LOGOUT << "Transaction constructed by " << address << endl
                           << "------------------------------------------" << endl;

                    LOGOUT << "Transaction log: " << endl;
                    LOGOUT << "Type: Change" << endl;
                    LOGOUT << "Sender Address: " << address << endl;
                    LOGOUT << "Receiver Address: " << address << endl;
                    LOGOUT << "Value: " << self.output.GetValue() << endl;
                    LOGOUT << "ID: " << self.GetID() << endl;
                    LOGOUT << "PrevTx ID: " << self.input.GetPrevID() << endl;
                    LOGOUT << "Signature: " << self.input.signature << endl
                           << "------------------------------------------" << endl;
                }
                return true;
            }
        }
        LOGOUT << "Invalid input value" << endl;
        return false;
    }
}

void Wallet::CreateCoinbase(int x)
{
    Transaction transaction(address); //as receiver
    TxOutput _output(x, publicKeyHash);
    transaction.output = _output;
    //transaction.SetID();
    Transaction::txPool.push_back(transaction);
    Transaction::toBePackedTx.push_back(transaction);

    LOGOUT << "Coinbase transaction constructed." << endl
           << "------------------------------------------" << endl;

    LOGOUT << "Transaction log: " << endl;
    LOGOUT << "Type: Coinbase transaction" << endl;
    LOGOUT << "Sender Address: " << transaction.senderAdr << endl;
    LOGOUT << "Receiver Address: " << address << endl;
    LOGOUT << "Value: " << transaction.output.GetValue() << endl;
    LOGOUT << "ID: " << transaction.GetID() << endl
           //<< "PrevTx ID: " << transaction.input.GetPrevID() << endl
           << "------------------------------------------" << endl;
}

void Wallet::CreateCoinbase()
{
    Transaction transaction(address); //as receiver
    TxOutput _output(Transaction::mineReward, publicKeyHash);
    transaction.output = _output;
    //transaction.SetID();
    Transaction::txPool.push_back(transaction);
    Transaction::toBePackedTx.push_back(transaction);

    LOGOUT << "Coinbase transaction constructed." << endl
           << "------------------------------------------" << endl;

    LOGOUT << "Transaction log: " << endl;
    LOGOUT << "Type: Coinbase transaction" << endl;
    LOGOUT << "Sender Address: " << transaction.senderAdr << endl;
    LOGOUT << "Receiver Address: " << address << endl;
    LOGOUT << "Value: " << transaction.output.GetValue() << endl;
    LOGOUT << "ID: " << transaction.GetID() << endl
           //<< "PrevTx ID: " << transaction.input.GetPrevID() << endl
           << "------------------------------------------" << endl;
}

string ConverToHex(int x)
{
    stringstream ss;
    ss << hex << x;
    string out(ss.str());
    return out;
}

void Wallet::Sign(Transaction &tx, string receiverPublicKeyHash, int val)
{
    if (!tx.IsCoinbase())
    {
        /* cout << "publicKeyHash: " << publicKeyHash << endl
             << "receiverPublicKeyHash: " << receiverPublicKeyHash << endl; */
        string hexVal = ConverToHex(val);
        string signStr = "0x" + publicKeyHash + receiverPublicKeyHash + hexVal;
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
    vector<Transaction> txPool = pool, myTx, UTXOTx;
    vector<int> spent = spentTxId;
    vector<int>::iterator ret;
    for (Transaction &tx : txPool)
    {
        if (tx.receiverAdr == address)
        {
            myTx.push_back(tx);
        }
    }

    for (Transaction &tx : myTx)
    {
        ret = find(spent.begin(), spent.end(), tx.txID);
        if (ret == spent.end()) //not found
        {
            UTXOTx.push_back(tx);
        }
    }
    return UTXOTx;
}

int *Wallet::FindBalance()
{
    int result = 0, cnt = 0;
    vector<int> spentTxId = FindSpent(Transaction::txPool);
    vector<Transaction> balanceTx = FindUTXO(spentTxId, Transaction::packedTx);
    int len = balanceTx.size() + 1;
    int *balance = new int[len + 1];
    balance[cnt++] = len;

    LOGOUT << "UTXO information:" << endl;
    LOGOUT << setiosflags(ios::left) << setfill(' ') << setw(20) << "Transaction ID"
           << "\t"
           << "UTXO value" << endl;
    //int count = 1;
    if (!balanceTx.size())
    {
        LOGOUT << setiosflags(ios::left) << setfill(' ') << setw(20) << "null"
               << "\t"
               << "null" << endl;
    }
    else
    {
        for (Transaction &tx : balanceTx)
        {
            result += tx.output.GetValue();
            balance[cnt++] = tx.output.GetValue();
            LOGOUT << dec << setiosflags(ios::left) << setfill(' ') << setw(20) << tx.GetID() << "\t" << tx.output.GetValue() << endl;
            //count++;
        }
    }
    balance[cnt] = result;
    return balance;
}

bool Wallet::VerifyTx(const Transaction &_tx)
{
    Transaction tx = _tx;
    if (!tx.IsCoinbase())
    {
        string senderHash = rmd160(sha256(tx.input.publicKey.ToString() + tx.input.N.ToString()));
        string receiverHash = tx.output.GetPublicHash();
        string hexVal = ConverToHex(tx.output.GetValue());
        string info = "0x" + senderHash + receiverHash + hexVal;
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
