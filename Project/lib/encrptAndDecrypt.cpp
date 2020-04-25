#include <iostream>
#include <cstdio>
#include <cstring>
#include <random>
#include <ctime>
#include <thread>
#include <mutex>
#include <vector>
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

int EncryptAndDecrypt()
{
    BigInt Private("0x289bbabe0d6e22aaca299054e040f94b6fb93beb0a32657e5f44f83b84a09ed0e313ce830b938a6c1730612647d216c6023749a30285b636c23f5aa3d17be25ac61b358e329723466278d34d248aadcc91bf612d3d165be8f014ae012e37741c00dda187ad75c72a0ea6987c53bf74d882ba1c49f54f8ca0c9a42278b04c4a01");
    BigInt Public("0x0000000000010001");
    BigInt N("0x9ede10881247a4722307fc106417dcfb00ce0dc1fd06d76c63cf752078ba5e289e062311c23d7694af80bd1b4709067b1ffd3f3d8eae77c14a6c8398065a483ce0d072436cc143d4216dcbc7a1f9e13ff9906d69f21ee4a6f6671c2e821ce5283b2e49f02ae707119e1fe931c5374a45c1d14379d519e133ee0ebde03fb1ee8d");
    BigInt data;
    while (cin >> data)
    {
        if (data == BigInt::zero)
        {
            break;
        }
        BigInt encrypt = RSA::EncryptAndDecrypt(data, Private, N);
        BigInt decrypt = RSA::EncryptAndDecrypt(encrypt, Public, N);
        bool isSame = (data == decrypt);
        cout << isSame << endl;
    }
    return 0;
}
