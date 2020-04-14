#include "miner.h"
#include "block.h"

Miner::Miner(int worker)
{
    Init(worker);
}

void Miner::PoW()
{
    for (uint64_t i = 0; i < UINT64_MAX; ++i)
    {
        ;
    }
    return;
}

bool Miner::Check(const Block &toCheck)
{
    return true;
}
