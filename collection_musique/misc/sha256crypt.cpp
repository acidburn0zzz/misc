#include <iostream>
#include <cstring>
#include "../sha2.h"

int main(int argc, char **argv) {
    sha256_ctx ctx;
	unsigned char hval[SHA256_DIGEST_SIZE], *data;
    std::string s;
    //~ char passwd_hash[65];
    
    if (argc == 2) {
        s = argv[1];
    } else {
        std::cout << "Veuillez entrer le mot de passe a crypter: ";
        std::cin >> s;
    }
    
    data = new unsigned char[s.length()];
    
    strncpy((char*)data, s.c_str(), s.length());
    
    sha256_begin(&ctx);
    sha256_hash(data, s.length(), &ctx);
    sha256_end(hval, &ctx);
    
    delete data;
    std::cout << std::hex;
    for (int i=0; i<SHA256_DIGEST_SIZE; i++)
        printf("%.2x", hval[i]);
    
    std::cout << std::endl;
    return 0;
}