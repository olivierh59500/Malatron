#include "PEX86.hpp"
#include "aho_corasick.hpp"
#include <cstdlib>
#include <string>
#include <vector>
#include <fstream>
#include <algorithm>

static const int TOTAL_OPCODES = 400; // some bs.

struct metrics {
    int antidebug;
    int nop;
    int op_matches;
    int ng_matches;
    // rest coming soon:
    metrics() :antidebug(0), op_matches(0), ng_matches(0), nop(0) {

    }
};

const std::vector<std::string> anti_ops = {"cpuid", "int", "rdtsc", "invalid", "sysenter", "sidt", "sgdt"};

// not a pointer once i add a copy constructor and move constructor or something .-.
aho_corasick<TOTAL_OPCODES>* AC_state(const std::string& file) {
    std::ifstream infile(file);
    std::vector<std::vector<int>> common;
    for (int len = 1; len <= 10; len++) {
        int num_seqs;
        infile >> num_seqs;
        for (int i = 0; i < num_seqs; i++) {
            std::vector<int> seq;
            std::copy_n(std::istream_iterator<int>(infile), len, std::back_inserter(seq));
            common.push_back(seq);
        }
    }

    return new aho_corasick<TOTAL_OPCODES>(common);
}

metrics count_metrics(const std::string& file) {
    PEX86 pe(file, "disass.py", 300);
    metrics out;
    for (const std::string& str : anti_ops)
        out.antidebug += pe.count(str);

    out.nop = pe.count("nop");
    aho_corasick<TOTAL_OPCODES>* state = AC_state("common_ops.txt");
    std::vector<int> intcodes = pe.get_intcodes("op_map.txt");
    out.op_matches = state->count(intcodes);
    delete state;
    return out;
}