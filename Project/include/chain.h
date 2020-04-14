#ifndef CHAIN_H
#define CHAIN_H
#include <vector>
#include "Block.h"

class Chain
{
public:
    Block GetLatestBlock();

private:
    std::vector<Block> chain;
    uint32_t difficulty;
};

#endif //CHAIN_H