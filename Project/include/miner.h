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
    bool PoW(int nonce);
    uint32_t GetNonce() const
    {
        return nonce;
    }
    Block GetBlock()
    {
        return now;
    }
    void SetNonce(uint32_t _nonce)
    {
        nonce = _nonce;
        now.SetNonce(_nonce);
    }

protected:
    uint32_t nonce = 1;
    Block now;
};
#endif //MINER_H