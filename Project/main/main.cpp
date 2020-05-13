#include <stdio.h>
#include <unistd.h>
#include <termios.h>
#include "command.h"

using namespace std;

enum CMD_SET
{
     DEMO,
     DEBUG,
     ERR
};

/*read input from keyboard.
版权声明：本文为CSDN博主「jieniyimiao」的原创文章，遵循CC 4.0 BY-SA版权协议，转载请附上原文出处链接及本声明。
原文链接：https://blog.csdn.net/u013467442/java/article/details/51173441/*/
int scanKeyboard()
{
     int in;
     struct termios new_settings;
     struct termios stored_settings;
     tcgetattr(0, &stored_settings);
     new_settings = stored_settings;
     new_settings.c_lflag &= (~ICANON);
     new_settings.c_cc[VTIME] = 0;
     tcgetattr(0, &stored_settings);
     new_settings.c_cc[VMIN] = 1;
     tcsetattr(0, TCSANOW, &new_settings);

     in = getchar();

     tcsetattr(0, TCSANOW, &stored_settings);
     return in;
}

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
                    cout << line << endl;
                    Work(line);
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
                    cout << line << endl;
                    if (!Work(line))
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
               printf(":%d", scanKeyboard());
          }
          string cmd;
          while (getline(cin, cmd))
          {
               if (!Work(cmd))
               {
                    break;
               }
          }
     }
     catch (bool)
     {
          cout << "Wrong command." << endl;
     }
     return 0;
}
