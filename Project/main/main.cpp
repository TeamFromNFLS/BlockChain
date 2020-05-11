#include "command.h"

using namespace std;

enum CMD_SET
{
     SHOW,
     DEBUG,
     ERR
};

CMD_SET CmdSelect(char *const cmd)
{
     if (cmd == "show")
          return SHOW;
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
          case SHOW:
               cout << "Run demo..." << endl;
               break;
          case DEBUG:
               cout << "Run errors..." << endl;
               break;
          default:
               throw false;
               break;
          }
     }
     catch (int)
     {
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
