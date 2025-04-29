#include "game.h"
#include <iostream>
#include <algorithm>
#include <iomanip>

using namespace std;

/**
 * @brief Generates a random uppercase letter ('A' to 'Z').
 */
char getRandomLetter(mt19937& rng) {
    uniform_int_distribution<int> dist('A', 'Z');
    return static_cast<char>(dist(rng));
}

/**
 * @brief Checks if a word can be placed at a given position and direction.
 * @param grid The current state of the word search grid.
 * @param word The word to check placement for.
 * @param r Starting row.
 * @param c Starting column.
 * @param dr Row direction (-1=up, 0=no change, 1=down).
 * @param dc Column direction (-1=left, 0=no change, 1=right).
 * @param width Grid width.
 * @param height Grid height.
 * @return True if the word can be placed, false otherwise.
 */
bool canPlace(const vector<vector<char>>& grid, const string& word, int r, int c, int dr, int dc, int width, int height) {
    for (size_t i = 0; i < word.length(); ++i) {
        int nr = r + i * dr;
        int nc = c + i * dc;
        if (nr < 0 || nr >= height || nc < 0 || nc >= width) {
            return false; // Out of bounds
        }
        // Check for conflicts: Cell must be empty ('\0') or contain the same letter
        if (grid[nr][nc] != '\0' && grid[nr][nc] != word[i]) {
            return false; // Conflict with existing letter
        }
    }
    return true; // Placement possible
}

/**
 * @brief Attempts to place a word onto the grid randomly.
 * @param grid The character grid (will be modified if placement is successful).
 * @param highlightGrid The boolean grid marking placed words (will be modified).
 * @param word The word to place.
 * @param width Grid width.
 * @param height Grid height.
 * @param allowVertical Allow placing words vertically.
 * @param allowReverse Allow placing words backwards (left or up).
 * @param rng Mersenne Twister random number engine.
 * @return True if the word was successfully placed, false otherwise.
 */
bool tryPlaceWord(vector<vector<char>>& grid, vector<vector<bool>>& highlightGrid, const string& word, int width, int height, bool allowVertical, bool allowReverse, mt19937& rng) {
    int maxPlacementAttempts = 100; // Limit attempts to avoid long waits
    vector<pair<int, int>> directions;
    directions.push_back({0, 1}); // Right
    if (allowReverse) directions.push_back({0, -1}); // Left
    if (allowVertical) directions.push_back({1, 0}); // Down
    if (allowVertical && allowReverse) directions.push_back({-1, 0}); // Up
    // Note: Diagonal directions could be added here if desired

    shuffle(directions.begin(), directions.end(), rng); // Randomize direction order

    uniform_int_distribution<int> rowDist(0, height - 1);
    uniform_int_distribution<int> colDist(0, width - 1);

    for (int attempt = 0; attempt < maxPlacementAttempts; ++attempt) {
        int r = rowDist(rng); // Random start row
        int c = colDist(rng); // Random start column
        for (const auto& dir : directions) {
            int dr = dir.first;
            int dc = dir.second;
            if (canPlace(grid, word, r, c, dr, dc, width, height)) {
                // Place the word if possible
                for (size_t i = 0; i < word.length(); ++i) {
                    int place_r = r + i * dr;
                    int place_c = c + i * dc;
                    grid[place_r][place_c] = word[i];
                    highlightGrid[place_r][place_c] = true; // Mark for highlight
                }
                return true; // Word placed successfully
            }
        }
    }
    return false; // Failed to place word after attempts
}


/**
 * @brief Generates the complete word search puzzle grid and related data.
 * @param wordList The initial list of words to potentially include.
 * @param numWords The target number of words to place in the grid.
 * @param width The desired width of the grid.
 * @param height The desired height of the grid.
 * @param allowVertical Allow vertical word placement.
 * @param allowReverse Allow backwards word placement.
 * @return A std::tuple containing:
 * - std::vector<std::vector<char>>: The generated character grid.
 * - std::vector<std::vector<bool>>: A grid marking the positions of placed words.
 * - std::vector<std::string>: A list of the words that were actually placed.
 * Returns an empty tuple ({}, {}, {}) if generation fails significantly.
 */
