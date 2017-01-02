#include "PEX86.hpp"
#include <iostream>

int main() {
    PEX86 pe("crypto.exe");
    auto disass = pe.flat_disass();
    for (const instructionX86& i : disass) {
        std::cout << i.mnemonic << std::endl;
    }
}