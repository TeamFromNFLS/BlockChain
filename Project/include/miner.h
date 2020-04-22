#ifndef MINER_H
#define MINER_H

#include "wallet.h"
#include "block.h"
class Miner : public Wallet
{
public:
    Miner(int worker);
    Miner();
    bool Check(const Block &toCheck);
    bool PoW(int nonce);
    uint32_t GetNonce() const
    {
        return nonce;
    }
    void SetNonce(int _nonce)
    {
        nonce = _nonce;
    }

protected:
    int nonce = 1;
    int difficultyTarget;
    Block stored;
};
#endif //MINER_H