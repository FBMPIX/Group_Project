#ifndef GENERATE_HTML_H
#define GENERATE_HTML_H

#include <iostream>
#include <string>
#include <vector>
#include <map>
using namespace std;

namespace generate_html {
    string generate_html_header(const string& title);
    string generate_html_footer();
    string generate_heading(const string& text, int level);
    string generate_paragraph(const string& text);
    string generate_pattern_table(const vector<string>& pattern);
    string generate_character_counts(const map<char, int>& counts);
    string generate_result_message(bool correct, const string& guess, string& type, const vector<char>& correct_chars, const map<char, int>& counts);
} 

#endif 
