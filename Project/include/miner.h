#ifndef MINER_H
#define MINER_H

#include <vector>
#include "wallet.h"
#include "block.h"
class Miner : public Wallet
{
public:
    Miner(int worker);
    Miner();
    bool Check(const Block &toCheck);
    bool TestPoW(int nonce);
    void PoW(std::vector<Transaction> &vec);
    uint32_t GetNonce() const
    {
        return nonce;
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