#include <iostream>
#include <string>
#include <vector>
#include <sstream>
#include <cstdlib>
#include "get_validate_input.h"

using namespace std;

// Function to trim leading and trailing whitespace from a string
string get_validate_input::trim(const string& str) {
    size_t first = str.find_first_not_of(' ');
    if (string::npos == first) {
        return str;
    }
    size_t last = str.find_last_not_of(' ');
    return str.substr(first, (last - first + 1));
}

// Function to validate if a string contains only uppercase letters
bool get_validate_input::is_uppercase(const string& str) {
    for (char c : str) {
        if (c < 'A' || c > 'Z') {
            return false;
        }
    }
    return true;
}

// Function to get form data and validate it
bool get_validate_input::get_form_data(vector<string>& pattern, string& guess, string& type) {
    char* post_data = getenv("CONTENT_LENGTH");
    if (post_data == nullptr || atoi(post_data) <= 0) {
        cerr << "Content length is not valid." << endl;
        return false;
    }

    long content_length = atoi(post_data);
    vector<char> buffer(content_length);
    cin.read(buffer.data(), content_length);
    string post_string(buffer.begin(), buffer.end());

    // Basic parsing of the multipart/form-data (simplified)
    string file_content;
    size_t file_content_start = post_string.find("\r\n\r\n");
    if (file_content_start != string::npos) file_content_start += 4;
    size_t file_content_end = post_string.find("\r\n------", file_content_start);
    if (file_content_start != string::npos && file_content_end != string::npos) {
        file_content = post_string.substr(file_content_start, file_content_end - file_content_start);
        stringstream ss(file_content);
        string line;
        int rows = 0;
        int cols = 0;
        if (getline(ss, line)) {
            stringstream ss_dims(line);
            if (!(ss_dims >> rows >> cols)) return false;
        }
        while (getline(ss, line)) {
            string trimmed_line = trim(line);
            if (trimmed_line.length() == cols && is_uppercase(trimmed_line)) {
                pattern.push_back(trimmed_line);
            } else if (!trimmed_line.empty()) {
                cerr << "Invalid pattern line: " << trimmed_line << endl;
                return false;
            }
        }
        if (pattern.size() != rows) return false;
    } else {
        cerr << "Could not find file content in POST data." << endl;
        return false;
    }

    size_t guess_start = post_string.find("name=\"guess_char\"\r\n\r\n") + 20;
    size_t guess_end = post_string.find("\r\n------", guess_start);
    if (guess_start == string::npos || guess_end == string::npos) {
        cerr << "Could not find guess_char in POST data." << endl;
        return false;
    }
    guess = trim(post_string.substr(guess_start, guess_end - guess_start));
    if (guess.length() != 1 || !is_uppercase(guess)) {
        cerr << "Invalid guess character." << endl;
        return false;
    }

    size_t type_start = post_string.find("name=\"guess_type\"\r\n\r\n") + 18;
    size_t type_end = post_string.find("\r\n------", type_start);
    if (type_start == string::npos || type_end == string::npos) {
        cerr << "Could not find guess_type in POST data." << endl;
        return false;
    }
    type = trim(post_string.substr(type_start, type_end - type_start));
    if (type != "maximum" && type != "minimum") {
        cerr << "Invalid guess type." << endl;
        return false;
    }

    return true;
}
