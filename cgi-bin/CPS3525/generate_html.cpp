#include <iostream>
#include <string>
#include <vector>
// #include <map> // Removed map include as generate_character_counts is removed
#include "generate_html.h" // Assuming this header declares the generate_html class and its functions

using namespace std;

// Function to generate the basic HTML header for the response page
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
           "        .correct { color: green; font-weight: bold; }\n" // Style for correct guess message
           "        .incorrect { color: red; font-weight: bold; }\n" // Style for incorrect guess message
           "    </style>\n"
           "</head>\n"
           "<body>\n"
           "    <div class=\"container\">\n";
}

// Function to generate the basic HTML footer to close the document structure
string generate_html::generate_html_footer() {
    return "    </div>\n"
           "</body>\n"
           "</html>";
}

// Function to generate an HTML heading tag (h2 or h3)
string generate_html::generate_heading(const string& text, int level) {
    if (level == 2) return "<h2>" + text + "</h2>\n";
    if (level == 3) return "<h3>" + text + "</h3>\n";
    return "<h2>" + text + "</h2>\n"; // Default to h2 if level is not 2 or 3
}

// Function to generate an HTML paragraph tag
string generate_html::generate_paragraph(const string& text) {
    return "<p>" + text + "</p>\n";
}

// Function to generate the 2D pattern grid as an HTML table for display
string generate_html::generate_pattern_table(const vector<string>& pattern) {
    if (pattern.empty()) {
        return "<p>No pattern grid to display.</p>\n"; // Updated text
    }
    string table = "<table class=\"pattern-table\">\n"; // Removed paragraph tag here, will add label in main
    for (const auto& row : pattern) {
        table += "<tr>\n";
        for (char c : row) {
            table += "<td>" + string(1, c) + "</td>\n";    // Each character in its own table cell
        }
        table += "</tr>\n";
    }
    table += "</table>\n";
    return table;
}

// Removed generate_character_counts function as it's not needed for the core word search result display

// Function to generate the result message for the word search game
// This function compares the guessed occurrence count with the actual count and generates corresponding HTML
string generate_html::generate_result_message(bool correct, const string& guessed_pattern, int guessed_occurrence, int actual_occurrence_count) {
    string message;

    // Display the user's guess for the pattern and its occurrences
    message += generate_paragraph("Your guess: " + to_string(guessed_occurrence) + " occurrences of \"" + guessed_pattern + "\"");

    // Display the actual count of the pattern found in the grid
    message += generate_paragraph("Actual occurrences found: " + to_string(actual_occurrence_count));

    // Generate the final result message based on whether the guess was correct
    if (correct) {
        message += generate_heading("Congratulations! Your guess is correct.", 3);
        message += generate_paragraph("<span class=\"correct\">The pattern \"" + guessed_pattern + "\" was found exactly " + to_string(actual_occurrence_count) + " times.</span>");
    } else {
        message += generate_heading("Sorry, your guess is incorrect.", 3);
        message += generate_paragraph("<span class=\"incorrect\">You guessed " + to_string(guessed_occurrence) + " occurrences, but the pattern \"" + guessed_pattern + "\" was found " + to_string(actual_occurrence_count) + " times.</span>");
    }

    return message;
}
