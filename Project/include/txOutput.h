#ifndef TX_OUTPUT_H
#define TX_OUTPUT_H
#include <string>
#include "bigInt.h"
#include "rsa.h"

class TxOutput
{
public:
    TxOutput(){};
    TxOutput(int _value, string _receiverPublicHash) : value(_value), publicHash(_receiverPublicHash){};
    string GetPublicHash() { return publicHash; }
    int GetValue() const { return value; }

private:
    int value;
    string publicHash;
};

#endif