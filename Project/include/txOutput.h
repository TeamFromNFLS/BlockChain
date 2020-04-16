#ifndef TX_OUTPUT_H
#define TX_OUTPUT_H
#include <string>
#include "wallet.h"
#include "bigInt.h"
#include "rsa.h"
#include "txInput.h"
#include "transaction.h"

class TxOutput
{
public:
    TxOutput(int _value, BigInt receiverPublicKey);
    string getPublicHash();
    int value;
    string publicHash;
};

#endif