#ifndef PATTERN_ANALYZER_H
#define PATTERN_ANALYZER_H

#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <algorithm>
using namespace std;

namespace pattern_analyzer {
    map<char, int> count_characters(const vector<string>& pattern);
    vector<char> find_max_char(const map<char, int>& counts);
    vector<char> find_min_char(const map<char, int>& counts);
    void analyze_pattern_diversity(const map<char, int>& counts);
    void calculate_average_frequency(const map<char, int>& counts, int total_chars);
} // namespace pattern_analyzer

#endif 
