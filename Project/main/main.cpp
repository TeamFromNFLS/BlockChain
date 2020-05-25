#include <stdio.h>
#include <unistd.h>
#include <vector>
#include "command.h"

using namespace std;

enum CMD_SET
{
     DEMO,
     DEBUG,
     ERR
};

CMD_SET CmdSelect(string const cmd)
{
     if (cmd == "demo")
          return DEMO;
     if (cmd == "debug")
          return DEBUG;
     else
          return ERR;
}

int main(int argc, char **argv)
{
     Init();
     try
     {
          if (argc == 1)
          {
               throw 0;
          }
          if (argc != 2)
          {
               throw false;
          }
          switch (CmdSelect(argv[1]))
          {
          case DEMO:
          {
               cout << "Run demo..." << endl;
               fileIn.open("demo.txt", ios::in);
               inBackup = fileIn.rdbuf();
               cin.rdbuf(inBackup);
               string line;
               while (SafeGetline(cin, line))
               {
                    Output(line);
                    bool flag = Work(line);
                    vector<string> lineList = CmdInit(line);
                    if (lineList[0] == "log")
                    {
                         cin.rdbuf(inBackup);
                    }
                    if (lineList[0] == "import")
                    {
                         cin.rdbuf(inBackup);
                    }
                    if (!flag)
                    {
                         return 0;
                    }
               }
               fileIn.close();
               break;
          }
          case DEBUG:
          {
               cout << "Run errors..." << endl;
               fileIn.open("debug.txt", ios::in);
               inBackup = fileIn.rdbuf();
               cin.rdbuf(inBackup);
               string line;
               while (SafeGetline(cin, line))
               {
                    Output(line);
                    bool flag = Work(line);
                    vector<string> lineList = CmdInit(line);
                    if (lineList[0] == "log")
                    {
                         cin.rdbuf(inBackup);
                    }
                    if (lineList[0] == "import")
                    {
                         cin.rdbuf(inBackup);
                    }
                    if (!flag)
                    {
                         return 0;
                    }
               }
               fileIn.close();
               break;
          }
          default:
               throw false;
               break;
          }
     }
     catch (int)
     {
          string cmd;
          while (SafeGetline(cin, cmd))
          {
               if (!Work(cmd))
               {
                    break;
               }
          }
     }
     catch (bool)
     {
          Output("Wrong command. Type \"demo\" for demonstration; type \"debug\" for wrong-command demonstration.");
     }
     return 0;
}
