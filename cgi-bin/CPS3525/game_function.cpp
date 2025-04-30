#include <iostream>
#include <vector>
#include <algorithm>
#include <iomanip>

// Function to count character occurrences
void countCharacterOccurrences(const std::vector<std::vector<char>>& pattern, int counts[26]) {
    for (int i = 0; i < 26; ++i) {
        counts[i] = 0;
    }
    for (const auto& row : pattern) {
        for (char c : row) {
            counts[c - 'A']++;
        }
    }
}

// Function to find the character with the maximum count
char findMaxCountCharacter(const int counts[26]) {
    int maxCount = -1;
    char maxChar = ' ';
    for (int i = 0; i < 26; ++i) {
        if (counts[i] > maxCount) {
            maxCount = counts[i];
            maxChar = 'A' + i;
        }
    }
    return maxChar;
}

// Function to find the character with the minimum count
char findMinCountCharacter(const int counts[26]) {
    int minCount = -1;
    char minChar = ' ';
    for (int i = 0; i < 26; ++i) {
        if (minCount == -1 || counts[i] < minCount) {
            minCount = counts[i];
            minChar = 'A' + i;
        }
    }
    return minChar;
}
