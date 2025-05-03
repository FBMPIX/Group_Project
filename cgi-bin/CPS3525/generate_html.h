#ifndef GENERATE_HTML_H
#define GENERATE_HTML_H

#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <algorithm>
#include <utility>

using namespace std;

namespace generate_html {
	string generate_html_header(const string& title);;
	string generate_html_footer();
	string generate_heading(const string& text, int level);
	string generate_paragraph(const string& text);
	string generate_pattern_table(const vector<string>& pattern);
	vector<string> generate_numbered_grid(const vector<string>& grid, const string& pattern);
	string generate_result_message(bool correct, const string& guessed_pattern, int guessed_occurrence, int actual_occurrence_count);
}

#endif 
