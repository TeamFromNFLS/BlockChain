#ifndef CHAIN_H
#define CHAIN_H
#include <vector>
#include "Block.h"

class Chain
{
public:
    Chain(){};
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
    void SetDifficulty(uint32_t difficultyTarget);
    void print();

private:
    uint32_t difficulty = 0;
    Block *end = nullptr;
};

#endif //CHAIN_H