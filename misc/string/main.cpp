#include <iostream>

#include "string.h"
using namespace std;

//~ int main(int argc, char **argv) {
int main() {
    String s("Hello");
    cout << s[3] << "\t" << s.length() << endl;
    
    s += "World";
    cout << s.getString() << "\t" << s.length() << endl;
    
    return 0;
}