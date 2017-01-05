#ifndef FUNCX86_HPP
#define FUNCX86_HPP

typedef unsigned long long addr_t;

struct instructionX86 {
    std::string mnemonic;
    std::vector<std::string> ops;
    addr_t address;
    instructionX86(const std::string& mnem, 
        const std::vector<std::string>& o, addr_t a) :mnemonic(mnem), ops(o), address(a)
    {
        // I love Gu.
    }
};

class funcX86;

class funcX86 {
private:
    addr_t address;
    std::vector<instructionX86> instr;
    std::string name; // assigned name
public:
    funcX86(const std::vector<instructionX86>& i, 
        const std::string& n) :instr(i), name(n)
    {
        if (i.empty()) {
            address = (addr_t)(-1);
        } else {
            address = i.front().address;
        }
    }

    // this is for "stub" functions (which we will count, sometimes as syscalls, etc)
    funcX86(const std::string& n) :name(n) {
        address = (addr_t)(-1); // addr_t is unsigned
    }

    std::vector<instructionX86> get_instructions() {
        return instr;
    }

    std::string get_name() {
        return name;
    }

    int get_address() {
        return address;
    }
};

#endif