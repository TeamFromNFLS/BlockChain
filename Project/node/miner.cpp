#include "miner.h"
#include "block.h"
#include "chain.h"

Miner::Miner(int worker)
{
    Init(worker);
    Chain::minerSet.push_back(*this);
}

Miner::Miner()
{
    Chain::minerSet.push_back(*this);
}

bool Miner::PoW(int nonce)
{
    if (nonce == INT16_MAX)
    {
        return true;
    }
    return false;
}

bool Miner::Check(const Block &toCheck)
{
    return true;
}
