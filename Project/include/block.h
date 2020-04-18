#ifndef BLOCK_H
#define BLOCK_H
#include <string>
#include <iostream>
#include <cstring>
#include <ctime>
#include "transaction.h"

class Chain;
class Block
{
public:
    friend Chain;
    void Pack(vector<Transaction> &vec);
    Block();
    Block(int _nonce, int difficulty, vector<Transaction> &vec) : difficultyTarget(difficulty), nonce(_nonce)
    {
        Pack(vec);
    }
    Block(const Block &p) : preBlockHash(p.preBlockHash),
                            merkleRoot(p.merkleRoot),
                            time(p.time),
                            difficultyTarget(p.difficultyTarget),
                            height(p.height),
                            nonce(p.nonce){};
    void SetDifficultyTarget(uint32_t difficulty)
    {
        difficultyTarget = difficulty;
    }
    void SetNonce(int _nonce)
    {
        nonce = _nonce;
    }
    std::string GetHash();
    void show() const
    {
        std::string now = ctime(&time);
        std::cout << height << std::endl
                  << preBlockHash << std::endl
                  << merkleRoot << std::endl
                  << now << nonce << std::endl;
    }
    Block *preBlock = nullptr;

private:
    int version = 0x00;
    std::string preBlockHash;
    std::string merkleRoot;
    time_t time;
    int difficultyTarget;
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
        std::string TransactionHash;
        node *leftTree;
        node *rightTree;
        node *father;
    };
    static node *CreateTree(std::vector<Transaction> &vec);

private:
    uint64_t numTransactions = 0;
    std::vector<Transaction> transactionSet;
    node *merkleTree;
};

#endif //BLOCK_H