#ifndef RSA_H
#define RSA_H
#include<iostream>
#include"bigInt.h"
using namespace std;

class RSA{
    public:
        RSA(){}
        ~RSA(){}
        void init(); //prime & euler
        BigInt CreatePrime(); //create two prime numbers p and q
        BigInt CreatePublic(const BigInt &); //randomly create the public key
        BigInt CreatePrivate(const BigInt &); //create the private key
        BigInt encrypt(const BigInt &);
        BigInt decrypt(const BigInt &);
        bool IsPrime(const BigInt &);
        BigInt PublicKey;
    private:
        BigInt PrivateKey, p, q, eul;
};

#endif