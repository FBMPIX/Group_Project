#ifndef PATTERN_ANALYZER_H
#define PATTERN_ANALYZER_H

#include <vector>
#include <string>
#include <map> // Included for potential future use or compatibility, though not strictly needed for current core functions.
#include <utility> // For std::pair
#include <algorithm> // Included for std::sort

using namespace std;

// Namespace to organize pattern analysis related functions
namespace pattern_analyzer {

    struct PatternLocation {
        int row;
        int col;
        char direction; // 'H' for horizontal, 'V' for vertical
        int length;

        // Comparison operator for sorting PatternLocation objects (reading order)
        bool operator<(const PatternLocation& other) const {
            if (row != other.row) {
                return row < other.row;
            }
            return col < other.col;
        }
    };
    int count_pattern_occurrences(const vector<string>& grid, const string& pattern);
    vector<PatternLocation> find_horizontal_locations(const vector<string>& grid, const string& pattern);
    vector<PatternLocation> find_vertical_locations(const vector<string>& grid, const string& pattern);
    vector<string> generate_numbered_grid(const vector<string>& grid, const string& pattern);
} // namespace pattern_analyzer

#endif // PATTERN_ANALYZER_H
