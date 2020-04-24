#include <iostream>
#include <cstdio>
#include <cstring>
#include <random>
#include <ctime>
#include <thread>
#include <mutex>
#include <vector>
#include "rsa.h"
#include "bigInt.h"
#include "wallet.h"
#include "miner.h"
#include "sha256.h"
#include "ripemd160.h"
#include "block.h"
#include "chain.h"
#include "test.h"
using namespace std;

/* Assume transactions are packed immediately after they are constructed.
Packed transactions are stored in Transaction::packedTx */

int main()
{
     /* Wallet a, b;
     BigInt pub("0x0000000000010001");
     BigInt aPri("0x289bbabe0d6e22aaca299054e040f94b6fb93beb0a32657e5f44f83b84a09ed0e313ce830b938a6c1730612647d216c6023749a30285b636c23f5aa3d17be25ac61b358e329723466278d34d248aadcc91bf612d3d165be8f014ae012e37741c00dda187ad75c72a0ea6987c53bf74d882ba1c49f54f8ca0c9a42278b04c4a01");
     BigInt aN("0x9ede10881247a4722307fc106417dcfb00ce0dc1fd06d76c63cf752078ba5e289e062311c23d7694af80bd1b4709067b1ffd3f3d8eae77c14a6c8398065a483ce0d072436cc143d4216dcbc7a1f9e13ff9906d69f21ee4a6f6671c2e821ce5283b2e49f02ae707119e1fe931c5374a45c1d14379d519e133ee0ebde03fb1ee8d");
     string aAddr = "11V6pyWsBYr2N8i99ifnVjYaWb7d77oZ";

     BigInt bPri("0x67459425562cb6983f7cfaafeab65d8fd98edf448fe4012fa3eb76ff52641d7a4c8417a2bf96f7b60b5b7012512f98221aab9d63b13616413797f24c94780cf38e98c4bb21b0ee8abd3ec00fa711f248029860036c3817b67f0d37e004be285ff5724e1ad24d943c7a8c1aae44cebc348abdac65865ed9aa41a0ab7b49a760f9");
     BigInt bN("0xcfc1fa494e068747c948cefeaacf7218af12af88cc723491bbac40de5e980696a8cca9b2e768bce3e22e7832de6bb75f05657d8ffa0aaad98c51b5b63369fafd884ccf2d46f5738e2f99aa37022ffaeb6e536a0c5d0b133d96b1368e33947831e9bd799435d781b061fa158bfbe848b575a5e230cdedb634583353d925b9409f");
     string bAddr = "112383bH5cpA7tz86wuScJVwTRyrhMHFP";

     a.SetWallet(pub, aPri, aN, aAddr);
     b.SetWallet(pub, bPri, bN, bAddr);

     /* cout << get<1>(Wallet::walletInfo[0]) << get<1>(Wallet::walletInfo[1]) << endl;
     string signStr = "0x" + get<1>(Wallet::walletInfo[0]) + get<1>(Wallet::walletInfo[1]);
     BigInt signInfo(signStr);
     cout << signInfo << endl;
     BigInt encrypted = RSA::EncryptAndDecrypt(signInfo, aPri, aN);
     BigInt decrypted = RSA::EncryptAndDecrypt(encrypted, pub, aN);
     bool IsSame = (decrypted == signInfo);
     cout << encrypted << endl
          << IsSame << endl; */
     Wallet a(1), b(1), c(1);
     b.CreateCoinbase();
     c.CreateCoinbase();
     b.CreateTransaction(Wallet::walletInfo[0], Transaction::mineReward);
     c.CreateTransaction(Wallet::walletInfo[0], Transaction::mineReward);
     a.CreateCoinbase();
     a.FindBalance();

     for (Transaction &tx : Transaction::packedTx)
     {
          Wallet::VerifyTx(tx);
     }
     auto st = clock();
     freopen("ans.txt", "w", stdout);
     balance();
     TestTx();
     TestMine();
     auto ed = clock();
     cout << "Total Time: " << ed - st << endl;
     return 0;
}
