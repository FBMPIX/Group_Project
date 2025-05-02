#ifndef GET_VALIDATE_INPUT_H
#define GET_VALIDATE_INPUT_H

#include <iostream>
#include <string>
#include <vector>
#include <sstream>
#include <cstdlib>
using namespace std;

namespace get_validate_input {
    string trim(const string& str);
    bool is_uppercase(const string& str);
    bool get_form_data(vector<string>& pattern, string& guess, int& occurrence, bool& convertToNumber);
} // namespace get_validate_input

#endif
