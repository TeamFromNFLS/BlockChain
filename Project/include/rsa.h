#ifndef RSA_H
#define RSA_H
#include <iostream>
#include "bigInt.h"
using namespace std;

class RSA
{
public:
    RSA() {}
    ~RSA() {}
    void Init(); //prime & euler
    bool PrimePair(const BigInt &, const BigInt &);
    BigInt CreatePrime(); //create two prime numbers p and q
    BigInt CreateRandom(int);
    void CreatePublic(); //randomly create the public key
    void CreatePrivate();
    BigInt CreateRandomSmaller(const BigInt &); //for the private key and the prime test
    BigInt EncryptByPublic(const BigInt &);
    BigInt EncryptByPrivate(const BigInt &);
    BigInt DecryptByPublic(const BigInt &);
    BigInt DecryptByPrivate(const BigInt &);
    bool IsPrime(const BigInt &, int k = 10); //k随便取的，判断错误率是1/4^k
    const int length = 15;
    BigInt publicKey, product;

private:
    BigInt privateKey, p, q, Euler;
};

#endif
