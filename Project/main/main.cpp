#include <iostream>
#include <vector>
#include "bigInt.h"
using namespace std;

int main()
{
    BigInt a("16"), b("12354548");
    //cin >> a;
    //cout << a << ' ' << b << endl;
    a *= 16;
    //BigInt c = a + b;
    cout << a << endl;
    cout << BigInt::PowMod(2, 11, 23) << endl;
    return 0;
}