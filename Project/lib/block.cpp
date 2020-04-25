#include <iostream>
#include <string>
#include <cstring>
#include <ctime>
#include <queue>
#include <utility>
#include <random>
#include "chain.h"
#include "block.h"
#include "sha256.h"
#include "transaction.h"

using namespace std;

Block::node *CreateTree(vector<Transaction> &vec)
{
    bool evenFlag = false;
    int len = vec.size();
    if (len & 1)
    {
        len++;
        vec.push_back(vec.back());
        evenFlag = true;
    }
    Block::node *now, *next;
    queue<Block::node *> q;
    for (int i = 0; i < len; i++)
    {
        string s;
        s = vec[i].GetTxHash();
        now = new Block::node;
        now->transactionHash = s;
        q.push(now);
    }
    while (q.size() > 1)
    {
        string a, b;
        next = new Block::node;
        now = q.back();
        now->father = next;
        next->leftTree = now;
        a = now->transactionHash;
        q.pop();
        now = q.back();
        now->father = next;
        next->rightTree = now;
        b = now->transactionHash;
        q.pop();
        next->transactionHash = sha256(a + b);
        q.push(next);
    }
    if (evenFlag)
    {
        vec.pop_back();
    }
    return next;
}

void Block::ShowTree()
{
    int cnt = 0;
    Block::node *now = *(merkleTree.end() - 1);
    pair<int, Block::node *> tmp = make_pair(cnt, now);
    queue<pair<int, Block::node *>> q;
    q.push(tmp);
    while (!q.empty())
    {
        tmp = q.front();
        cout << "Depth: " << tmp.first << ' ' << "Hash: " << tmp.second->transactionHash << endl;
        q.pop();
        int newCnt = tmp.first + 1;
        if (tmp.second->leftTree != nullptr)
        {
            q.push(make_pair(newCnt, tmp.second->leftTree));
        }
        if (tmp.second->rightTree != nullptr)
        {
            q.push(make_pair(newCnt, tmp.second->rightTree));
        }
    }
    return;
}

/*Block::Block()
{
    mt19937 rng;
    random_device randev;
    rng.seed(randev());
    uniform_int_distribution<int> num(0, INT32_MAX);
    string tmp = to_string(num(rng));
    preBlockHash = sha256(tmp);
    tmp = to_string(num(rng));
    merkleRoot = sha256(tmp);
    height = num(rng);
    nonce = num(rng);
    time = std::time(0);
}*/

Block::Block(int _nonce, string difficulty, vector<Transaction> &vec)
{
    bool evenFlag = false;
    nonce = _nonce;
    difficultyTarget = difficulty;
    preBlock = blockChain.GetLastBlock();
    preBlockHash = preBlock->GetHash();
    height = (preBlock->height) + 1;
    AddTransactionSet(vec);
    int len = transactionSet.size();
    if (len & 1)
    {
        len++;
        transactionSet.push_back(transactionSet.back());
        evenFlag = true;
    }
    Block::node *now, *next;
    queue<Block::node *> q;
    for (int i = 0; i < len; i++)
    {
        string s;
        s = transactionSet[i].GetTxHash();
        now = new Block::node;
        now->transactionHash = s;
        q.push(now);
    }
    while (q.size() > 1)
    {
        string a, b;
        next = new Block::node;
        now = q.front();
        now->father = next;
        next->leftTree = now;
        a = now->transactionHash;
        merkleTree.push_back(now);
        q.pop();
        now = q.front();
        now->father = next;
        next->rightTree = now;
        b = now->transactionHash;
        merkleTree.push_back(now);
        q.pop();
        next->transactionHash = sha256(a + b);
        q.push(next);
    }
    merkleTree.push_back(next);
    merkleTreeRoot = next;
    merkleRoot = merkleTreeRoot->transactionHash;
    if (evenFlag)
    {
        transactionSet.pop_back();
    }
}

Block::Block(const Block &p) : preBlockHash(p.preBlockHash),
                               merkleRoot(p.merkleRoot),
                               time(p.time),
                               difficultyTarget(p.difficultyTarget),
                               height(p.height),
                               nonce(p.nonce),
                               numTransactions(p.numTransactions),
                               merkleTreeRoot(p.merkleTreeRoot)
{
    transactionSet.assign(p.transactionSet.begin(), p.transactionSet.end());
    merkleTree.assign(p.merkleTree.begin(), p.merkleTree.end());
}

string Block::GetHash()
{
    string s;
    s += to_string(version) +
         preBlockHash +
         merkleRoot +
         difficultyTarget +
         to_string(height) +
         to_string(nonce);
    return sha256(sha256(s));
}

void Block::Pack()
{
    time = std::time(0);
    blockChain.AddBlock(*this);
    for (auto &tx : transactionSet)
    {
        Transaction::packedTx.push_back(tx);
    }
    Transaction::toBePackedTx.clear();
    return;
}

void Block::Show()
{
    string now = ctime(&time);
    cout << "Block Log:" << endl
         << "Head:" << endl
         << "------------------------------------------" << endl
         << "Height: " << height << endl
         << "PreBlockHash: " << preBlockHash << endl
         << "MerkleRoot: " << merkleRoot << endl
         << "Date: " << now << "nonce: " << nonce << endl
         << "------------------------------------------" << endl
         << "Transaction:" << endl
         << "------------------------------------------" << endl;
    for (int i = 0; i < transactionSet.size(); ++i)
    {
        transactionSet[i].Show();
    }
    cout << "Merkle Tree: " << endl;
    ShowTree();
}