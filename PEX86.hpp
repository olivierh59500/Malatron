#ifndef PEX86_HPP
#define PEX86_HPP

#include <string>
#include <vector>
#include "funcX86.hpp"

class PEX86 {
private:
    std::string path;
    std::vector<funcX86*> funcs;
public:
    PEX86(const std::string& p);
    std::vector<instructionX86> flat_disass();
    ~PEX86() {
        for (funcX86* f : funcs) {
            delete f;
        }
    }
};

#endif