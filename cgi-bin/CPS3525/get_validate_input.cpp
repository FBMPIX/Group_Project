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

string get_validate_input::trim(const string& str) {
    const char* whitespace = " \t\n\r\f\v";
    size_t first = str.find_first_not_of(whitespace);
    if (string::npos == first) {
        return "";
    }
    size_t last = str.find_last_not_of(whitespace);
    return str.substr(first, (last - first + 1));
}

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

bool get_validate_input::get_form_data(vector<string>& pattern, string& guess, string& type) {
    try {

        Cgicc cgi;

        const_form_iterator guess_iter = cgi.getElement("guess_char");
        if (guess_iter == cgi.getElements().end() || guess_iter->getValue().empty()) {
            cout << "Error: Form field 'guess_char' not found or empty." << endl;
            return false;
        }
        guess = guess_iter->getValue();

        if (guess.length() != 1 || !is_uppercase(guess)) {
            cout << "Error: Invalid guess character submitted: '" << guess << "'. Expected single uppercase letter." << endl;
            return false;
        }

        const_form_iterator type_iter = cgi.getElement("guess_type");
        if (type_iter == cgi.getElements().end() || type_iter->getValue().empty()) {
            cout << "Error: Form field 'guess_type' not found or empty." << endl;
            return false;
        }
        type = type_iter->getValue();

        if (type != "maximum" && type != "minimum") {
            cout << "Error: Invalid guess type submitted: '" << type << "'. Expected 'maximum' or 'minimum'." << endl;
            return false;
        }

        file_iterator fileIter = cgi.getFile("pattern_file");


        if (fileIter == cgi.getFiles().end()) {
            cout << "Error: File input field 'pattern_file' not found in the submitted form data." << endl;
            return false;
        }

        const FormFile& fileRef = *fileIter;

        if (fileRef.getDataLength() == 0) {
            cout << "Error: Uploaded file 'pattern_file' is empty." << endl;
            return false;
        }

        stringstream ss_file_content(fileRef.getData());
        string line;

        size_t rows = 0;
        size_t cols = 0;


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

        pattern.clear();
        size_t actual_rows_read = 0;

        while (getline(ss_file_content, line)) {
            string trimmed_line = trim(line);

            if (!trimmed_line.empty()) {

                if (trimmed_line.length() == cols && is_uppercase(trimmed_line)) {
                    pattern.push_back(trimmed_line);
                    actual_rows_read++;
                } else {
                    cout << "Error: Invalid pattern line format or content found. Line: '" << trimmed_line << "'. Expected " << cols << " uppercase characters." << endl;
                    return false;
                }
            }
        }

        if (actual_rows_read != rows) {
            cout << "Error: Number of valid pattern rows read (" << actual_rows_read
                 << ") does not match the expected number of rows (" << rows
                 << ") specified in the first line." << endl;
            return false;
        }

        return true;

    } catch (const exception& e) {
        cout << "Exception caught during form data processing: " << e.what() << endl;
        return false;
    } catch (...) {
        cout << "Unknown exception caught during form data processing." << endl;
        return false;
    }
}