#include <iostream>
#include <string>
#include <cstring>
#include <ctime>
#include <queue>
#include <utility>
#include <random>
#include "chain.h"
#include "block.h"
#include "wallet.h"
#include "sha256.h"
#include "transaction.h"

using namespace std;

Block::node *Block::CreateTree(vector<Transaction> &vec)
{
    bool evenFlag = false;
    int len = vec.size();
    vector<Transaction> newVec = vec;
    if (len & 1)
    {
        len++;
        newVec.push_back(newVec.back());
        evenFlag = true;
    }
    Block::node *now, *next;
    queue<Block::node *> q;
    for (int i = 0; i < len; i++)
    {
        string s;
        s = newVec[i].GetTxHash();
        now = new Block::node;
        now->transactionHash = s;
        now->id = i;
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
        q.pop();
        now = q.front();
        now->father = next;
        next->rightTree = now;
        b = now->transactionHash;
        q.pop();
        next->transactionHash = sha256(a + b);
        q.push(next);
    }
    return next;
}

/*Assume that only one tx is corrupted.*/
pair<bool, Block::node *> Block::CreateTreeCheck()
{
    bool evenFlag = false;
    vector<Transaction> vec;
    GetTransactionSet(vec);
    int len = vec.size();
    vector<Transaction> newVec = vec;
    if (len & 1)
    {
        len++;
        newVec.push_back(newVec.back());
        evenFlag = true;
    }
    Block::node *now, *next;
    queue<Block::node *> q;
    for (int i = 0; i < len; i++)
    {
        string s;
        s = newVec[i].GetTxHash();
        now = new Block::node;
        now->transactionHash = s;
        now->id = i;
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
        q.pop();
        now = q.front();
        now->father = next;
        next->rightTree = now;
        b = now->transactionHash;
        q.pop();
        next->transactionHash = sha256(a + b);
        q.push(next);
    }
    if (next->transactionHash != merkleRoot)
    {
        now = next;
        next = merkleTreeRoot;
        while (now->id < 0)
        {
            if (now->leftTree != next->leftTree)
            {
                now = now->leftTree;
                next = next->leftTree;
            }
            else
            {
                now = now->rightTree;
                next = next->rightTree;
            }
        }
        return make_pair(false, now);
    }
    return make_pair(true, next);
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
        LOGOUT << "Depth: " << tmp.first << ' ' << "Hash: " << tmp.second->transactionHash << endl;
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
    cout << "------------------------------------------" << endl;
    return;
}

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
        now->id = i;
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
    blockChain.AddBlock(this);
    for (auto &tx : transactionSet)
    {
        Transaction::packedTx.push_back(tx);
    }
    Transaction::toBePackedTx.clear();
    for (auto &t : Wallet::walletSet)
    {
        t.SetChain(blockChain);
    }
    return;
}

void Block::Show()
{
    string now = ctime(&time);
    LOGOUT << "Block Log:" << endl
           << "------------------------------------------" << endl;
    LOGOUT << "Head:" << endl;
    LOGOUT << "Height: " << height << endl;
    LOGOUT << "PreBlockHash: " << preBlockHash << endl;
    LOGOUT << "MerkleRoot: " << merkleRoot << endl;
    LOGOUT << "Date: " << now << "nonce: " << nonce << endl
           << "------------------------------------------" << endl;
    LOGOUT << "Transaction:" << endl
           << "------------------------------------------" << endl;
    for (int i = 0; i < transactionSet.size(); ++i)
    {
        transactionSet[i].Show();
    }
    LOGOUT << "Merkle Tree: " << endl;
    ShowTree();
    cout << "------------------------------------------" << endl;
}