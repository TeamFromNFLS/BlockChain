#ifndef TX_INPUT_H
#define TX_INPUT_H
#include <string>
#include "bigInt.h"
#include "rsa.h"

class TxInput
{
public:
    TxInput(int _value, BigInt _publicKey, BigInt _N)
    {
        value = _value;
        publicKey = _publicKey;
        N = _N;
    }
    TxInput() : value(0), prevID(-1){};
    int GetPrevID() const { return prevID; }
    int GetValue() { return value; }
    void SetPrevID(int ID) { prevID = ID; }
    BigInt signature;
    BigInt publicKey;
    BigInt N;

private:
    int prevID;
    int value;
};

#endif