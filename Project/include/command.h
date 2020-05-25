#ifndef TEST_H
#define TEST_H

#include <string>
#include <iostream>
#include <sstream>
#include <fstream>

using namespace std;

enum COMMAND
{
    HELP,
    ADD,
    DELETE,
    MINE,
    FIND,
    MAKE,
    DISPLAY,
    CLEAN,
    LOG,
    IMPORT,
    EXIT,
    ERROR
};

extern ofstream fileOut;
extern ifstream logIn, fileIn;
extern streambuf *coutBackup, *fileBackup, *cinBackup, *inBackup, *logBackup;
extern struct timeval timeStart, timeEnd;

int Mine();
int balance();
void Init();
bool Work(string s);
void Output(string s);
void QuickOutput(string s);
vector<string> CmdInit(string cmd);
istream &SafeGetline(std::istream &is, std::string &t);

#endif //TEST_H