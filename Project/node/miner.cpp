#include "miner.h"
#include "block.h"

Miner::Miner(int worker)
{
    Init(worker);
}

bool Miner::PoW(int nonce)
{
    if (nonce == 65537)
    {
        return true;
    }
    return false;
}

bool Miner::Check(const Block &toCheck)
{
    return true;
}
