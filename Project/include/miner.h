#ifndef MINER_H
#define MINER_H

#include "wallet.h"
#include "block.h"
class Miner : public Wallet
{
public:
    Miner(int worker);
    Miner(){};
    bool Check(const Block &toCheck);
    void PoW();

protected:
    uint32_t nonce = 1;
    Block now;
};
#endif //MINER_H