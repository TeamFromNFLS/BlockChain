#ifndef MINER_H
#define MINER_H

#include <vector>
#include "wallet.h"
#include "block.h"

using namespace std;
class Miner : public Wallet
{
public:
    Miner(int worker);
    Miner();
    virtual void Init(int worker);
    void Reset();
    Miner(const Miner &p) : nonce(p.nonce), difficultyTarget(p.difficultyTarget), stored(p.stored){};
    bool Check(Block &toCheck);
    bool TestPoW(int nonce);
    void PoW(std::vector<Transaction> &vec);
    int GetNonce() const
    {
        return nonce;
    }
    void Load(vector<Transaction> &vec)
    {
        int now = nonce;
        Block tmp(now, difficultyTarget, vec);
        stored = tmp;
    }
    Block GetBlock()
    {
        return stored;
    }
    void SetNonce(int _nonce)
    {
        nonce = _nonce;
    }
    void SetDifficulty(string _difficultyTarget)
    {
        difficultyTarget = _difficultyTarget;
    }
    void ShowDifficulty()
    {
        cout << difficultyTarget << endl;
    }
    static vector<Miner *> minerSet;

protected:
    int nonce;
    string difficultyTarget;
    Block stored;
};
#endif //MINER_H