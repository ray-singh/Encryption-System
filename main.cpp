#include <iostream>
#include <fstream>
#include <string>
#include <unordered_set>
#include <vector>
#include <algorithm>
#include <sstream>
#include <stdexcept>
#include <map>

// Function to open a file and read all text from it into a vector of strings
std::vector<std::string> OpenFile(std::ifstream& file, const std::string& filename) {
    file.open(filename);
    if (!file.is_open()) {
        std::cerr << "Error: Cannot open '" + filename + "'." << std::endl;
        std::exit(1);
    }
    std::vector<std::string> text;
    std::string line;
    while (std::getline(file, line)) {
        if (line.empty()) {
            text.push_back("");
        } else {
            text.push_back(line);
        }
    }
    file.close();
    return text;
}

// Function to read encoding pairs from a string and store them in a vector of pairs
std::vector<std::pair<char, char>> ReadEncodingPairs(std::string& line) {
    std::vector<std::pair<char, char>> encoding_pairs;
    if (line.empty()) {
        std::cerr << "Error: No encoding pairs provided." << std::endl;
        std::exit(1);
    }
    std::stringstream ss(line);
    std::string pair_str;
    while (ss >> pair_str) {
        if (pair_str.length() != 2 || pair_str[0] == ' ' || pair_str[1] == ' ') {
            std::cerr << "Error: Unknown encoding '" + pair_str + "'." << std::endl;
            std::exit(1);
        }
        auto it = std::find_if(encoding_pairs.begin(), encoding_pairs.end(),
                               [&](const std::pair<char, char>& p) { return p.first == pair_str[0]; });
        if (it != encoding_pairs.end()) {
            std::cerr << "Error: The character '" + std::string(1, pair_str[0])
                      << "' is encoded as both '" + std::string(1, it->second)
                      << "' and '" + std::string(1, pair_str[1]) + "'." << std::endl;
            std::exit(1);
        }
        encoding_pairs.push_back(std::make_pair(pair_str[0], pair_str[1]));
    }
    return encoding_pairs;
}

// Function to apply encoding to a string based on encoding pairs
std::string ApplyEncoding(const std::string& line,
                          const std::vector<std::pair<char, char>>& encoding_pairs) {
    std::string result;
    for (char c : line) {
        auto it = std::find_if(encoding_pairs.begin(), encoding_pairs.end(),
                               [&](const std::pair<char, char>& p) { return p.first == c; });
        if (it != encoding_pairs.end()) {
            result += it->second;
        } else {
            result += c;
        }
    }
    return result;
}

// Function to process lines from a vector, apply encoding, and remove duplicates
std::ostringstream ProcessFile(const std::vector<std::string>& lines,
                               const std::vector<std::pair<char, char>>& encoding_pairs) {
    std::ostringstream oss;
    std::unordered_set<std::string> unique_lines;
    for (std::string l : lines) {
        std::string transformed_line = ApplyEncoding(l, encoding_pairs);
        if (unique_lines.insert(transformed_line).second) {
            oss << transformed_line << "\n";
        }
    }
    return oss;
}

int main() {
    std::string input_val; // Variable for user input
    std::string file_name; // Variable to store the file name

    std::cin >> input_val; // Read user input
    std::cin >> file_name; // Read the file name from user input

    std::string codes;
    std::getline(std::cin, codes); // Read encoding pairs from user input

    std::ifstream input_file;
    std::vector<std::string> line_list = OpenFile(input_file, file_name); // Open and read the file

    std::vector<std::pair<char, char>> encoding_pair = ReadEncodingPairs(codes); // Parse encoding pairs
    std::ostringstream unique_line = ProcessFile(line_list, encoding_pair); // Process and remove duplicates
    std::cout << unique_line.str(); // Print the result

    return 0;
}
