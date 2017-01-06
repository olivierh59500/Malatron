#include "PEX86.hpp"
#include "aho_corasick.hpp"
#include <cstdlib>
#include <string>
#include <vector>
#include <fstream>

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

const std::vector<std::string> anti_ops = {"cpuid", "int3", "rdtsc"};

metrics count_metrics(const std::string& file) {
    PEX86 pe(file);
    metrics out;
    for (const std::string& str : anti_ops)
        out.antidebug += pe.count(str);

    out.nop = pe.count("nop");
    return out;
}