#include <iostream>
#include <string>
#include <vector>
// #include <map> // Removed map include as character counts are no longer central to the core game logic
#include <sstream>
#include <algorithm> // Still potentially useful
#include "generate_html.h" // For generating HTML output
#include "get_validate_input.h" // For getting and validating form data
#include "pattern_analyzer.h" // For analyzing the pattern grid (counting occurrences and numbering)

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
    cout << generate_html::generate_html_header("Word Search Game Result"); // Updated title

    // Variables to store extracted form and file data
    vector<string> pattern_grid_content; // Stores the original pattern grid from the file
    string guessed_pattern; // Stores the user's guessed pattern string
    int guessed_occurrence; // Stores the user's guessed number of occurrences
    bool convertToNumber; // Stores the state of the 'convert to numbers' checkbox

    // Attempt to read and validate the form and file data
    // The get_form_data function populates pattern_grid_content, guessed_pattern, guessed_occurrence, and convertToNumber
    if (!get_validate_input::get_form_data(pattern_grid_content, guessed_pattern, guessed_occurrence, convertToNumber)) {
        // If input validation failed, show an error message
        cout << generate_html::generate_heading("Error: Invalid Input", 2);
        cout << generate_html::generate_paragraph("There was an issue processing the uploaded file or the form data. Please check the file format and your input. Check the server error log for more details.");
        // Note: More specific error messages are generated within get_validate_input::get_form_data
    } else {
        // --- Game Logic: Word Search ---

        // Count the actual non-overlapping occurrences of the guessed pattern in the original grid
        int actual_occurrence_count = pattern_analyzer::count_pattern_occurrences(pattern_grid_content, guessed_pattern);

        // Determine if the user's guess is correct by comparing guessed and actual occurrences
        bool correct = (guessed_occurrence == actual_occurrence_count);

        // --- Output Results ---

        // Always display the original pattern grid
        cout << generate_html::generate_paragraph("Original Pattern Grid:"); // Add label for the original grid
        cout << generate_html::generate_pattern_table(pattern_grid_content);

        // If convertToNumber is true, generate and display the numbered grid
        if (convertToNumber) {
            // Generate the grid with occurrences replaced by numbers using the new function
            vector<string> numbered_grid = generate_html::generate_numbered_grid(pattern_grid_content, guessed_pattern);
            // Display the numbered grid
            cout << generate_html::generate_paragraph("Pattern Grid with Occurrences Numbered:"); // Add label for the numbered grid
            cout << generate_html::generate_pattern_table(numbered_grid);
        }

        // Show analysis and the user's guess result
        cout << generate_html::generate_heading("Word Search Analysis and Guess Result", 2); // Updated heading

        // Display whether the guess was correct and the actual correct count
        cout << generate_html::generate_result_message(correct, guessed_pattern, guessed_occurrence, actual_occurrence_count);

        // Removed calls to generate_character_counts as it's not part of the core game result display
    }

    // Output the closing HTML tags
    cout << generate_html::generate_html_footer();

    return 0;
}
