#ifndef TEST_H
#define TEST_H

#include <string>
#include <iostream>
#include <sstream>
#include <fstream>

using namespace std;

enum COMMAND
{
    DEMO,
    HELP,
    ADD,
    DELETE,
    MINE,
    FIND,
    MAKE,
    DISPLAY,
    CLEAN,
    LOG,
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

#endif //TEST_H