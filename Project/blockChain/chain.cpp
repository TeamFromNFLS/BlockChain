#include <iostream>
#include <cstdlib>
#include "chain.h"
#include "block.h"
#include "wallet.h"

using namespace std;
Block coinBase;
Chain blockChain;

Chain::Chain()
{
    coinBase.preBlockHash = "0";
    coinBase.merkleRoot = "0";
    coinBase.difficultyTarget = 0;
    coinBase.height = 1;
    coinBase.nonce = 0;
    end = &coinBase;
}

void Chain::print()
{
    Block *now = end;
    while (now != nullptr)
    {
        now->show();
        now = now->preBlock;
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
    result = Wallet::Base58(now->GetHash());
    return result;
}