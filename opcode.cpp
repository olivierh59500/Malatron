#include "PEX86.hpp"
#include <iostream>
#include <algorithm>
#include <map>

int main() {
    PEX86 pe("zeus.exe");
    auto disass = pe.flat_disass();
    std::map<std::string, int> gu;
    for (const instructionX86& i : disass) {
        gu[i.mnemonic]++;
    }
    
    std::vector<std::string> luv;
    for (auto a : gu) {
        luv.push_back(a.first);
    }

    std::sort(luv.begin(), luv.end(), [&](const std::string& a, const std::string& b) {
        return gu[a] > gu[b];
    });

    for (auto x : luv) {
        std::cout << x << '\t' << gu[x] << std::endl;
    }

    auto dumb = pe.get_bytes();
    for (unsigned char x : dumb)
        std::cout << x;

    return 0;
}