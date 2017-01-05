#ifndef PEX86_HPP
#define PEX86_HPP

#include <string>
#include <vector>
#include "funcX86.hpp"

class PEX86 {
private:
    std::string path;
    std::vector<funcX86*> funcs;
    std::vector<unsigned char> bytes;
    std::vector<instructionX86> flat_disass;
    bool timeout;
public:
    PEX86(const std::string& p);
    std::vector<instructionX86> get_flat_disass();
    std::vector<unsigned char> get_bytes();
    bool timed_out() {
        return timeout;
    }
    
    ~PEX86() {
        for (funcX86* f : funcs) {
            delete f;
        }
    }
};

#endif