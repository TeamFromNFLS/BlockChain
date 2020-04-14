#ifndef WALLET_H
#define WALLET_H
#include <string>
#include "bigInt.h"

class Wallet
{
public:
    Wallet(int worker);

private:
    BigInt privateKey, publicKey, n;
    string address;
    string version = "00";
    string Base58(string s);
};

#endif //WALLET_H