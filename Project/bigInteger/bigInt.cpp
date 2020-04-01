#include"bigInt.h"
#include<iostream>
#include<cstring>
#include<vector>
using namespace std;
vector<int>::iterator it;
void BigInt::check() {
    while (!number.empty() && !number.back())
        number.pop_back();
    if (number.empty()) return;
    for (it = number.begin() + 1; it != number.end(); ++it) {
        *it += *(it - 1) / 10;
        *(it - 1) %= 10;
    }
    while (number.back() > 10) {
        it = number.begin() + number.size() - 2;
        number.push_back(number.back() / 10);
        *it %= 10;
    }
    return;
}

istream &operator>>(istream &input, BigInt &bigInt) {
    string s;
    input >> s;
    string::iterator _it;
    bigInt.number.clear();
    for (_it = s.end() - 1; _it >= s.begin(); --_it)
        bigInt.number.push_back(*_it - '0');
    return input;
}

ostream &operator<<(ostream &output, BigInt &bigInt) {
    if(bigInt.number.empty()) output << 0;
    for (it = bigInt.number.end() - 1; it >= bigInt.number.begin(); --it)
        output << *it;
    return output;
}