#include "PEX86.hpp"
#include <fstream>
#include <cstdlib>
#include <unistd.h>
#include <iomanip>
#include <sstream>
#include <iostream>
#include <map>

PEX86::PEX86(const std::string& p, const std::string& s, int tl) :path(p) {
    int pid = fork();
    if (pid == 0) {
        alarm(tl); // -_- 
        execlp("python", "python", s.c_str(), p.c_str(), "malatron_dout.txt", NULL);
        _exit(EXIT_FAILURE); // Gu.
    }

    int status;
    waitpid(pid, &status, 0);
    if (WIFSIGNALED(status) && WTERMSIG(status) == SIGALRM) {
        timeout = true;
        return;
    }

    // snag the deets;
    std::ifstream stream("malatron_dout.txt");
    while (true) {
        std::string tmp;
        std::string fname;
        addr_t faddr;
        std::vector<instructionX86> instr;
        if (!(stream >> tmp >> faddr)) 
            break;

        stream >> tmp >> fname;
        std::string line;
        while (std::getline(stream, line)) {
            if (line == "DONE")
                break;
            if (!(line.find("| 0x") == 0 || line.find("\\ 0x") == 0))
                continue;
            
            int t = line.find('~');
            if (t != std::string::npos)
                line = line.substr(0, t - 1) + line.substr(t + 1);

            line = line.substr(1);
            addr_t address;
            std::istringstream iss(line);
            iss >> std::hex >> address;
            iss >> std::hex >> tmp;
            std::string mnem;
            iss >> mnem;
            
            // ignore all operands for now.
            // this is just a bare bones version so that opcodes can be extracted
            // more is soon to come ._.
            instr.emplace_back(mnem, std::vector<std::string>(), address);
            opcount[mnem]++;
            flat_disass.push_back(instr.back());
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

std::vector<int> PEX86::get_intcodes(const std::string& file) {
    std::ifstream infile(file);
    std::map<std::string, int> mnem2op;
    while (infile) {
        std::string mnem;
        int code;
        infile >> mnem >> code;
        mnem2op[mnem] = code;
    }

    auto disass = this->get_flat_disass();
    std::vector<int> result;
    for (int i = 0; i < disass.size(); i++)
        result.push_back(mnem2op[disass[i].mnemonic]);

    return result;
}