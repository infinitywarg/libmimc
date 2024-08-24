#include <iostream>
#include "MiMC.cpp"

using namespace std;


int main() {
    Field plain(22, 29);
    Field key(13, 29);

    std::cout<< "Plain Value: ";
    plain.print();

    std::cout<< "Encryption Key: ";
    key.print();

    MiMC config = *new MiMC(29);
    config.print();
    Field cipher = config.encrypt(plain, key);
    std::cout<< "Encrypted Value: ";
    cipher.print();
    Field decipher = config.decrypt(cipher,key);
    std::cout<< "Decrypted Value: ";
    decipher.print();


    return 0;
}
