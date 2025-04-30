#include <iostream>
#include <string>
#include <vector>
#include "get_validate_input.h"
#include "game_function.h"
#include "style.h" // For HTML generation

int main() {
    // Set the content type for HTML output
    std::cout << "Content-Type: text/html\n\n";

    // Get and validate user input
    UserInput userInput = getAndValidateInput();

    // Check for errors during input validation
    if (userInput.error) {
        generateErrorPage(userInput.errorMessage); // Use style.cpp for error page
        return 0;
    }

    // The pattern is now ALWAYS read from the uploaded file.  The check is done in get_validate_input.cpp
    std::vector<std::vector<char>> pattern = userInput.pattern;

    // Analyze the pattern
    int charCounts[26];
    countCharacterOccurrences(pattern, charCounts); // Use game_function.cpp
    char actualChar;
    if (userInput.guessType == "max") {
        actualChar = findMaxCountCharacter(charCounts); // Use game_function.cpp
    } else {
        actualChar = findMinCountCharacter(charCounts); // Use game_function.cpp
    }

    // Generate the HTML response using functions from style.cpp
    generateHTMLHeader();
    generateGameTitle();
    generatePatternDisplay(pattern); // Use style.cpp for pattern display
    generateGuessResult(userInput.guessChar, actualChar, userInput.guessType); // Use style.cpp
    if (userInput.showPattern) {
        generatePatternDisplay(pattern); //show pattern
    }
    if (userInput.showCounts) {
        generateCharacterCounts(charCounts); //show counts
    }
    generateHTMLFooter();

    return 0;
}
