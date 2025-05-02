#include <iostream>
#include <string>
#include <vector>
#include <sstream>
#include <stdexcept>

#include "cgicc/Cgicc.h"
#include "cgicc/HTTPHTMLHeader.h"
#include "cgicc/HTMLClasses.h"
#include "cgicc/FormFile.h"

#include "get_validate_input.h"

using namespace std;
using namespace cgicc;

// Trims leading and trailing whitespace from a string
string get_validate_input::trim(const string& str) {
    const char* whitespace = " \t\n\r\f\v";
    size_t first = str.find_first_not_of(whitespace);
    if (string::npos == first) {
        return "";
    }
    size_t last = str.find_last_not_of(whitespace);
    return str.substr(first, (last - first + 1));
}

// Checks if a string contains only uppercase letters
bool get_validate_input::is_uppercase(const string& str) {
    if (str.empty()) {
        return false;
    }
    for (char c : str) {
        if (c < 'A' || c > 'Z') {
            return false;
        }
    }
    return true;
}

// Extracts and validates form data including the uploaded pattern file,
// the guessed pattern string, the guessed occurrences count, and the convert option.
// Variable names aligned with HTML form field names where appropriate.
bool get_validate_input::get_form_data(vector<string>& pattern_grid_content, string& guessed_pattern, int& guessed_occurrences, bool& convertToNumber) {
    try {
        Cgicc cgi; // Create CGI object to access form data

        // Retrieve the "guess_pattern" field from the form
        const_form_iterator guess_pattern_iter = cgi.getElement("guess_pattern");
        if (guess_pattern_iter == cgi.getElements().end() || guess_pattern_iter->getValue().empty()) {
            cout << "Error: Form field 'guess_pattern' not found or empty." << endl;
            return false;
        }
        guessed_pattern = guess_pattern_iter->getValue(); // Assign value to the updated variable name

        // Validate the guessed pattern string (e.g., check if it's empty or contains invalid characters)
        // Assuming only uppercase letters are allowed in the pattern string based on previous logic.
        if (guessed_pattern.empty() || !is_uppercase(guessed_pattern)) { // Assuming is_uppercase can handle strings > 1 char
            cout << "Error: Invalid guess pattern submitted: '" << guessed_pattern << "'. Expected one or more uppercase letters." << endl;
            return false;
        }


        // Retrieve the "guess_occurrences" field from the form
        const_form_iterator guess_occurrences_iter = cgi.getElement("guess_occurrences");
        if (guess_occurrences_iter == cgi.getElements().end() || guess_occurrences_iter->getValue().empty()) {
            cout << "Error: Form field 'guess_occurrences' not found or empty." << endl;
            return false;
        }
        string occurrence_str = guess_occurrences_iter->getValue(); // Get string value from the field

        // Convert the string value to an integer and validate
        try {
            size_t processed_chars;
            guessed_occurrences = stoi(occurrence_str, &processed_chars); // Convert and assign to updated variable name

            // Check if the entire string was consumed by stoi, ensuring no trailing non-digit characters
            if (processed_chars != occurrence_str.length()) {
                 cout << "Error: Invalid guess occurrences submitted: '" << occurrence_str << "'. Expected an integer." << endl;
                 return false;
            }

            // Optional: Add a check for non-negative occurrence, as occurrences are typically non-negative
            if (guessed_occurrences < 0) {
                 cout << "Error: Invalid guess occurrences submitted: '" << occurrence_str << "'. Expected a non-negative integer." << endl;
                 return false;
            }

        } catch (const invalid_argument& ia) {
            cout << "Error: Invalid guess occurrences submitted: '" << occurrence_str << "'. Expected an integer." << endl;
            return false;
        } catch (const out_of_range& oor) {
            cout << "Error: Guess occurrences value out of integer range: '" << occurrence_str << "'" << endl;
            return false;
        }

        // retrieve the convertToNum radio button
        // Assuming the radio button's name is "conversion" based on your HTML
        const_form_iterator conversion_iter = cgi.getElement("conversion"); // Updated iterator name and declared here
        if (conversion_iter != cgi.getElements().end() && conversion_iter->getValue() == "convert") {
            convertToNumber = true;
        } else {
            convertToNumber = false;
        }

        // Access the uploaded file from the form using its name "pattern_file"
        file_iterator fileIter = cgi.getFile("pattern_file");
        if (fileIter == cgi.getFiles().end()) {
            cout << "Error: File input field 'pattern_file' not found in the submitted form data." << endl;
            return false;
        }

        const FormFile& fileRef = *fileIter;
         // Check if the uploaded file is empty
        if (fileRef.getDataLength() == 0) {
            cout << "Error: Uploaded file 'pattern_file' is empty." << endl;
            return false;
        }
        // Read the file contents into a string stream
        stringstream ss_file_content(fileRef.getData());
        string line;

        size_t rows = 0;
        size_t cols = 0;

        // Read the first line which should contain the dimensions (rows and columns)
        if (getline(ss_file_content, line)) {
            stringstream ss_dims(line);
            if (!(ss_dims >> rows >> cols) || rows == 0 || cols == 0) {
                cout << "Error: Could not parse valid positive rows and columns from the first line of the file: '" << line << "'" << endl;
                return false;
            }
        } else {
            cout << "Error: Uploaded file content seems empty or invalid after checking size (could not read first line)." << endl;
            return false;
        }

        pattern_grid_content.clear(); // Clear the vector to store new content
        size_t actual_rows_read = 0;
        // Read and validate each line of the pattern data
        while (getline(ss_file_content, line)) {
            string trimmed_line = trim(line);

            if (!trimmed_line.empty()) {
            // Each line must have the correct number of columns and be uppercase
                if (trimmed_line.length() == cols && is_uppercase(trimmed_line)) {
                    pattern_grid_content.push_back(trimmed_line); // Store line in the updated variable name
                    actual_rows_read++;
                } else {
                    cout << "Error: Invalid pattern line format or content found. Line: '" << trimmed_line << "'. Expected " << cols << " uppercase characters." << endl;
                    return false;
                }
            }
        }
         // Ensure the number of valid rows read matches the expected number
        if (actual_rows_read != rows) {
            cout << "Error: Number of valid pattern rows read (" << actual_rows_read
                 << ") does not match the expected number of rows (" << rows
                 << ") specified in the first line." << endl;
            return false;
        }
        // All validations passed
        return true;

    } catch (const exception& e) {
        // Catch and print any standard exceptions
        cout << "Exception caught during form data processing: " << e.what() << endl;
        return false;
    } catch (...) {
        // Catch any unknown exceptions
        cout << "Unknown exception caught during form data processing." << endl;
        return false;
    }
}
