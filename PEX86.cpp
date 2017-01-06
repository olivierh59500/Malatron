#include "PEX86.hpp"
#include <fstream>
#include <cstdlib>
#include <unistd.h>
#include <iomanip>
#include <sstream>
#include <iostream>
PEX86::PEX86(const std::string& p) :path(p), timeout(false) {
    int pid = fork();
    if (pid == 0) {
        alarm(300); // -_- 
        execlp("python", "python", "disass.py", p.c_str(), "malatron_dout.txt", NULL);
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

std::vector<unsigned char> PEX86::get_bytes() {
    return bytes;
}

std::vector<instructionX86> PEX86::get_flat_disass() {
    return flat_disass;
}