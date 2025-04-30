#include <iostream>
#include <string>
#include <cstdlib>
#include <sstream>
#include <vector>
#include <cctype>
#include <fstream>
#include "get_validate_input.h"

UserInput readPatternFromFile(std::istream& inputFile) {
    UserInput input;
    input.error = false;
    input.errorMessage = "";
    std::vector<std::vector<char>> pattern;
    std::string line;
    int rows, cols;

    if (std::getline(inputFile, line)) {
        std::stringstream ss(line);
        if (ss >> rows >> cols && rows > 0 && cols > 0) {
            pattern.resize(rows, std::vector<char>(cols));
            for (int i = 0; i < rows; ++i) {
                if (std::getline(inputFile, line) && static_cast<int>(line.length()) == cols) {
                    for (int j = 0; j < cols; ++j) {
                        if (isupper(line[j])) {
                            pattern[i][j] = line[j];
                        } else {
                            input.error = true;
                            input.errorMessage = "Error: Invalid character in pattern file.";
                            return input;
                        }
                    }
                } else {
                    input.error = true;
                    input.errorMessage = "Error: Incorrect number of columns in pattern file.";
                    return input;
                }
            }
            input.pattern = pattern;
        } else {
            input.error = true;
            input.errorMessage = "Error: Invalid dimensions in pattern file.";
            return input;
        }
    } else {
        input.error = true;
        input.errorMessage = "Error: Empty pattern file.";
        return input;
    }
    return input;
}

UserInput getAndValidateInput() {
    UserInput input;
    input.error = false;
    input.errorMessage = "";
    input.fileUploaded = false;
    input.pattern = {};
    input.guessChar = ' ';
    input.guessType = "";
    input.gridSize = 0;
    input.showPattern = false;
    input.showCounts = false;

    const char* contentType = getenv("CONTENT_TYPE");
    const char* contentLengthStr = getenv("CONTENT_LENGTH");

    if (contentType && std::string(contentType).find("multipart/form-data") != std::string::npos && contentLengthStr) {
        long contentLength = std::stol(contentLengthStr);
        std::string postData(contentLength, ' ');
        std::cin.read(&postData[0], contentLength);

        std::string boundary = "";
        size_t boundaryPos = std::string(contentType).find("boundary=");
        if (boundaryPos != std::string::npos) {
            boundary = std::string(contentType).substr(boundaryPos + 9);
            if (boundary.length() > 0) {
                boundary = "--" + boundary;
            }
        }

        if (boundary.empty()) {
            input.error = true;
            input.errorMessage = "Error: Could not find boundary in content type.";
            return input;
        }

        size_t currentPos = 0;
        while (currentPos < postData.length()) {
            size_t nextBoundaryPos = postData.find(boundary, currentPos);
            if (nextBoundaryPos == std::string::npos) {
                break; // No more boundaries found
            }

            size_t dispositionPos = postData.find("Content-Disposition:", nextBoundaryPos);
            if (dispositionPos != std::string::npos) {
                size_t namePos = postData.find("name=\"", dispositionPos);
                if (namePos != std::string::npos) {
                    size_t nameStart = namePos + 6;
                    size_t nameEnd = postData.find("\"", nameStart);
                    if (nameEnd != std::string::npos) {
                        std::string fieldName = postData.substr(nameStart, nameEnd - nameStart);

                        size_t dataStartPos = postData.find("\r\n\r\n", nameEnd);
                        if (dataStartPos != std::string::npos) {
                            dataStartPos += 4;
                            size_t dataEndPos = postData.find(boundary, dataStartPos);
                            if (dataEndPos != std::string::npos) {
                                std::string fieldValue = postData.substr(dataStartPos, dataEndPos - dataStartPos);

                                if (fieldName == "guess_char") {
                                    if (fieldValue.length() == 1 && isupper(fieldValue[0])) {
                                        input.guessChar = fieldValue[0];
                                    } else {
                                        input.error = true;
                                        input.errorMessage = "Error: Invalid guess character.";
                                        return input;
                                    }
                                } else if (fieldName == "guess_type") {
                                    if (fieldValue == "max" || fieldValue == "min") {
                                        input.guessType = fieldValue;
                                    } else {
                                        input.error = true;
                                        input.errorMessage = "Error: Invalid guess type.";
                                        return input;
                                    }
                                } else if (fieldName == "grid_size") {
                                    try {
                                        input.gridSize = std::stoi(fieldValue);
                                    } catch (const std::invalid_argument& e) {
                                        input.error = true;
                                        input.errorMessage = "Error: Invalid grid size.";
                                        return input;
                                    }
                                } else if (fieldName == "show_pattern") {
                                    input.showPattern = (fieldValue == "yes");
                                } else if (fieldName == "show_counts") {
                                    input.showCounts = (fieldValue == "yes");
                                } else if (fieldName == "pattern_file") {
                                    // Handle file upload
                                    size_t fileContentStart = postData.find("\r\n\r\n", dataStartPos) + 4;
                                    size_t fileContentEnd = postData.find(boundary, dataStartPos);

                                    if (fileContentStart != std::string::npos && fileContentEnd != std::string::npos && fileContentStart < fileContentEnd) {
                                        std::string fileContent = postData.substr(fileContentStart, fileContentEnd - fileContentStart - 2); // -2 to remove CRLF before boundary
                                        std::stringstream fileStream(fileContent);

                                        //check if file is empty
                                        if (fileContent.length() == 0) {
                                            input.error = true;
                                            input.errorMessage = "Error: Uploaded file is empty.";
                                            return input;
                                        }
                                        input = readPatternFromFile(fileStream); // Use the helper function
                                        if (!input.error) {
                                            input.fileUploaded = true;
                                        } else {
                                            return input;
                                        }
                                    }  else {
                                         input.error = true;
                                         input.errorMessage = "Error: Could not read file content.";
                                         return input;
                                    }
                                }
                            }
                        }
                    }
                }
            }
            currentPos = nextBoundaryPos + boundary.length();
        }
    } else {
        input.error = true;
        input.errorMessage = "Error: Not a POST request with multipart/form-data.";
        return input;
    }

    if (!input.fileUploaded) {
        input.error = true;
        input.errorMessage = "Error: Pattern file is required.";
        return input;
    }

    if (input.guessChar == ' ' || input.guessType == "") {
        input.error = true;
        input.errorMessage = "Error: Missing guess character or guess type.";
        return input;
    }

    return input;
}
