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
#include "command.h"

using namespace std;

ofstream fileOut;
ifstream logIn, fileIn;
streambuf *coutBackup, *fileBackup, *cinBackup, *inBackup, *logBackup;
struct timeval timeStart, timeEnd;

void Output(string s)
{
    string::iterator it;
    for (it = s.begin(); it != s.end(); ++it)
    {
        cout << *it << flush;
        usleep(2000);
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
set<string> commandSet{"help", "add", "delete", "mine", "find", "make", "display", "clean", "log", "exit"};

COMMAND CommandSelect(string s)
{
    if (s == "help")
        return HELP;
    if (s == "add")
        return ADD;
    if (s == "delete")
        return DELETE;
    if (s == "mine")
        return MINE;
    if (s == "find")
        return FIND;
    if (s == "make")
        return MAKE;
    if (s == "display")
        return DISPLAY;
    if (s == "clean")
        return CLEAN;
    if (s == "log")
        return LOG;
    if (s == "exit")
        return EXIT;
    else
        return ERROR;
}

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

void Init()
{
    fileOut.open("log.txt", ios::app);
    logIn.open("log.txt", ios::in);
    coutBackup = cout.rdbuf();
    cinBackup = cin.rdbuf();
    fileBackup = fileOut.rdbuf();
    logBackup = logIn.rdbuf();
    Output("Welcome to P.R.O.M.E.T.H.E.U.S, a blockchain simulation program.");
    Output("For bug reporting instructions, please send issue to:");
    Output("https://github.com/TeamFromNFLS/BlockChain");
    Output("For help, type \"help\".");
    Clean();
}

bool Work(string cmd)
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
    switch (CommandSelect(cmdList[0]))
    {

    case HELP:
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
            switch (CommandSelect(cmdList[1]))
            {
            case ADD:
                Output("add [wallet/miner] [number of nodes]: add some nodes to the net. Each would be given a unique address and id.");
                break;
            case DELETE:
                Output("delete [wallet/miner]: delete the last wallet or miner which is added into the net.");
                break;
            case HELP:
                Output("NO MATRYOSHKA!");
                break;
            case FIND:
                Output("find [wallet id]: find the remainder of a wallet and related transactions.");
                break;
            case MINE:
                Output("Only packed transactions could be avalidable for spending. This command would ask all the miners in the net to try to pack transactions.");
                break;
            case MAKE:
                Output("make [walletA id] [walletB id] [value]: make a transaction from walletA to walletB.");
                Output("Especially, setting the first id to -1 means that this is a reward from coinBase.");
                break;
            case DISPLAY:
                Output("display [wallet id]: display the blockchain in this wallet.");
                break;
            case CLEAN:
                Output("Delete everything in log.");
                break;
            case LOG:
                Output("Display everything in log onto the screen.");
                break;
            case EXIT:
                Output("Exit the program.");
                break;
            default:
            {
                string guess = EditDistance(cmdList[1]);
                Output("Undefined command: \"" + cmdList[1] + "\". Do you mean \"" + guess + "\" ?");
            }
            break;
            }
        }
        break;
    }

    case ADD:
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
            }
        }
        catch (bool)
        {
            Output("Ambiguous command \"" + cmd + "\": add [wallet/miner] [number of nodes].");
        }
        break;
    }

    case DELETE:
    {
        if (cmdList[1] != "wallet" && cmdList[1] != "miner")
        {
            Output("Ambiguous command \"" + cmd + "\": delete [wallet/miner].");
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
        break;
    }

    case MAKE:
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
                break;
            }
            bool flag = Wallet::walletSet[a].CreateTransaction(Wallet::walletInfo[b], value);
            cout.rdbuf(coutBackup);
            if (!flag)
            {
                throw 0;
            }
            Output("Transaction made.");
        }
        catch (bool)
        {
            Output("Ambiguous command \"" + cmd + "\": make [walletA id] [walletB id] [value].");
        }
        catch (char const *)
        {
            stringstream ss;
            ss << Wallet::walletSet.size();
            Output("Wallet id cannot find. ONLY " + ss.str() + " nodes exists.");
        }
        catch (int)
        {
            Output("Transaction construction failed.");
        }
        break;
    }

    case CLEAN:
        Clean();
        break;

    case MINE:
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
            stringstream ss;
            ss << output;
            Output("Found miner id: " + ss.str());
            Output("Found miner address: " + Miner::minerSet[output]->GetAddress());
        }
        catch (bool)
        {
            Output("All transactions have already been packed.");
        }
        catch (int)
        {
            Output("No miner in the net!");
        }
        catch (char const *)
        {
            Output("Ambiguous command \"" + cmd + "\": mine");
        }
        break;
    }

    case DISPLAY:
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
        }
        catch (bool)
        {
            Output("Ambiguous command \"" + cmd + "\": display [wallet id].");
        }
        catch (int)
        {
            stringstream ss;
            ss << Wallet::walletSet.size();
            Output("Wallet id cannot find. ONLY " + ss.str() + " nodes exists.");
        }
        break;
    }

    case FIND:
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
            int *balance = Wallet::walletSet[id].FindBalance();
            int len = balance[0];
            int remainder = balance[len];
            cout.rdbuf(coutBackup);
            stringstream ss;
            if (!remainder)
            {
                ss << dec << remainder;
            }
            else
            {
                ss << dec << remainder << "(";
                for (int i = 1; i < len - 1; i++)
                {
                    ss << dec << balance[i] << " + ";
                }
                ss << dec << balance[len - 1] << ")";
            }
            Output("Money in wallet " + Wallet::walletSet[id].GetAddress() + " : " + ss.str());
            delete[] balance;
        }
        catch (bool)
        {
            cout << "Ambiguous command "
                 << "\"" << cmd << "\":"
                 << "find [wallet id]." << endl;
        }
        catch (int)
        {
            stringstream ss;
            ss << Wallet::walletSet.size();
            Output("Wallet id cannot find. ONLY " + ss.str() + " nodes exists.");
        }
        break;
    }

    case LOG:
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
        }
        catch (bool)
        {
            Output("Ambiguous command \"" + cmd + "\": log");
        }
        catch (int)
        {
            Output("The log is empty!");
        }
        break;
    }

    case EXIT:
    {
        fileOut.close();
        return false;
        break;
    }

    default:
    {
        string guess = EditDistance(cmdList[0]);
        Output("Undefined command: \"" + cmdList[0] + "\". Do you mean \"" + guess + "\" ?");
        Output("Try \"help\".");
        break;
    }
    }
    return true;
}
