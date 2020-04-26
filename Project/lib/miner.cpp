#include <random>
#include <vector>
#include "miner.h"
#include "block.h"
#include "chain.h"
#include "sha256.h"
#include "ripemd160.h"

using namespace std;

vector<Miner *> Miner::minerSet;

Miner::Miner(int worker) : Wallet()
{
    Init(worker);
    mt19937 rng;
    random_device randev;
    rng.seed(randev());
    uniform_int_distribution<int> num(0, INT32_MAX);
    nonce = num(rng);
    Miner::minerSet.push_back(this);
}

Miner::Miner() : Wallet()
{
    mt19937 rng;
    random_device randev;
    rng.seed(randev());
    uniform_int_distribution<int> num(0, INT32_MAX);
    nonce = num(rng);
    difficultyTarget = blockChain.GetDifficulty();
    Miner::minerSet.push_back(this);
}

void Miner::Reset()
{
    Block tmp;
    stored = tmp;
    mt19937 rng;
    random_device randev;
    rng.seed(randev());
    uniform_int_distribution<int> num(0, INT32_MAX);
    nonce = num(rng);
}

void Miner::Init(int worker)
{
    cout << "Creating a new miner. Please wait..." << endl;
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
    chain = blockChain;
    cout << "Begin checking blockchain in this miner..." << endl;
    if (!CheckChain())
    {
        cout << "Incorrect chain in this miner." << endl;
    }
    else
    {
        cout << "Passed blockchain test in this miner." << endl;
    }
    cout << "Complete." << endl
         << "Address: " << address << endl
         << "------------------------------------------" << endl;
}

bool Miner::TestPoW(int nonce)
{
    stored.SetNonce(nonce);
    int len = difficultyTarget.length();
    string hash = stored.GetHash();
    string check = hash.substr(0, len);
    if (check == difficultyTarget)
    {
        return true;
    }
    return false;
}

void Miner::PoW(vector<Transaction> &vec)
{
    int now = nonce;
    Block tmp(now, difficultyTarget, vec);
    stored = tmp;
    while (true)
    {
        if (TestPoW(now))
        {

            if (Check(stored))
            {
                stored.Pack();
                stored.Show();
            }
            break;
        }
        now = (now + 1) % INT32_MAX;
    }
}

bool Miner::Check(Block &toCheck)
{
    cout << "Begin checking block..." << endl
         << endl;
    int len = difficultyTarget.length();
    string hash = toCheck.GetHash();
    string check = hash.substr(0, len);
    if (check != difficultyTarget)
    {
        cout << "Failed Proof of Work test. Refuse to pack." << endl;
        return false;
    }
    cout << "Passed Proof of Work test. Hash: " << hash << endl
         << endl;
    vector<Transaction> checkTx;
    toCheck.GetTransactionSet(checkTx);
    if (Block::CreateTree(checkTx)->transactionHash != toCheck.GetTreeRoot())
    {
        cout << "Failed merkle tree root test. Refuse to pack." << endl;
        return false;
    }
    cout << "Passed merkle tree root test. Merkle tree root hash: " << toCheck.GetTreeRoot() << endl
         << endl;
    bool checkFlag = false;
    for (auto &tx : checkTx)
    {
        if (tx.senderAdr == "null")
        {
            if (!checkFlag)
            {
                checkFlag = true;
            }
            else
            {
                cout << "Failed one coinBase test. Refuse to pack." << endl;
                return false;
            }
        }
    }
    cout << "Passed one coinBase test. Only one reward received." << endl
         << endl;
    for (auto &tx : checkTx)
    {
        cout << "Now transaction: " << tx.GetID() << endl;
        if (!Wallet::VerifyTx(tx))
        {
            return false;
        }
    }
    cout << "Passed transaction test. All transactions are correct." << endl
         << endl
         << "All tests passed! Ready to be packed." << endl
         << "------------------------------------------" << endl;
    return true;
}
