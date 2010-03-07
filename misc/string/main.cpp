#include <iostream>

#include "string.h"
using namespace std;

int main() {
    String mine = "abcdef";

    cout << endl;

    cout << mine.substr(2, 2).getString() << endl;
    cout << mine.substr(4, 2).getString() << endl;
    cout << mine.substr(4, 3).getString() << endl;
    return 0;
}
