#include <iostream>
#include <vector>
#include "bigInt.h"
using namespace std;

int main()
{
    BigInt a("1700002"), b("12354548");
    //cin >> a;
    //cout << a << ' ' << b << endl;
    a /= 2;
    //BigInt c = a + b;
    cout << a << endl;
    return 0;
}