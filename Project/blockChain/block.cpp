#include <iostream>
#include <string>
#include <cstring>
#include <ctime>
#include <queue>
#include <random>
#include "chain.h"
#include "block.h"
#include "sha256.h"
#include "transaction.h"

using namespace std;

Block::node *CreateTree(vector<Transaction> &vec)
{
    int len = vec.size();
    if (len & 1)
    {
        len++;
        vec.push_back(vec.back());
    }
    Block::node *now, *next;
    queue<Block::node *> q;
    for (int i = 0; i < len; i++)
    {
        string s;
        s = vec[i].GetTxHash();
        now = (Block::node *)malloc(sizeof(Block::node));
        now->TransactionHash = s;
        q.push(now);
    }
    while (q.size() > 1)
    {
        string a, b;
        next = (Block::node *)malloc(sizeof(Block::node));
        now = q.back();
        now->father = next;
        next->leftTree = now;
        a = now->TransactionHash;
        q.pop();
        now = q.back();
        now->father = next;
        next->rightTree = now;
        b = now->TransactionHash;
        q.pop();
        next->TransactionHash = sha256(a + b);
        q.push(next);
    }
    return next;
}

Block::Block()
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
}

Block::Block(int _nonce, int difficulty, vector<Transaction> &vec)
{
    nonce = _nonce;
    difficultyTarget = difficulty;
    AddTransactionSet(vec);
    merkleRoot = CreateTree(transactionSet)->TransactionHash;
}

string Block::GetHash()
{
    string s;
    s += to_string(version) +
         preBlockHash +
         merkleRoot +
         to_string(difficultyTarget) +
         to_string(height) +
         to_string(nonce) +
         ctime(&time);
    return sha256(sha256(s));
}

void Block::Pack(vector<Transaction> &vec)
{
    AddTransactionSet(vec);
    preBlock = blockChain.GetLastBlock();
    preBlockHash = preBlock->GetHash();
    height = preBlock->height + 1;
    merkleRoot = CreateTree(transactionSet)->TransactionHash;
    time = std::time(0);
    return;
}