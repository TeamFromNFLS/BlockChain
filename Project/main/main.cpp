#include <iostream>
#include <vector>
#include "bigInt.h"
using namespace std;

int main()
{
    vector<int> getNumber;
    //cin >> a;
    string s = "3";
    BigInt a = 5, b("5");
    a.check();
    BigInt c, d("12345");
    c += 5;
    bool flag = (c == 25);
    //c = a * a;
    cout << c << endl;

    return 0;
}