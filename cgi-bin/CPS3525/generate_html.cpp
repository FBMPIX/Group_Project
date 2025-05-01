#include <iostream>
#include <string>
#include <vector>
#include <map>
#include "generate_html.h"

using namespace std;

// Function to generate the basic HTML header
string generate_html::generate_html_header(const string& title) {
    return "<!DOCTYPE html>\n"
           "<html lang=\"en\">\n"
           "<head>\n"
           "    <meta charset=\"UTF-8\">\n"
           "    <meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0\">\n"
           "    <title>" + title + "</title>\n"
           "    <style>\n"
           "        body { font-family: sans-serif; margin: 20px; background-color: #f4f4f4; }\n"
           "        .container { background-color: #fff; padding: 20px; border-radius: 8px; box-shadow: 0 0 10px rgba(0, 0, 0, 0.1); }\n"
           "        h2, h3 { color: #333; text-align: center; }\n"
           "        p { color: #555; margin-bottom: 10px; }\n"
           "        pre, .pattern-table { background-color: #eee; padding: 10px; border-radius: 4px; overflow-x: auto; }\n"
           "        .pattern-table { border-collapse: collapse; margin-bottom: 15px; }\n"
           "        .pattern-table td { border: 1px solid #ccc; padding: 5px; text-align: center; min-width: 20px; }\n"
           "    </style>\n"
           "</head>\n"
           "<body>\n"
           "    <div class=\"container\">\n";
}

// Function to generate the basic HTML footer
string generate_html::generate_html_footer() {
    return "    </div>\n"
           "</body>\n"
           "</html>";
}

// Function to generate a heading
string generate_html::generate_heading(const string& text, int level) {
    if (level == 2) return "<h2>" + text + "</h2>\n";
    if (level == 3) return "<h3>" + text + "</h3>\n";
    return "<h2>" + text + "</h2>\n"; // Default to h2
}

// Function to generate a paragraph
string generate_html::generate_paragraph(const string& text) {
    return "<p>" + text + "</p>\n";
}

// Function to generate the 2D pattern as an HTML table
string generate_html::generate_pattern_table(const vector<string>& pattern) {
    if (pattern.empty()) {
        return "<p>No pattern to display.</p>\n";
    }
    string table = "<p>Original Pattern:</p>\n<table class=\"pattern-table\">\n";
    for (const auto& row : pattern) {
        table += "<tr>\n";
        for (char c : row) {
            table += "<td>" + string(1, c) + "</td>\n";
        }
        table += "</tr>\n";
    }
    table += "</table>\n";
    return table;
}

// Function to generate preformatted text (for character counts)
string generate_html::generate_character_counts(const map<char, int>& counts) {
    string output = "<p>Character counts:</p><pre>";
    for (const auto& pair : counts) {
        output += pair.first;
        output += ": ";
        output += to_string(pair.second);
        output += "<br>";
    }
    output += "</pre>\n";
    return output;
}

// Function to generate the result message
string generate_html::generate_result_message(bool correct, const string& guess, string& type, const vector<char>& correct_chars, const map<char, int>& counts) {
    string message;
    if (correct) {
        message += "<h3>Congratulations! Your guess for the " + type + " count character is correct.</h3>\n";
        message += "<p>The character(s) with the " + type + " count (";
        if (!correct_chars.empty()) {
            message += correct_chars[0];
            message += " - ";
            message += to_string(counts.at(correct_chars[0]));
        }
        message += ") is/are: ";
        for (size_t i = 0; i < correct_chars.size(); ++i) {
            message += correct_chars[i];
            message += (i < correct_chars.size() - 1 ? ", " : "");
        }
        message += ".</p>\n";
    } else {
        message += "<h3>Sorry, your guess for the " + type + " count character is incorrect.</h3>\n";
        message += "<p>The character(s) with the " + type + " count (";
         if (!correct_chars.empty()) {
            message += correct_chars[0];
            message += " - ";
             message += to_string(counts.at(correct_chars[0]));
        }
        message += ") is/are: ";
        for (size_t i = 0; i < correct_chars.size(); ++i) {
            message += correct_chars[i];
            message += (i < correct_chars.size() - 1 ? ", " : "");
        }
        message += ".</p>\n";
    }
    return message;
}
