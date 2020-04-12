#ifndef RSA_H
#define RSA_H
#include <iostream>
#include <mutex>
#include "bigInt.h"
using namespace std;

class KeyPair
{
public:
    KeyPair(BigInt _exponent, BigInt _mod) : exponent(_exponent), mod(_mod){};

private:
    BigInt exponent, mod;
};

class RSA
{
public:
    RSA() {}
    ~RSA() {}
    void Init(int worker);                                            //prime & euler & product
    BigInt CreatePrime(int worker);                                   //create two prime numbers p and q
    void PrimeWorker(mutex *mutex, bool *finishFlag, BigInt *result); // multi-thread
    BigInt CreateRandom(int);
    void CreateKeys();                          //use ext gcd to create public and private key
    BigInt CreateRandomSmaller(const BigInt &); //for the private key and the prime test
    BigInt EncryptByPublic(const BigInt &);
    BigInt EncryptByPrivate(const BigInt &);
    BigInt DecryptByPublic(const BigInt &);
    BigInt DecryptByPrivate(const BigInt &);
    bool IsPrime(const BigInt &, int k = 4); //k随便取的，判断错误率是1/4^k
    int Sieve(vector<BigInt> &, const BigInt &, int);
    //const int length = 305;
    BigInt publicKey, privateKey, Euler, product, p, q;
    //KeyPair public, private;
    void setNumber(BigInt, BigInt);

private:
    // BigInt privateKey,
    //     Euler;
};

#endif
