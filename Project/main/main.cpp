#include <iostream>
#include <vector>
#include "bigInt.h"
using namespace std;

int main()
{
    vector<int> getNumber;
    //cin >> a;
    string s = "3";
    BigInt a("123"), b("123");
    BigInt c, d("12345");
    c = BigInt::Pow(a, b);
    //c = a * a;
    cout << c << endl;

    return 0;
}