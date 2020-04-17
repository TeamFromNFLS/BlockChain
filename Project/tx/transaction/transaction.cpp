#include "txInput.h"
#include "txOutput.h"
#include "transaction.h"
#include "bigInt.h"
#include <string>
#include <iostream>
using namespace std;

vector<Transaction> Transaction::txPool;
vector<Transaction> Transaction::toBePackedTx;
int Transaction::mineReward = 5; //to be set
