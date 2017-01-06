#ifndef PEX86_HPP
#define PEX86_HPP

#include <string>
#include <vector>
#include <map>
#include "funcX86.hpp"

class PEX86 {
private:
    std::string path;
    std::vector<funcX86*> funcs;
    std::vector<unsigned char> bytes;
    std::vector<instructionX86> flat_disass;
    std::map<std::string, int> opcount;
    bool timeout;
public:
    PEX86(const std::string& p, const std::string& s, int tl);
    std::vector<instructionX86> get_flat_disass() {
        return flat_disass;
    }

    std::vector<unsigned char> get_bytes() {
        return bytes;
    }

    int count(const std::string& op) {
        return opcount[op];
    }

    std::vector<int> get_intcodes(const std::string& file);

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