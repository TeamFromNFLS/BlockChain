#include <sys/time.h>
#include <unistd.h>
#include "rsa.h"
#include "bigInt.h"
#include "wallet.h"
#include "miner.h"
#include "sha256.h"
#include "ripemd160.h"
#include "block.h"
#include "chain.h"
#include "test.h"
using namespace std;

/* Assume transactions are packed immediately after they are constructed.
Packed transactions are stored in Transaction::packedTx */

int main()
{
     //freopen("ans.txt", "w", stdout);
     struct timeval timeStart, timeEnd;
     double runtime = 0;
     gettimeofday(&timeStart, NULL);
     balance();
     //TestMine();
     gettimeofday(&timeEnd, NULL);
     runtime = (timeEnd.tv_sec - timeStart.tv_sec) + (double)(timeEnd.tv_usec - timeStart.tv_usec) / 1000000;
     cout << "Total time: " << runtime << "s." << endl;
     //fclose(stdout);
     return 0;
}
