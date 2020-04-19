#ifndef CHAIN_H
#define CHAIN_H
#include <vector>
#include "block.h"
#include "miner.h"

class Chain
{
public:
    Chain();
    Chain(const Chain &p) : difficulty(p.difficulty), end(p.end){};

    Block *GetLastBlock()
    {
        return end;
    }
    void AddBlock(Block &newBlock)
    {
        Block *newBlockPoint = &newBlock;
        newBlockPoint->preBlock = end;
        end = newBlockPoint;
    }
    void SetDifficulty(int difficultyTarget)
    {
        difficulty = difficultyTarget;
    }
    int GetDifficulty()
    {
        return difficulty;
    }
    static string GetCoinBaseIndex();
    static vector<Transaction> GetTransaction();
    static vector<Miner> minerSet;
    void print();

private:
    int difficulty = 0;
    Block *end = nullptr;
};

extern Chain blockChain;

#endif //CHAIN_H