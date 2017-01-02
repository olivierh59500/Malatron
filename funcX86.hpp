#ifndef FUNCX86_HPP
#define FUNCX86_HPP

struct instructionX86 {
    std::string mnemonic;
    std::vector<std::string> ops;
    int address;
    instructionX86(const std::string& mnem, 
        const std::vector<std::string>& o, int a) :mnemonic(mnem), ops(o), address(a)
    {
        // I love Gu.
    }
};

class funcX86;

class funcX86 {
private:
    int address;
    std::vector<instructionX86> instr;
    std::string name; // assigned name
public:
    funcX86(const std::vector<instructionX86>& i, 
        const std::string& n) :instr(i), name(n)
    {
        address = i.front().address;
    }

    // this is for "stub" functions (which we will count, sometimes as syscalls, etc)
    funcX86(const std::string& n) :name(n) {
        address = -1;
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