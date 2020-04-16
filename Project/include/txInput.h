#ifndef TX_INPUT_H
#define TX_INPUT_H
#include <string>
#include "wallet.h"
#include "bigInt.h"
#include "rsa.h"
#include "transaction.h"
#include "txOutput.h"

class TxInput
{
public:
    TxInput(int _value, BigInt _publicKey);
    TxInput(); //coinbase
    void setSignature(BigInt _signature);
    int GetPrevID();
    int GetValue();
    BigInt publicKey;
    BigInt signature;

private:
    int prevID;
    int value;
};

#endif