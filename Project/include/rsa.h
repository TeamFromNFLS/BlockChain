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
    void Init();          //prime & euler & product
    BigInt CreatePrime(); //create two prime numbers p and q
    BigInt CreateRandom(int);
    void CreateKeys();                          //use ext gcd to create public and private key
    BigInt CreateRandomSmaller(const BigInt &); //for the private key and the prime test
    BigInt EncryptByPublic(const BigInt &);
    BigInt EncryptByPrivate(const BigInt &);
    BigInt DecryptByPublic(const BigInt &);
    BigInt DecryptByPrivate(const BigInt &);
    bool IsPrime(const BigInt &, int k = 10); //k随便取的，判断错误率是1/4^k
    //const int length = 305;
    BigInt publicKey, privateKey, Euler, product, p, q, length;
    void setNumber(BigInt, BigInt);

private:
    // BigInt privateKey,
    //     Euler;
};

#endif
