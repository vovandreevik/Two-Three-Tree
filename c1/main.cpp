#include <iostream>
#include <string>
#include <sstream>
#include <cctype>  
#include <vector>
#include <fstream>
#include "TwoThreeTree.h"

void buildTreeFromFile(const std::string& filename, TwoThreeTree& tree);

int main() {
    try {
        TwoThreeTree tree;
        buildTreeFromFile("test.txt", tree);

        std::cout << std::endl << "Tree:" << std::endl;
        std::vector<std::string> wordList;
        tree.print(wordList);

        std::cout << std::endl;

        // Test insert
        TwoThreeTree test;

        std::cout << "Testing insert operation:" << std::endl;
        std::cout << " insert banana" << std::endl;
        test.insert("banana", 20);

        std::cout << std::endl;

        // Test search
        std::cout << "Testing search operation:" << std::endl;
        Node* result = test.search("banana");

        if (result) {
            std::cout << "Word found: " << result->keys[0] << std::endl;
        }
        else {
            std::cout << "Word not found." << std::endl;
        }

        std::cout << std::endl;

        // Test remove
        std::cout << "Testing remove operation:" << std::endl;
        test.remove("banana");

        result = test.search("banana");
        if (result) {
            std::cout << "Word found: " << result->keys[0] << std::endl;
        }
        else {
            std::cout << "Word not found." << std::endl;
        }
    }
    catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
    }

    return 0;
}

void buildTreeFromFile(const std::string& filename, TwoThreeTree& tree) {
    std::ifstream inputFile(filename);
    if (!inputFile) {
        throw std::runtime_error("Failed to open the input file.");
    }

    std::string line;
    int lineNumber = 1;
    while (std::getline(inputFile, line)) {
        std::istringstream iss(line);
        std::string word;
        std::cout << lineNumber << " ";
        while (iss >> word) {
            std::cout << word << " ";
            word.erase(std::remove_if(word.begin(), word.end(), [](char c) {
                return std::ispunct(static_cast<unsigned char>(c)) or std::isdigit(static_cast<unsigned char>(c));
                }), word.end());

            if (!word.empty()) {
                tree.insert(word, lineNumber);
            }
        }
        lineNumber++;
        std::cout << std::endl;
    }

    inputFile.close();
}