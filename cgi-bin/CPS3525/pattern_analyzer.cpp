#include <iostream>
#include <string>
#include <vector>
#include <map> // Included for potential future use or compatibility.
#include <algorithm>
#include <utility> // For std::pair
#include "pattern_analyzer.h" // Include the updated header file

using namespace std;
// Removed 'using namespace pattern_analyzer;' here to be more explicit and avoid potential issues
// Instead, use pattern_analyzer:: prefix for functions defined within the namespace.

// Helper function to find overlapping horizontal occurrences and store locations
vector<pattern_analyzer::PatternLocation> pattern_analyzer::find_horizontal_locations(const vector<string>& grid, const string& pattern) {
    vector<pattern_analyzer::PatternLocation> locations;
    if (pattern.empty() || grid.empty() || grid[0].empty()) {
        return locations;
    }

    int rows = grid.size();
    int cols = grid[0].size();
    int pattern_len = pattern.length();

    if (pattern_len > 0 && pattern_len <= cols) {
        for (int i = 0; i < rows; ++i) {
            const string& row_str = grid[i];
            size_t pos = row_str.find(pattern, 0);
            // Modified to find overlapping occurrences: advance position by 1
            while (pos != string::npos) {
                locations.push_back({i, static_cast<int>(pos), 'H', pattern_len});
                pos = row_str.find(pattern, pos + 1); // Find next occurrence starting from the next character
            }
        }
    }
    return locations;
}

// Helper function to find overlapping vertical occurrences and store locations
vector<pattern_analyzer::PatternLocation> pattern_analyzer::find_vertical_locations(const vector<string>& grid, const string& pattern) {
    vector<pattern_analyzer::PatternLocation> locations;
    if (pattern.empty() || grid.empty() || grid[0].empty()) {
        return locations;
    }

    int rows = grid.size();
    int cols = grid[0].size();
    int pattern_len = pattern.length();

    if (pattern_len > 0 && pattern_len <= rows) {
        for (int j = 0; j < cols; ++j) {
            string col_str = "";
            for (int i = 0; i < rows; ++i) {
                col_str += grid[i][j];
            }

            size_t pos = col_str.find(pattern, 0);
            // Modified to find overlapping occurrences: advance position by 1
            while (pos != string::npos) {
                locations.push_back({static_cast<int>(pos), j, 'V', pattern_len});
                pos = col_str.find(pattern, pos + 1); // Find next occurrence starting from the next character
            }
        }
    }
    return locations;
}


// Counts overlapping occurrences that don't share characters between horizontal and vertical matches
int pattern_analyzer::count_pattern_occurrences(const vector<string>& grid, const string& pattern) {
    if (pattern.empty() || grid.empty() || grid[0].empty()) {
        return 0;
    }

    int rows = grid.size();
    int cols = grid[0].size();
    int pattern_len = pattern.length();

    if (pattern_len == 0) return 0;

    // Call helper functions to find ALL overlapping locations
    vector<PatternLocation> h_locations = pattern_analyzer::find_horizontal_locations(grid, pattern);
    vector<PatternLocation> v_locations = pattern_analyzer::find_vertical_locations(grid, pattern);

    vector<PatternLocation> all_locations;
    all_locations.insert(all_locations.end(), h_locations.begin(), h_locations.end());
    all_locations.insert(all_locations.end(), v_locations.begin(), v_locations.end());

    // Sort locations to process them in a consistent order (reading order)
    sort(all_locations.begin(), all_locations.end());

    // Use a boolean grid to track occupied cells
    vector<vector<bool>> occupied(rows, vector<bool>(cols, false));
    int valid_count = 0;

    for (const auto& loc : all_locations) {
        bool can_occupy = true;
        // Check if the cells for this occurrence are already occupied
        if (loc.direction == 'H') {
            for (int i = 0; i < loc.length; ++i) {
                // Ensure bounds are checked
                if (loc.col + i < cols && occupied[loc.row][loc.col + i]) {
                    can_occupy = false;
                    break;
                }
            }
        } else { // direction == 'V'
            for (int i = 0; i < loc.length; ++i) {
                 // Ensure bounds are checked
                if (loc.row + i < rows && occupied[loc.row + i][loc.col]) {
                    can_occupy = false;
                    break;
                }
            }
        }

        // If no cells are occupied, this is a valid occurrence (not sharing characters with previously processed valid matches)
        if (can_occupy) {
            valid_count++;
            // Mark the cells as occupied
            if (loc.direction == 'H') {
                for (int i = 0; i < loc.length; ++i) {
                     if (loc.col + i < cols) { // Ensure bounds are checked
                        occupied[loc.row][loc.col + i] = true;
                     }
                }
            } else { // direction == 'V'
                for (int i = 0; i < loc.length; ++i) {
                    if (loc.row + i < rows) { // Ensure bounds are checked
                        occupied[loc.row + i][loc.col] = true;
                    }
                }
            }
        }
    }

    return valid_count; // Return the total count of valid, non-sharing overlapping occurrences
}

