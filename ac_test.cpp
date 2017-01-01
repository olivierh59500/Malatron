#include "aho_corasick.hpp"
#include <iostream>

int main() {
    std::vector<std::vector<int>> gu;
    std::vector<int> a = {0, 1, 2};
    std::vector<int> b = {1, 2};
    std::vector<int> bigly =  {0, 1, 2, 0, 1, 2};
    gu.push_back(a);
    gu.push_back(b);
    aho_corasick<3> deets(gu);
    std::cout << deets.count(bigly) << std::endl;
    return 0;
}