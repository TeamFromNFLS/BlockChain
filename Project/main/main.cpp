#include <iostream>
#include <vector>
#include "bigInt.h"
using namespace std;

int main()
{
    BigInt a;
    vector<int> getNumber;
    cin >> a;
    string s = "123456789";
    a.SetNumber(s);
    a.GetNumber(getNumber);
    vector<int>::iterator it;
    for (it = getNumber.begin(); it != getNumber.end(); ++it)
        cout << *it;
    cout << endl;
    cout << a << endl;
    return 0;
}