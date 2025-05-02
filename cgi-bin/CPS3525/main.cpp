#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <sstream>

#include <algorithm>
#include "generate_html.h"
#include "get_validate_input.h"
#include "pattern_analyzer.h"

#include <cgicc/CgiDefs.h>
#include <cgicc/Cgicc.h>
#include <cgicc/HTTPHTMLHeader.h>
#include <cgicc/HTMLClasses.h>

using namespace cgicc;
using namespace std;

int main() {
    // Output the HTTP response header to indicate an HTML page is being returned
    cout << "Content-Type: text/html\r\n\r\n";
    // Output the beginning of the HTML document with a page title
    cout << generate_html::generate_html_header("Pattern Game Result");
    // Variables to store extracted form and file data
    vector<string> pattern;
    string guess;
    string type;
    // Attempt to read and validate the form and file data
    if (!get_validate_input::get_form_data(pattern, guess, type)) {
        // If input validation failed, show an error message
        cout << generate_html::generate_heading("Error: Invalid Input", 2);
        cout << generate_html::generate_paragraph("There was an issue processing the uploaded file or the form data. Please check the file format and your input. Check the server error log for more details.");
    } else {
        // Analyze the uploaded pattern data to count character frequencies
        std::map<char, int> char_counts = pattern_analyzer::count_characters(pattern);
        // Identify characters with maximum and minimum frequency
        vector<char> max_chars = pattern_analyzer::find_max_char(char_counts);
        vector<char> min_chars = pattern_analyzer::find_min_char(char_counts);
        // Determine if the user's guess is correct
        bool correct = false;
        vector<char> correct_chars;

        if (type == "maximum") {
            // Check if guess matches one of the characters with max frequency
            if (!max_chars.empty() && find(max_chars.begin(), max_chars.end(), guess[0]) != max_chars.end()) {
                correct = true;
            }
             correct_chars = max_chars;
        } else if (type == "minimum") {
            // Check if guess matches one of the characters with min frequency
             if (!min_chars.empty() && find(min_chars.begin(), min_chars.end(), guess[0]) != min_chars.end()) {
                correct = true;
            }
             correct_chars = min_chars;
        }

        // Display the uploaded pattern in a table format
        cout << generate_html::generate_pattern_table(pattern);
        // Show analysis and the user's guess result
        cout << generate_html::generate_heading("Pattern Analysis and Guess Result", 2);
        cout << generate_html::generate_paragraph("Your guess: " + guess + " for " + type);
         // Display frequency of each character in the pattern
        cout << generate_html::generate_character_counts(char_counts);
        // Display whether the guess was correct and the actual correct characters
        cout << generate_html::generate_result_message(correct, guess, type, correct_chars, char_counts);
    }
    // Output the closing HTML tags
    cout << generate_html::generate_html_footer();

    return 0;
}
