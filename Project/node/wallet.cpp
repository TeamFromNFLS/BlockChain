#include <iostream>
#include <tuple>
#include <vector>
#include <cmath>
#include "wallet.h"
#include "rsa.h"
#include "bigInt.h"
#include "sha256.h"
#include "ripemd160.h"
using namespace std;

BigInt ToInt(string s)
{
    vector<uint64_t> result;
    int len = s.length();
    uint64_t cnt = 0;
    for (int i = len - 1; i >= 0; --i)
    {
        if (!cnt)
        {
            result.push_back(0);
        }
        uint64_t now;
        if (s[i] >= 'a' && s[i] <= 'z')
        {
            now = s[i] - 'a' + 10;
        }
        else
        {
            now = s[i] - '0';
        }
        result.back() += now * (uint64_t)pow(UINT64_C(16), cnt);
        cnt = (cnt + 1) % 16;
    }
    return BigInt(result);
}

const string Base58String = "123456789ABCDEFGHJKLMNPQRSTUVWXYZabcdefghijkmnopqrstuvwxyz";
string Wallet::Base58(string s)
{
    BigInt x = ToInt(s), base("58");
    string result;
    vector<uint64_t> now;
    while (x > BigInt::zero)
    {
        /*BigInt tmp = x % base;
        cout << tmp << endl;*/
        (x % base).GetNumber(now);
        result += Base58String[now[0]];
        x /= base;
    }
    string::iterator it = s.begin();
    while (*it == '0')
    {
        result += Base58String[0];
        it++;
    }
    reverse(result.begin(), result.end());
    return result;
}

vector<pair<string, string>> Wallet::walletInfo;

void Wallet::Init(int worker)
{
    RSA a;
    a.Init(worker);
    a.CreateKeys();
    tuple<BigInt, BigInt, BigInt> keys;
    keys = a.GetKey();
    publicKey = get<0>(keys);
    privateKey = get<1>(keys);
    n = get<2>(keys);
    string publicKeyHash = rmd160(sha256(publicKey.ToString() + n.ToString()));
    string versionpublicKeyHash = version + publicKeyHash;
    string tailHash = sha256(sha256(versionpublicKeyHash)).substr(0, 4);
    string finalHash = versionpublicKeyHash + tailHash;
    address = Base58(finalHash);
    walletInfo.push_back(make_pair(address, publicKeyHash));
    cout << address << endl;
}

Wallet::Wallet(int worker)
{
    Init(worker);
}

//00c94696460043b120981f922acd5f3bccefe1a5fdd6d4