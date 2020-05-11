#include "command.h"

using namespace std;

enum CMD_SET
{
     DEMO,
     DEBUG,
     ERROR
};

CMD_SET CmdSelect(char *const cmd)
{
     if (cmd == "demo")
          return DEMO;
     if (cmd == "DEBUG")
          return DEBUG;
     else
          return ERROR;
}

int main(int argc, char **argv)
{
     Init();
     string cmd;
     while (getline(cin, cmd))
     {
          if (!Work(cmd))
          {
               break;
          }
     }
     return 0;
}
