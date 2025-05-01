#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <sstream>
#include <cstdlib>
#include <algorithm>
#include "generate_html.h"
#include "get_validate_input.h"
#include "pattern_analyzer.h"

using namespace std;

int main() {
    cout << generate_html::generate_html_header("Pattern Game Result");

    vector<string> pattern;
    string guess;
    string type;

    if (!get_validate_input::get_form_data(pattern, guess, type)) {
        cout << generate_html::generate_heading("Error: Invalid Input", 2);
        cout << generate_html::generate_paragraph("There was an issue processing the uploaded file or the form data. Please check the file format and your input.");
    } else {
        map<char, int> char_counts = pattern_analyzer::count_characters(pattern);
        vector<char> max_chars = pattern_analyzer::find_max_char(char_counts);
        vector<char> min_chars = pattern_analyzer::find_min_char(char_counts);
        bool correct = false;
        vector<char> correct_chars;

         if (type == "maximum") {
            if (find(max_chars.begin(), max_chars.end(), guess[0]) != max_chars.end()) {
                correct = true;
                correct_chars = max_chars;
            }
        } else if (type == "minimum") {
             if (find(min_chars.begin(), min_chars.end(), guess[0]) != min_chars.end()) {
                correct = true;
                correct_chars = min_chars;
            }
        }

        cout << generate_html::generate_pattern_table(pattern);
        cout << generate_html::generate_heading("Pattern Analysis and Guess Result", 2);
        cout << generate_html::generate_paragraph("Your guess: " + guess + " for " + type);
        cout << generate_html::generate_character_counts(char_counts);
        cout << generate_html::generate_result_message(correct, guess, type, correct_chars, char_counts);
    }

    cout << generate_html::generate_html_footer();

    return 0;
}
