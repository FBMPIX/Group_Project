#ifndef GAME_FUNCTION_H
#define GAME_FUNCTION_H

#include <vector>

// Function to count character occurrences
void countCharacterOccurrences(const std::vector<std::vector<char>> &pattern,
                               int counts[26]);

// Function to find the character with the maximum count
char findMaxCountCharacter(const int counts[26]);

// Function to find the character with the minimum count
char findMinCountCharacter(const int counts[26]);

#endif // GAME_FUNCTION_H
