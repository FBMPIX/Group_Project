#include <iostream>
#include <string>
#include <vector>
#include <iomanip>

// Function to generate the HTML header
void generateHTMLHeader() {
    std::cout << "<!DOCTYPE html>\n";
    std::cout << "<html>\n";
    std::cout << "<head>\n";
    std::cout << "    <title>Pattern Guessing Game</title>\n";
    std::cout << "    <style>\n";
    std::cout << "      body { font-family: sans-serif; }\n";
    std::cout << "      .container { margin: 20px; }\n";
    std::cout << "      h1 { text-align: center; }\n";
    std::cout << "      .pattern-display { font-family: monospace; background-color: #f0f0f0; padding: 10px; margin: 20px auto; width: fit-content; }\n";
    std::cout << "      .result { font-weight: bold; font-size: 1.2em; text-align: center; }\n";
    std::cout << "      .error { color: red; }\n";
    std::cout << "       table, th, td {border: 1px solid black; border-collapse: collapse;}\n";
    std::cout << "    </style>\n";
    std::cout << "</head>\n";
    std::cout << "<body>\n";
    std::cout << "<div class=\"container\">\n";
}

// Function to generate the game title
void generateGameTitle() {
    std::cout << "    <h1>Pattern Guessing Game</h1>\n";
}

// Function to display the pattern
void generatePatternDisplay(const std::vector<std::vector<char>>& pattern) {
    std::cout << "    <div class=\"pattern-display\">\n";
    for (const auto& row : pattern) {
        for (char c : row) {
            std::cout << c << " ";
        }
        std::cout << "<br>\n";
    }
    std::cout << "    </div>\n";
}

// Function to display the guess result
void generateGuessResult(char guessChar, char actualChar, const std::string& guessType) {
    std::cout << "    <div class=\"result\">\n";
    std::cout << "        <p>Your Guess: " << guessChar << " for " << guessType << "</p>\n";
    if (guessChar == actualChar) {
        std::cout << "        <p>Congratulations! Your guess is correct!</p>\n";
    } else {
        std::cout << "        <p>Sorry, your guess is incorrect.</p>\n";
        std::cout << "        <p>The character with the " << guessType << " count is: " << actualChar << "</p>\n";
    }
    std::cout << "    </div>\n";
}

void generateCharacterCounts(const int counts[26]) {
    std::cout << "    <h2>Character Counts:</h2>\n";
    std::cout << "    <table border=\"1\">\n";
    std::cout << "        <tr><th>Character</th><th>Count</th></tr>\n";
    for (int i = 0; i < 26; ++i) {
        std::cout << "        <tr><td>" << (char)('A' + i) << "</td><td>" << counts[i] << "</td></tr>\n";
    }
    std::cout << "    </table>\n";
}

// Function to generate an error page
void generateErrorPage(const std::string& errorMessage) {
    std::cout << "<!DOCTYPE html>\n";
    std::cout << "<html>\n";
    std::cout << "<head>\n";
    std::cout << "    <title>Error</title>\n";
    std::cout << "    <style>\n";
    std::cout << "      body { font-family: sans-serif; background-color: #f8f8f8; }\n";
    std::cout << "      .error-container { margin: 50px auto; padding: 20px; border: 1px solid #d3d3d3; background-color: #fff; width: 500px; }\n";
    std::cout << "      h1 { color: #ff0000; }\n";
    std::cout << "      p { font-size: 1.1em; }\n";
    std::cout << "    </style>\n";
    std::cout << "</head>\n";
    std::cout << "<body>\n";
    std::cout << "    <div class=\"error-container\">\n";
    std::cout << "        <h1>Error</h1>\n";
    std::cout << "        <p class=\"error\">" << errorMessage << "</p>\n";
    std::cout << "    </div>\n";
    std::cout << "</body>\n";
    std::cout << "</html>\n";
}

// Function to generate the HTML footer
void generateHTMLFooter() {
    std::cout << "</div>";
    std::cout << "</body>\n";
    std::cout << "</html>\n";
}