tuple<vector<vector<char>>, vector<vector<bool>>, vector<string>> generateWordSearch(
    const vector<string>& wordList,
    int numWords,
    int width,
    int height,
    bool allowVertical,
    bool allowReverse)
{
    vector<vector<char>> grid;
    vector<vector<bool>> highlightGrid;
    vector<string> placedWords;

    // --- Input Validation ---
    if (width <= 0 || height <= 0 || wordList.empty()) {
        cerr << "Error: Invalid grid dimensions or empty word list." << endl;
        return make_tuple(vector<vector<char>>(), vector<vector<bool>>(), vector<string>());
    }

    // --- Initialize Grids ---
    grid.assign(height, vector<char>(width, '\0')); // Fill grid with null characters
    highlightGrid.assign(height, vector<bool>(width, false)); // Fill highlight map with false

    // --- Prepare Word List ---
    vector<string> availableWords = wordList;
    int maxDim = max(width, height); // Max length a word can have

    // Remove words that are too long to fit
    availableWords.erase(
        remove_if(availableWords.begin(), availableWords.end(),
                   [&](const string& w) { return w.length() > maxDim; }),
        availableWords.end());

    if (availableWords.empty()) {
        cerr << "Error: No words in the list are short enough to fit in the grid." << endl;
        return make_tuple(vector<vector<char>>(), vector<vector<bool>>(), vector<string>());
    }

    // --- Randomization and Placement ---
    random_device rd;        // Obtain a random seed from the hardware/OS
    mt19937 rng(rd());       // Seed the Mersenne Twister engine
    shuffle(availableWords.begin(), availableWords.end(), rng); // Shuffle words for random placement order

    // Determine actual number of words to place
    int wordsToPlaceCount = min(numWords, (int)availableWords.size());
     if (wordsToPlaceCount <= 0) {
         cerr << "Error: Number of words to place is zero or negative." << endl;
         return make_tuple(vector<vector<char>>(), vector<vector<bool>>(), vector<string>());
    }

    int placedCount = 0;
    // Attempt to place words from the shuffled list
    for (int i = 0; i < availableWords.size() && placedCount < wordsToPlaceCount; ++i) {
        if (tryPlaceWord(grid, highlightGrid, availableWords[i], width, height, allowVertical, allowReverse, rng)) {
            placedWords.push_back(availableWords[i]);
            placedCount++;
        } else {
             // Optional: Log failure for debugging
             cerr << "Warning: Could not find a position for word '" << availableWords[i] << "'." << endl;
        }
    }

     // --- Check if any words were placed ---
     if (placedCount == 0) {
        cerr << "Error: Failed to place any words into the grid. Try different parameters or words." << endl;
        return make_tuple(vector<vector<char>>(), vector<vector<bool>>(), vector<string>());
    }

    // --- Fill Remaining Empty Cells ---
    for (int r = 0; r < height; ++r) {
        for (int c = 0; c < width; ++c) {
            if (grid[r][c] == '\0') { // If cell is empty
                grid[r][c] = getRandomLetter(rng); // Fill with random letter
            }
        }
    }

    // --- Return Results ---
    // Package the grid, highlight map, and placed words list into a tuple
    return make_tuple(grid, highlightGrid, placedWords);
}


// --- Main Function (Example Usage / Test Driver) ---
// This function demonstrates how to use the generateWordSearch function.
// It's not strictly part of the library but useful for testing.
int main() {
    // --- Configuration ---
    vector<string> words = {"HELLO", "WORLD", "PUZZLE", "SEARCH", "GRID", "CODE", "CPP", "VECTOR", "STRING", "ALGORITHM", "RANDOM", "GENERATOR", "CONSOLE", "OUTPUT", "HEADER", "IMPLEMENT"};
    int numWordsToPlace = 3;
    int gridWidth = 10;
    int gridHeight = 10;
    bool allowVertical = true;
    bool allowReverse = true;

    // --- Print Settings ---
    cout << "Generating " << gridWidth << "x" << gridHeight << " word search puzzle..." << endl;
    cout << "Attempting to place " << numWordsToPlace << " words." << endl;
    cout << "Allow Vertical: " << (allowVertical ? "Yes" : "No") << endl;
    cout << "Allow Reverse: " << (allowReverse ? "Yes" : "No") << endl;
    cout << "------------------------------------" << endl;


    // --- Generate the Puzzle ---
    // Call the function defined above (and declared in game.h)
    auto resultTuple = generateWordSearch(words, numWordsToPlace, gridWidth, gridHeight, allowVertical, allowReverse);

    // --- Unpack the Tuple Results ---
    // Use std::get<index>(tuple) to access the elements
    const auto& grid = get<0>(resultTuple);             // Character grid
    const auto& highlightGrid = get<1>(resultTuple);    // Highlight map
    const auto& placedWords = get<2>(resultTuple);      // List of placed words

    // --- Check if Generation was Successful ---
    // An empty grid indicates failure during generation.
    if (!grid.empty()) {
        cout << "\nPuzzle generated successfully!" << endl;

        // --- Display the Puzzle Grid ---
        cout << "\nPuzzle Grid:" << endl;
        for (int r = 0; r < gridHeight; ++r) {
            for (int c = 0; c < gridWidth; ++c) {
                // Add extra space for better alignment
                cout << grid[r][c] << "  ";
            }
            cout << endl; // Newline after each row
        }

        // --- Display the Highlight Grid (Optional Debugging) ---
        cout << "\nHighlight Grid (1 = Part of a word):" << endl;
         for (int r = 0; r < gridHeight; ++r) {
            for (int c = 0; c < gridWidth; ++c) {
                // Print 0 or 1 and spaces for alignment
                cout << highlightGrid[r][c] << "  ";
            }
            cout << endl;
        }

        // --- Display the List of Placed Words ---
        cout << "\nSuccessfully placed words (" << placedWords.size() << " total):" << endl;
        if (!placedWords.empty()) {
            for (const auto& word : placedWords) {
                cout << "- " << word << endl;
            }
        } else {
             cout << "(None)" << endl; // Should not happen if grid is not empty
        }

    } else {
        // Error messages should have been printed to cerr by generateWordSearch
        cerr << "\nError: Failed to generate the puzzle. Check previous error messages." << endl;
        return 1; // Indicate failure
    }

    return 0; // Indicate successful execution
}
