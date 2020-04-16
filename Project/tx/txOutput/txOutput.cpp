#include <string>
#include "sha256.h"
#include "bigInt.h"
#include "rsa.h"
#include "txOutput.h"

TxOutput::TxOutput(int _value, BigInt receiverPublicKey) : value(_value)
{
    //TODO: calculate hash
}

string TxOutput::getPublicHash()
{
    return publicHash;
}