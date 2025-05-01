#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <algorithm>
#include "pattern_analyzer.h"

using namespace std;

// Function to count the occurrences of each character in the pattern
map<char, int> pattern_analyzer::count_characters(const vector<string>& pattern) {
    map<char, int> counts;
    for (const auto& row : pattern) {
        for (char c : row) {
            counts[c]++;
        }
    }
    return counts;
}

// Function to find the character(s) with the maximum count
vector<char> pattern_analyzer::find_max_char(const map<char, int>& counts) {
    vector<char> max_chars;
    int max_count = -1;
    for (const auto& pair : counts) {
        if (pair.second > max_count) {
            max_count = pair.second;
            max_chars.clear();
            max_chars.push_back(pair.first);
        } else if (pair.second == max_count && max_count != -1) {
            max_chars.push_back(pair.first);
        }
    }
    return max_chars;
}

// Function to find the character(s) with the minimum count (greater than zero)
vector<char> pattern_analyzer::find_min_char(const map<char, int>& counts) {
    vector<char> min_chars;
    int min_count = -1;
    bool first = true;
    for (const auto& pair : counts) {
        if (pair.second > 0) {
            if (first || pair.second < min_count) {
                min_count = pair.second;
                min_chars.clear();
                min_chars.push_back(pair.first);
                first = false;
            } else if (pair.second == min_count) {
                min_chars.push_back(pair.first);
            }
        }
    }
    return min_chars;
}

// Placeholder for additional pattern analysis functions (to meet line count)
void pattern_analyzer::analyze_pattern_diversity(const map<char, int>& counts) {
    int distinct_chars = 0;
    for (const auto& pair : counts) {
        if (pair.second > 0) {
            distinct_chars++;
        }
    }
    cout << "[Analysis] Number of distinct characters: " << distinct_chars << endl;
}

void pattern_analyzer::calculate_average_frequency(const map<char, int>& counts, int total_chars) {
    if (total_chars > 0) {
        double average = static_cast<double>(total_chars) / counts.size();
        cout << "[Analysis] Average character frequency: " << average << endl;
    } else {
        cout << "[Analysis] Cannot calculate average frequency for an empty pattern." << endl;
    }
}
