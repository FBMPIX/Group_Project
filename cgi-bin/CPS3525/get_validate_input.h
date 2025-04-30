#include <string>
#include <vector>

// Structure to hold the user input
struct UserInput {
  std::vector<std::vector<char>> pattern;
  char guessChar;
  std::string guessType;
  int gridSize;
  bool showPattern;
  bool showCounts;
  bool fileUploaded;
  bool error;
  std::string errorMessage;
};

// Function to read pattern from file
UserInput readPatternFromFile(std::istream &inputFile);

// Function to get and validate user input from the form
UserInput getAndValidateInput();

#endif 
