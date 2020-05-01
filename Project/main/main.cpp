#include <sys/time.h>
#include <unistd.h>
#include <set>
#include <sstream>
#include <thread>
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

void WalletCreator(Wallet *wallet);
const int INF = 0x3f3f3f3f;
set<string> commendSet{"demo", "help", "add", "delete", "mine", "find", "make", "check", "exit"};

string EditDistance(string &s)
{
     int dp[15][15];
     int now = INF;
     string result;
     string check = s;
     check.insert(0, "0");
     int lenCheck = check.length() - 1;
     set<string>::iterator it;
     for (it = commendSet.begin(); it != commendSet.end(); ++it)
     {
          string commend = *it;
          commend.insert(0, "0");
          int lenCommend = commend.length() - 1;
          for (int i = 1; i <= lenCheck; ++i)
          {
               for (int j = 1; j <= lenCommend; ++j)
               {
                    dp[i][j] = INF;
               }
          }
          for (int i = 1; i <= lenCheck; ++i)
          {
               dp[i][0] = i;
          }
          for (int j = 1; j <= lenCommend; ++j)
          {
               dp[0][j] = j;
          }
          for (int i = 1; i <= lenCheck; ++i)
          {
               for (int j = 1; j <= lenCommend; ++j)
               {
                    bool flag = check[i] != commend[j];
                    dp[i][j] = min(dp[i - 1][j] + 1, min(dp[i][j - 1] + 1, dp[i - 1][j - 1] + flag));
               }
          }
          if (dp[lenCheck][lenCommend] <= now)
          {
               result = *it;
               now = dp[lenCheck][lenCommend];
          }
     }
     return result;
}

