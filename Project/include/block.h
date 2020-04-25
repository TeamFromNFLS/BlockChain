#ifndef BLOCK_H
#define BLOCK_H
#include <string>
#include <iostream>
#include <cstring>
#include <ctime>
#include <queue>
#include "transaction.h"

class Chain;
class Block
{
public:
    friend Chain;
    void Pack();
    Block(){};
    Block(int _nonce, string difficulty, vector<Transaction> &vec);
    Block(const Block &p);
    void SetDifficultyTarget(string difficulty)
    {
        difficultyTarget = difficulty;
    }
    void SetNonce(int _nonce)
    {
        nonce = _nonce;
    }
    int GetHeight()
    {
        return height;
    }
    std::string GetHash();
    void Show();
    Block *preBlock = nullptr;

private:
    int version = 0x00;
    std::string preBlockHash = "0";
    std::string merkleRoot;
    time_t time;
    string difficultyTarget;
    int height;
    int nonce;

public:
    bool AddTransactionSet(std::vector<Transaction> &add)
    {
        transactionSet.insert(transactionSet.end(), add.begin(), add.end());
        numTransactions += add.size();
        return true;
    }
    bool GetTransactionSet(std::vector<Transaction> &result)
    {
        result.assign(transactionSet.begin(), transactionSet.end());
        return true;
    }
    struct node
    {
        std::string transactionHash;
        node *leftTree = nullptr;
        node *rightTree = nullptr;
        node *father = nullptr;
    };
    static node *CreateTree(vector<Transaction> &vec);
    string GetTreeRoot()
    {
        return merkleRoot;
    }
    void ShowTree();

private:
    uint64_t numTransactions = 0;
    std::vector<Transaction> transactionSet;
    node *merkleTreeRoot;
    vector<node *> merkleTree;
};

#endif //BLOCK_H