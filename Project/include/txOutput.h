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
    TxOutput() {}
    TxOutput(int _value, string _receiverPublicHash) : value(_value), publicHash(_receiverPublicHash) {}
    string GetPublicHash() { return publicHash; }
    int GetValue() { return value; }

private:
    int value;
    string publicHash;
};

#endif