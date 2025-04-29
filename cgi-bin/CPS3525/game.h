#ifndef GAME_H
#define GAME_H

#include <vector>
#include <string>
#include <tuple>
#include <random>

using namespace std;

tuple<vector<vector<char>>, vector<vector<bool>>, vector<string>> generateWordSearch(
    const vector<string>& wordList,
    int numWords,
    int width,
    int height,
    bool allowVertical,
    bool allowReverse
);

bool tryPlaceWord(
    vector<vector<char>>& grid,
    vector<vector<bool>>& highlightGrid,
    const string& word,
    int width,
    int height,
    bool allowVertical,
    bool allowReverse,
    mt19937& rng
);

bool canPlace(
    const vector<vector<char>>& grid,
    const string& word,
    int r, int c,
    int dr, int dc,
    int width,
    int height
);

char getRandomLetter(mt19937& rng);

#endif // GAME_H
