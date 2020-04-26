#ifndef CHAIN_H
#define CHAIN_H
#include <vector>
#include "block.h"

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
    void SetDifficulty(string difficultyTarget);
    std::string GetDifficulty()
    {
        return difficulty;
    }
    static string GetCoinBaseIndex();
    static vector<Transaction> GetTransaction();
    void print();

private:
    std::string difficulty = "0000";
    Block *end = nullptr;
};

extern Chain blockChain;

#endif //CHAIN_H