#include "PEX86.hpp"
#include <fstream>
#include <cstdlib>
#include <unistd.h>
#include <iomanip>
#include <iostream>
#include <sstream>

PEX86::PEX86(const std::string& p) :path(p) {
    int pid = fork();
    if (pid == 0) {
        execlp("python", "python", "disass.py", p.c_str(), "malatron_dout.txt", NULL);
        _exit(EXIT_FAILURE); // gu.
    }

    waitpid(pid, NULL, 0);
    
    // snag the deets;
    std::ifstream stream("malatron_dout.txt");
    while (stream) {
        std::string tmp;
        std::string fname;
        int faddr;
        std::vector<instructionX86> instr;
        stream >> tmp >> faddr;
        stream >> tmp >> fname;
        std::string line;
        while (std::getline(stream, line)) {
            if (line.find("| 0x") == std::string::npos)
                continue;
            if (line == "DONE")
                break;

            line = line.substr(1);
            int semi = line.find(';');
            if (semi != std::string::npos) {
                while (line.back() != ';')
                    line.pop_back();

                line.pop_back();
            }

            int address;
            std::istringstream iss(line);
            iss >> std::hex >> address;
            iss >> std::hex >> tmp;
            std::string mnem;
            iss >> mnem;
            
            // ignore all operands for now.
            // this is just a bare bones version so that opcodes can be extracted
            // more is soon to come ._.
            instr.emplace_back(mnem, std::vector<std::string>(), address);
        }

        funcX86* func = new funcX86(instr, fname);
        funcs.push_back(func);
    }

    std::system("rm malatron_dout.txt");

    // read the bytes:
    std::ifstream binfile(path, std::ios::binary);
    while (binfile) {
        bytes.push_back(binfile.get());
    }
}

std::vector<unsigned char> PEX86::get_bytes() {
    return bytes;
}

std::vector<instructionX86> PEX86::flat_disass() {
    std::vector<instructionX86> res;
    for (funcX86* f : funcs) {
        auto f_instr = f->get_instructions();
        for (const instructionX86& i : f_instr) {
            res.push_back(i);
        }
    }

    return res;
}