#ifndef TX_INPUT_H
#define TX_INPUT_H
#include <string>
#include "bigInt.h"
#include "rsa.h"

class TxInput
{
public:
    TxInput(int _value, BigInt _publicKey) : value(_value), publicKey(_publicKey){};
    TxInput(){};
    //void setSignature(BigInt _signature);
    int GetPrevID() { return prevID; }
    int GetValue() { return value; }
    void SetPrevID(int ID) { prevID = ID; }
    BigInt signature;

private:
    int prevID;
    int value;
    BigInt publicKey;
};

#endif