int main()
{
     struct timeval timeStart, timeEnd;
     cout << "Welcome to Prometheus, a blockchain simulation program." << endl
          << "For bug reporting instructions, please see:" << endl
          << "N/A" << endl
          << "For help, type \"help\"." << endl;
     string cmd;
     while (getline(cin, cmd))
     {
          cmd.insert(cmd.end(), ' ');
          vector<string> cmdList;
          int pos = 0;
          for (int i = 0; i < cmd.length(); ++i)
          {
               if (cmd[i] == ' ')
               {
                    cmdList.push_back(cmd.substr(pos, i - pos));
                    pos = i + 1;
               }
          }
          cmd.erase(cmd.end() - 1);
          set<string>::iterator it = commendSet.find(cmdList[0]);
          if (it != commendSet.end())
          {
               if (*it == "demo")
               {
                    freopen("log.txt", "w", stdout);
                    cout << "Run demo..." << endl;
                    double runtime = 0;
                    gettimeofday(&timeStart, NULL);
                    //balance();
                    gettimeofday(&timeEnd, NULL);
                    runtime = (timeEnd.tv_sec - timeStart.tv_sec) + (double)(timeEnd.tv_usec - timeStart.tv_usec) / 1000000;
                    cout << "Total time: " << runtime << "s." << endl;
                    fclose(stdout);
                    cout << "Demo exited normally. Please check \"log.txt\" for details." << endl;
                    continue;
               }
               if (*it == "help")
               {
                    if (cmdList.size() == 1)
                    {
                         cout << "List of classes of commands:" << endl
                              << "demo -- run demo." << endl
                              << "add -- add a wallet or miner." << endl
                              << "delete -- delete a wallet or miner." << endl
                              << "mine -- use miners to pack transactions into blockchain." << endl
                              << "find -- find the balance of a wallet." << endl
                              << "make -- make a transaction." << endl
                              << "check -- check whether the wallet contains a right blockchain." << endl
                              << "exit -- exit the program." << endl
                              << endl
                              << "Type \"help\" followed by command name for full documentation." << endl;
                    }
                    else
                    {
                         it = commendSet.find(cmdList[1]);
                         if (it == commendSet.end())
                         {
                              string guess = EditDistance(cmdList[1]);
                              cout << "Undefined command: \"" << cmdList[1] << "\". Do you mean \"" << guess << "\" ?" << endl;
                         }
                         else
                         {
                              if (*it == "demo")
                              {
                                   cout << "Run a demo which shows how this program works with pictures and a log file." << endl;
                              }
                              if (*it == "add")
                              {
                                   cout << "add [wallet/miner] [number of nodes]: add some nodes to the net." << endl;
                              }
                              if (*it == "delete")
                              {
                                   cout << "delete [wallet/miner]: delete the last wallet or miner which is added into the net." << endl;
                              }
                              if (*it == "help")
                              {
                                   cout << "NO MATRYOSHKA!" << endl;
                              }
                              if (*it == "find")
                              {
                                   cout << "find [wallet address]: find the remainder of a wallet and related transactions." << endl;
                              }
                              if (*it == "mine")
                              {
                                   cout << "Only packed transactions could be avalidable for spending. This commend would ask all the miners in the net to try to pack transactions." << endl;
                              }
                              if (*it == "make")
                              {
                                   cout << "make [walletA address] [walletB address]: make a transaction from walletA to walletB." << endl;
                              }
                              if (*it == "check")
                              {
                                   cout << "check [wallet address]: check whether the blockchain in this wallet is true." << endl;
                              }
                              if (*it == "exit")
                              {
                                   cout << "Exit the program." << endl;
                              }
                         }
                    }
                    continue;
               }
               if (*it == "add")
               {
                    if (cmdList.size() != 3)
                    {
                         cout << "Ambiguous commend "
                              << "\"" << cmd << "\":"
                              << "add [wallet/miner] [number of nodes]." << endl;
                         continue;
                    }
                    stringstream ss(cmdList[2]);
                    int t;
                    char p;
                    if (cmdList[1] != "wallet" && cmdList[1] != "miner")
                    {
                         cout << "Ambiguous commend "
                              << "\"" << cmd << "\":"
                              << "add [wallet/miner] [number of nodes]." << endl;
                         continue;
                    }
                    else if (!(ss >> t) || ss >> p)
                    {
                         cout << "Ambiguous commend "
                              << "\"" << cmd << "\":"
                              << "add [wallet/miner] [number of nodes]." << endl;
                         continue;
                    }
                    else
                    {
                         freopen("log.txt", "w", stdout);
                         vector<Wallet *> wallets;
                         vector<thread> threads;
                         Wallet *tmp;
                         int loop = t / 8;
                         int remainder = t - loop * 8;
                         for (int i = 0; i < loop; ++i)
                         {
                              for (int j = 0; j < 8; ++j)
                              {
                                   tmp = new Wallet;
                                   wallets.push_back(tmp);
                                   threads.emplace_back(WalletCreator, tmp);
                              }
                         }
                         for (int i = 0; i < remainder; ++i)
                         {
                              tmp = new Wallet;
                              wallets.push_back(tmp);
                              threads.emplace_back(WalletCreator, tmp);
                         }
                         for (auto &t : threads)
                         {
                              if (t.joinable())
                              {
                                   t.join();
                              }
                         }
                         vector<Wallet *> newWallets;
                         for (int i = 0; i < t; ++i)
                         {
                              for (int j = 0; j < t; ++j)
                              {
                                   if (Wallet::walletInfo[i].first == wallets[j]->GetAddress())
                                   {
                                        newWallets.push_back(wallets[j]);
                                   }
                              }
                         }
                         fclose(stdout);
                         cout << "Add wallet complete." << endl;
                    }
               }
               if (*it == "exit")
               {
                    break;
               }
          }
          else
          {
               string guess = EditDistance(cmdList[0]);
               cout << "Undefined command: \"" << cmdList[0] << "\". Do you mean \"" << guess << "\" ?" << endl
                    << "Try \"help\"." << endl;
          }
     }
     return 0;
}