// Generates a grid with overlapping and non-sharing pattern occurrences replaced by numbers
vector<string> pattern_analyzer::generate_numbered_grid(const vector<string>& grid, const string& pattern) {
    vector<string> numbered_grid = grid; // Create a copy of the original grid

    if (pattern.empty() || grid.empty() || grid[0].empty()) {
        return numbered_grid; // Return original grid if pattern or grid is invalid
    }

    int rows = grid.size();
    int cols = grid[0].size();
    int pattern_len = pattern.length();

    if (pattern_len == 0) return numbered_grid;

    // Call helper functions to find ALL overlapping locations
    vector<PatternLocation> h_locations = pattern_analyzer::find_horizontal_locations(grid, pattern);
    vector<PatternLocation> v_locations = pattern_analyzer::find_vertical_locations(grid, pattern);

    vector<PatternLocation> all_locations;
    all_locations.insert(all_locations.end(), h_locations.begin(), h_locations.end());
    all_locations.insert(all_locations.end(), v_locations.begin(), v_locations.end());

    // Sort locations to process them in a consistent order (reading order)
    sort(all_locations.begin(), all_locations.end());

    // Use a boolean grid to track occupied cells
    vector<vector<bool>> occupied(rows, vector<bool>(cols, false));
    int occurrence_counter = 1;

    for (const auto& loc : all_locations) {
        bool can_occupy = true;
        // Check if the cells for this occurrence are already occupied
        if (loc.direction == 'H') {
            for (int i = 0; i < loc.length; ++i) {
                 if (loc.col + i < cols && occupied[loc.row][loc.col + i]) {
                    can_occupy = false;
                    break;
                }
            }
        } else { // direction == 'V'
            for (int i = 0; i < loc.length; ++i) {
                if (loc.row + i < rows && occupied[loc.row + i][loc.col]) {
                    can_occupy = false;
                    break;
                }
            }
        }

        // If no cells are occupied, this is a valid occurrence (not sharing characters with previously processed valid matches)
        if (can_occupy) {
            // Determine the replacement character based on the occurrence counter
            char rep_char;
            if (occurrence_counter > 0 && occurrence_counter <= 9) {
                rep_char = '0' + occurrence_counter;
            } else if (occurrence_counter >= 10 && occurrence_counter <= 10 + ('Z' - 'A')) {
                rep_char = 'A' + (occurrence_counter - 10);
            } else {
                 // If more than 36 occurrences, use a placeholder or handle as an error.
                 rep_char = '*';
            }

            // Perform the replacement in the numbered_grid copy and mark cells as occupied
            if (loc.direction == 'H') {
                for (int i = 0; i < loc.length; ++i) {
                     if (loc.col + i < cols) { // Ensure bounds are checked
                        numbered_grid[loc.row][loc.col + i] = rep_char;
                        occupied[loc.row][loc.col + i] = true;
                     }
                }
            } else { // direction == 'V'
                for (int i = 0; i < loc.length; ++i) {
                    if (loc.row + i < rows) { // Ensure bounds are checked
                        numbered_grid[loc.row + i][loc.col] = rep_char;
                        occupied[loc.row + i][loc.col] = true;
                    }
                }
            }

            occurrence_counter++; // Move to the next occurrence number
        }
    }

    return numbered_grid; // Return the grid with numbered occurrences
}
