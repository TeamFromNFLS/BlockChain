#ifndef WALLET_H
#define WALLET_H
#include <string>
#include <tuple>
#include "bigInt.h"

class Wallet
{
public:
    Wallet(int worker);
    Wallet(){};
    void Init(int worker);
    tuple<BigInt, BigInt, BigInt> GetKey()
    {
        return make_tuple(privateKey, publicKey, n);
    }

protected:
    BigInt privateKey, publicKey, n;
    string address;
    string version = "00";
    string Base58(string s);
};

#endif //WALLET_H