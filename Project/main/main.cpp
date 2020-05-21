#include <stdio.h>
#include <unistd.h>
#include <termio.h>
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

/*To solve the problem that in Linux getline would get a \r, I would use this SafeGetline.
copyright: 
————————————————
版权声明：本文为CSDN博主「潜行狙击」的原创文章，遵循CC 4.0 BY-SA版权协议，转载请附上原文出处链接及本声明。
原文链接：https://blog.csdn.net/fanwenbo/java/article/details/17390487*/
istream &SafeGetline(std::istream &is, std::string &t)
{
     t.clear();
     istream::sentry se(is, true);
     streambuf *sb = is.rdbuf();
     for (;;)
     {
          int c = sb->sbumpc();
          switch (c)
          {
          case '\n':
               return is;
          case '\r':
               if (sb->sgetc() == '\n')
                    sb->sbumpc();
               return is;
          case EOF:
               if (t.empty())
                    is.setstate(std::ios::eofbit);
               return is;
          default:
               t += (char)c;
          }
     }
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
                    //cout << "nice" << endl;
                    if (line == "log")
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
                    if (line == "log")
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
          while (1)
          {
               if (scanKeyboard() == 279165)
               {
                    cout << "yes" << endl;
               }
               printf(":%d", scanKeyboard());
          }
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
