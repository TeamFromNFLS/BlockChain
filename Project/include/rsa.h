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
    void Init();                            //prime & euler
    BigInt CreatePrime();                   //create two prime numbers p and q
    BigInt CreateOdd();                     //create an odd number (and then test if it is prime)
    BigInt CreatePublic(const BigInt &);    //randomly create the public key
    BigInt CreateRandomNum(const BigInt &); //for the private key and the prime test
    BigInt Encrypt(const BigInt &);
    BigInt Decrypt(const BigInt &);
    bool IsPrime(const BigInt &, const int k = 100); //k随便取的，判断正确率是1/4^k
    const int length = 10;
    BigInt PublicKey;

private:
    BigInt PrivateKey, p, q, eul;
};

#endif