#ifndef STYLE_H
#define STYLE_H

#include <string>
#include <vector>

// Function to generate the HTML header
void generateHTMLHeader();

// Function to generate the game title
void generateGameTitle();

// Function to display the pattern
void generatePatternDisplay(const std::vector<std::vector<char>> &pattern);

// Function to display the guess result
void generateGuessResult(char guessChar, char actualChar,
                         const std::string &guessType);

void generateCharacterCounts(const int counts[26]);

// Function to generate an error page
void generateErrorPage(const std::string &errorMessage);

// Function to generate the HTML footer
void generateHTMLFooter();

#endif 
