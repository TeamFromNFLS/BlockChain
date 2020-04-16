#include <string>
#include "wallet.h"
#include "bigInt.h"
#include "rsa.h"
#include "txInput.h"
#include "transaction.h"

TxInput::TxInput(int _value, BigInt _publicKey) : value(_value), publicKey(_publicKey) {}

TxInput::TxInput() : prevID(-1), value(0) {}

void TxInput::setSignature(BigInt _signature)
{
    signature = _signature;
}

int TxInput::GetPrevID()
{
    return prevID;
}

int TxInput::GetPrevID()
{
    return value;
}
