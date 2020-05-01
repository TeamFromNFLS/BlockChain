#include <iostream>
#include <cstdlib>
#include <vector>
#include "chain.h"
#include "block.h"
#include "wallet.h"
#include "miner.h"

using namespace std;

time_t _time_;
string _s;

/*define for log*/
Block coinBase;
Chain blockChain;

Chain::Chain()
{
    coinBase.preBlockHash = "0";
    coinBase.merkleRoot = "0";
    coinBase.difficultyTarget = "0000";
    coinBase.height = 1;
    coinBase.nonce = 0;
    SetDifficulty("0000");
    end = &coinBase;
}

void Chain::print()
{
    Block *now = end;
    while (now != nullptr)
    {
        now->Show();
        now = now->preBlock;
    }
}

void Chain::SetDifficulty(string difficultyTarget)
{
    difficulty = difficultyTarget;
    for (int i = 0; i < Miner::minerSet.size(); ++i)
    {
        Miner::minerSet[i]->SetDifficulty(difficultyTarget);
    }
}

vector<Transaction> Chain::GetTransaction()
{
    vector<Transaction> result;
    Block *now = blockChain.end;
    while (now != nullptr)
    {
        vector<Transaction> tmp;
        now->GetTransactionSet(tmp);
        result.insert(result.end(), tmp.begin(), tmp.end());
        now = now->preBlock;
    }
    return result;
}

string Chain::GetCoinBaseIndex()
{
    Block *now = blockChain.end;
    while (now->preBlock != nullptr)
    {
        now = now->preBlock;
    }
    string result;
    result = Base58(now->GetHash());
    return result;
}