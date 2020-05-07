#include <sys/time.h>
#include <unistd.h>
#include <set>
#include <sstream>
#include <fstream>
#include <thread>
#include <exception>
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

void Output(string s)
{
     string::iterator it;
     for (it = s.begin(); it != s.end(); ++it)
     {
          cout << *it << flush;
          usleep(20000);
     }
     cout << endl;
}

void WalletCreator(mutex *mutex, Wallet *wallet)
{
     mutex->lock();
     wallet->InitPrint();
     mutex->unlock();
     wallet->Init(1);
}

bool IsInt(string &s)
{
     stringstream ss(s);
     int t;
     char p;
     if (!(ss >> t))
     {
          return false;
     }
     if (ss >> p)
     {
          return false;
     }
     else
     {
          return true;
     }
}

const int INF = 0x3f3f3f3f;
set<string> commandSet{"demo", "help", "add", "delete", "mine", "find", "make", "display", "clean", "log", "exit"};

string EditDistance(string &s)
{
     int dp[15][15];
     int now = INF;
     string result;
     string check = s;
     check.insert(0, "0");
     int lenCheck = check.length() - 1;
     set<string>::iterator it;
     for (it = commandSet.begin(); it != commandSet.end(); ++it)
     {
          string command = *it;
          command.insert(0, "0");
          int lenCommand = command.length() - 1;
          for (int i = 1; i <= lenCheck; ++i)
          {
               for (int j = 1; j <= lenCommand; ++j)
               {
                    dp[i][j] = INF;
               }
          }
          for (int i = 1; i <= lenCheck; ++i)
          {
               dp[i][0] = i;
          }
          for (int j = 1; j <= lenCommand; ++j)
          {
               dp[0][j] = j;
          }
          for (int i = 1; i <= lenCheck; ++i)
          {
               for (int j = 1; j <= lenCommand; ++j)
               {
                    bool flag = check[i] != command[j];
                    dp[i][j] = min(dp[i - 1][j] + 1, min(dp[i][j - 1] + 1, dp[i - 1][j - 1] + flag));
               }
          }
          if (dp[lenCheck][lenCommand] <= now)
          {
               result = *it;
               now = dp[lenCheck][lenCommand];
          }
     }
     return result;
}

inline void Clean()
{
     fstream clean("log.txt", ios::out);
}

