#include <iostream>
#include <cstdlib>
#include "chain.h"
#include "block.h"

void Chain::print()
{
    Block *now = end;
    while (now != nullptr)
    {
        now->show();
        now = now->preBlock;
    }
}

void Chain::SetDifficulty(uint32_t difficultyTarget)
{
    difficulty = difficultyTarget;
}

vector<Transaction> Chain::GetTransaction()
{
    vector<Transaction> result;
    Block *now = end;
    while (now != nullptr)
    {
        vector<Transaction> tmp;
        now->GetTransactionSet(tmp);
        result.insert(result.end(), tmp.begin(), tmp.end());
        now = now->preBlock;
    }
    return result;
}