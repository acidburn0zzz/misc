#include <iostream>
#include <iomanip>
#include <sstream>

using namespace std;

void dec2int(const char *str) {
    unsigned int i;
    string s(str);
    stringstream ss(s);
    
    ss >> i;
    
    cout << i << endl;
}

void hex2int(const char *str) {
    unsigned int i;
    
    sscanf(str, "%x", &i);
    
    cout << i << endl;
}

int main() {
    dec2int("42");
    hex2int("ff");
    
    return 0;
}