int main()
{
     ofstream fileOut;
     ifstream fileIn, LogIn;
     fileOut.open("log.txt", ios::app);
     fileIn.open("in.txt", ios::in);
     LogIn.open("log.txt", ios::in);
     streambuf *coutBackup, *fileBackup, *cinBackup, *inBackup, *logBackup;
     coutBackup = cout.rdbuf();
     cinBackup = cin.rdbuf();
     fileBackup = fileOut.rdbuf();
     inBackup = fileIn.rdbuf();
     logBackup = LogIn.rdbuf();
     struct timeval timeStart, timeEnd;
     Output("Welcome to P.R.O.M.E.T.H.E.U.S, a blockchain simulation program.");
     Output("For bug reporting instructions, please send issue to:");
     Output("https://github.com/TeamFromNFLS/BlockChain");
     Output("For help, type \"help\".");
     string cmd;
     bool demoFlag = false;
     //Clean();
     //cin.rdbuf(inBackup);
     while (getline(cin, cmd))
     {
          if (demoFlag)
          {
               Clean();
               demoFlag = false;
          }
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
          set<string>::iterator it = commandSet.find(cmdList[0]);
          if (it != commandSet.end())
          {
               if (*it == "demo")
               {
                    Clean();
                    Output("Run demo...");
                    cout.rdbuf(fileBackup);
                    double runtime = 0;
                    gettimeofday(&timeStart, NULL);
                    balance();
                    gettimeofday(&timeEnd, NULL);
                    runtime = (timeEnd.tv_sec - timeStart.tv_sec) + (double)(timeEnd.tv_usec - timeStart.tv_usec) / 1000000;
                    cout << "Total time: " << runtime << "s." << endl;
                    cout.rdbuf(coutBackup);
                    Output("Demo exited normally. Please check \"log.txt\" for details.");
                    Output("Log would be cleaned after next command.");
                    demoFlag = true;
                    cin.rdbuf(logBackup);
                    string line;
                    while (getline(cin, line))
                    {
                         Output(line);
                    }
                    Output("Over.");
                    cin.rdbuf(cinBackup);
                    continue;
               }
               if (*it == "help")
               {
                    if (cmdList.size() == 1)
                    {
                         Output("List of classes of commands:");
                         Output("demo -- run demo.");
                         Output("add -- add a wallet or miner.");
                         Output("delete -- delete a wallet or miner.");
                         Output("mine -- use miners to pack transactions into blockchain.");
                         Output("find -- find the balance of a wallet.");
                         Output("make -- make a transaction.");
                         Output("display -- print the blockchain in a wallet.");
                         Output("clean -- clean the log.");
                         Output("exit -- exit the program.");
                         cout << endl;
                         Output("Type \"help\" followed by command name for full documentation.");
                    }
                    else
                    {
                         it = commandSet.find(cmdList[1]);
                         if (it == commandSet.end())
                         {
                              string guess = EditDistance(cmdList[1]);
                              Output("Undefined command: \"" + cmdList[1] + "\". Do you mean \"" + guess + "\" ?");
                         }
                         else
                         {
                              if (*it == "demo")
                              {
                                   Output("Run a demo which shows how this program works with pictures and a log file.");
                              }
                              if (*it == "add")
                              {
                                   Output("add [wallet/miner] [number of nodes]: add some nodes to the net. Each would be given a unique address and id.");
                              }
                              if (*it == "delete")
                              {
                                   Output("delete [wallet/miner]: delete the last wallet or miner which is added into the net.");
                              }
                              if (*it == "help")
                              {
                                   Output("NO MATRYOSHKA!");
                              }
                              if (*it == "find")
                              {
                                   Output("find [wallet id]: find the remainder of a wallet and related transactions.");
                              }
                              if (*it == "mine")
                              {
                                   Output("Only packed transactions could be avalidable for spending. This command would ask all the miners in the net to try to pack transactions.");
                              }
                              if (*it == "make")
                              {
                                   Output("make [walletA id] [walletB id] [value]: make a transaction from walletA to walletB.");
                                   Output("Especially, setting the first id to -1 means that this is a reward from coinBase.");
                              }
                              if (*it == "display")
                              {
                                   Output("display [wallet id]: display the blockchain in this wallet.");
                              }
                              if (*it == "clean")
                              {
                                   Output("Delete everything in log.");
                              }
                              if (*it == "log")
                              {
                                   Output("Display everything in log onto the screen.");
                              }
                              if (*it == "exit")
                              {
                                   Output("Exit the program.");
                              }
                         }
                    }
                    continue;
               }
               if (*it == "add")
               {
                    try
                    {
                         if (cmdList.size() != 3)
                         {
                              throw false;
                         }
                         if (cmdList[1] != "wallet" && cmdList[1] != "miner")
                         {
                              throw false;
                         }
                         if (!IsInt(cmdList[2]))
                         {
                              throw false;
                         }
                         else
                         {
                              int t = atoi(cmdList[2].c_str());
                              int loop = t / 8;
                              int remainder = t - loop * 8;
                              string const start[2] = {"adding wallets, please wait...", "adding miners, please wait..."};
                              string const end[2] = {"Add wallets complete.", "Add miners complete."};
                              bool flag = cmdList[1] == "miner";
                              Output(start[flag]);
                              cout.rdbuf(fileBackup);
                              vector<thread> threads;
                              Wallet *tmp;
                              mutex mutex;
                              for (int i = 0; i < loop; ++i)
                              {
                                   for (int j = 0; j < 8; ++j)
                                   {
                                        tmp = flag ? new Miner : new Wallet;
                                        threads.emplace_back(WalletCreator, &mutex, tmp);
                                   }
                              }
                              for (int i = 0; i < remainder; ++i)
                              {
                                   tmp = flag ? new Miner : new Wallet;
                                   threads.emplace_back(WalletCreator, &mutex, tmp);
                              }
                              for (auto &t : threads)
                              {
                                   if (t.joinable())
                                   {
                                        t.join();
                                   }
                              }
                              cout.rdbuf(coutBackup);
                              Output(end[flag]);
                              continue;
                         }
                    }
                    catch (bool)
                    {
                         Output("Ambiguous command \"" + cmd + "\": add [wallet/miner] [number of nodes].");
                         continue;
                    }
               }
               if (*it == "delete")
               {
                    if (cmdList[1] != "wallet" && cmdList[1] != "miner")
                    {
                         Output("Ambiguous command \"" + cmd + "\": delete [wallet/miner].");
                         continue;
                    }
                    string address;
                    if (cmdList[1] == "wallet")
                    {
                         try
                         {
                              if (Wallet::onlyWalletSet.empty())
                              {
                                   throw false;
                              }
                              address = Wallet::onlyWalletSet[Wallet::onlyWalletSet.size() - 1].GetAddress();
                              Wallet::onlyWalletSet.erase(Wallet::onlyWalletSet.end() - 1);
                              Output("Wallet: " + address + " has been deleted.");
                         }
                         catch (bool)
                         {
                              Output("No wallet in the net!");
                              continue;
                         }
                    }
                    if (cmdList[1] == "miner")
                    {
                         try
                         {
                              if (Miner::minerSet.empty())
                              {
                                   throw false;
                              }
                              address = Miner::minerSet[Miner::minerSet.size() - 1]->GetAddress();
                              Miner::minerSet.erase(Miner::minerSet.end() - 1);
                              Output("Miner: " + address + " has been deleted.");
                         }
                         catch (bool)
                         {
                              Output("No miner in the net!");
                              continue;
                         }
                    }
                    vector<Wallet>::iterator it;
                    for (it = Wallet::walletSet.begin(); it != Wallet::walletSet.end(); ++it)
                    {
                         if (it->GetAddress() == address)
                         {
                              Wallet::walletSet.erase(it);
                              break;
                         }
                    }
                    vector<pair<string, string>>::iterator _it;
                    for (_it = Wallet::walletInfo.begin(); _it != Wallet::walletInfo.end(); ++_it)
                    {
                         if (_it->first == address)
                         {
                              Wallet::walletInfo.erase(_it);
                              break;
                         }
                    }
                    continue;
               }
               if (*it == "make")
               {
                    try
                    {
                         if (cmdList.size() != 4)
                         {
                              throw false;
                         }
                         if (!(IsInt(cmdList[1]) && IsInt(cmdList[2]) && IsInt(cmdList[3])))
                         {
                              throw false;
                         }
                         int a = atoi(cmdList[1].c_str()), b = atoi(cmdList[2].c_str()), value = atoi(cmdList[3].c_str());
                         if (max(a, b) >= Wallet::walletSet.size())
                         {
                              throw "false";
                         }
                         cout.rdbuf(fileBackup);
                         if (a == -1)
                         {
                              Wallet::walletSet[b].CreateCoinbase(value);
                              cout.rdbuf(coutBackup);
                              Output("Transaction made.");
                              continue;
                         }
                         bool flag = Wallet::walletSet[a].CreateTransaction(Wallet::walletInfo[b], value);
                         cout.rdbuf(coutBackup);
                         if (!flag)
                         {
                              throw 0;
                         }
                         Output("Transaction made.");
                         continue;
                    }
                    catch (bool)
                    {
                         Output("Ambiguous command \"" + cmd + "\": make [walletA id] [walletB id] [value].");
                         continue;
                    }
                    catch (char const *)
                    {
                         stringstream ss;
                         ss << Wallet::walletSet.size();
                         Output("Wallet id cannot find. ONLY " + ss.str() + " nodes exists.");
                         continue;
                    }
                    catch (int)
                    {
                         Output("Transaction construction failed.");
                         continue;
                    }
               }
               if (*it == "clean")
               {
                    Clean();
                    continue;
               }
               if (*it == "mine")
               {
                    try
                    {
                         if (cmdList.size() != 1)
                         {
                              throw "false";
                         }
                         if (Transaction::toBePackedTx.empty())
                         {
                              throw false;
                         }
                         if (Miner::minerSet.empty())
                         {
                              throw 0;
                         }
                         cout.rdbuf(fileBackup);
                         int output = Mine();
                         cout.rdbuf(coutBackup);
                         Output("Found miner id: " + output);
                         Output("Found miner address: " + Miner::minerSet[output]->GetAddress());
                    }
                    catch (bool)
                    {
                         Output("All transactions have already been packed.");
                         continue;
                    }
                    catch (int)
                    {
                         Output("No miner in the net!");
                         continue;
                    }
                    catch (char const *)
                    {
                         Output("Ambiguous command \"" + cmd + "\": mine");
                         continue;
                    }
               }
               if (*it == "display")
               {
                    try
                    {
                         if (cmdList.size() != 2)
                         {
                              throw false;
                         }
                         if (!IsInt(cmdList[1]))
                         {
                              throw false;
                         }
                         int a = atoi(cmdList[1].c_str());
                         if (a >= Wallet::walletSet.size())
                         {
                              throw 0;
                         }
                         cout.rdbuf(fileBackup);
                         Wallet::walletSet[a].GetChain().Print();
                         cout.rdbuf(coutBackup);
                         Output("Print over. Please check the log.");
                         continue;
                    }
                    catch (bool)
                    {
                         Output("Ambiguous command \"" + cmd + "\": display [wallet id].");
                         continue;
                    }
                    catch (int)
                    {
                         stringstream ss;
                         ss << Wallet::walletSet.size();
                         Output("Wallet id cannot find. ONLY " + ss.str() + " nodes exists.");
                         continue;
                    }
               }
               if (*it == "find")
               {
                    try
                    {
                         if (cmdList.size() != 2)
                         {
                              throw false;
                         }
                         if (!IsInt(cmdList[1]))
                         {
                              throw false;
                         }
                         int id = atoi(cmdList[1].c_str());
                         if (id >= Wallet::walletSet.size())
                         {
                              throw 0;
                         }
                         cout.rdbuf(fileBackup);
                         int remainder = Wallet::walletSet[id].FindBalance();
                         cout.rdbuf(coutBackup);
                         stringstream ss;
                         ss << dec << remainder;
                         Output("Money in wallet " + Wallet::walletSet[id].GetAddress() + " : " + ss.str());
                         continue;
                    }
                    catch (bool)
                    {
                         cout << "Ambiguous command "
                              << "\"" << cmd << "\":"
                              << "find [wallet id]." << endl;
                         continue;
                    }
                    catch (int)
                    {
                         stringstream ss;
                         ss << Wallet::walletSet.size();
                         Output("Wallet id cannot find. ONLY " + ss.str() + " nodes exists.");
                         continue;
                    }
               }
               if (*it == "log")
               {
                    try
                    {
                         if (cmdList.size() != 1)
                         {
                              throw false;
                         }
                         cin.rdbuf(logBackup);
                         string line;
                         bool flag = false;
                         while (getline(cin, line))
                         {
                              flag = true;
                              Output(line);
                         }
                         cin.rdbuf(cinBackup);
                         if (!flag)
                         {
                              throw 0;
                         }
                         Output("Over.");
                         continue;
                    }
                    catch (bool)
                    {
                         Output("Ambiguous command \"" + cmd + "\": log");
                         continue;
                    }
                    catch (int)
                    {
                         Output("The log is empty!");
                         continue;
                    }
               }
               if (*it == "exit")
               {
                    fileOut.close();
                    break;
               }
          }
          else
          {
               string guess = EditDistance(cmdList[0]);
               Output("Undefined command: \"" + cmdList[0] + "\". Do you mean \"" + guess + "\" ?");
               Output("Try \"help\".");
          }
     }
     return 0;
}
