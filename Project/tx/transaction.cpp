#include "transaction.h"

int Transaction::mineReward = 10; //to be set
vector<Transaction> Transaction::txPool;
vector<Transaction> Transaction::toBePackedTx;
