#include <iostream>

#include "string.h"
using namespace std;

//~ int main(int argc, char **argv) {
int main() {
    String s("Hello");
    cout << s[3] << "\t" << s.length() << endl;
    
    s += "World";
    cout << s.getString() << "\t" << s.length() << endl;
    
    cout << (s.equals("") ? "True" : "False") << endl;
    return 0;
}