#include <random>
#include <vector>
#include "miner.h"
#include "block.h"
#include "chain.h"

using namespace std;

vector<Miner *> Miner::minerSet;

Miner::Miner(int worker)
{
    cout << "Creating a new miner, please wait..." << endl;
    Init(worker);
    mt19937 rng;
    random_device randev;
    rng.seed(randev());
    uniform_int_distribution<int> num(0, INT32_MAX);
    nonce = num(rng);
    Miner::minerSet.push_back(this);
}

Miner::Miner()
{
    cout << "Creating a new miner, please wait..." << endl;
    mt19937 rng;
    random_device randev;
    rng.seed(randev());
    uniform_int_distribution<int> num(0, INT32_MAX);
    nonce = num(rng);
    difficultyTarget = blockChain.GetDifficulty();
    Miner::minerSet.push_back(this);
}

bool Miner::TestPoW(int nonce)
{
    stored.SetNonce(nonce);
    int len = difficultyTarget.length();
    string hash = stored.GetHash();
    string check = hash.substr(0, len);
    if (check == difficultyTarget)
    {
        return true;
    }
    return false;
}

void Miner::PoW(vector<Transaction> &vec)
{
    int now = nonce;
    Block tmp(now, difficultyTarget, vec);
    stored = tmp;
    while (true)
    {
        if (TestPoW(now))
        {
            stored.Pack();
            stored.Show();
            break;
        }
        now = (now + 1) % INT32_MAX;
    }
}
bool Miner::Check(const Block &toCheck)
{
    return true;
}
