#pragma once
#include <iostream>
#include <string>
#include <cctype>  
#include <vector>

struct ListNode {
public:

    int lineNumber;
    ListNode* next;

    ListNode(int lineNumber) : lineNumber(lineNumber), next(nullptr) {}

    ~ListNode() {
        if (next != nullptr) {
            delete next;
        }
    }

    void addLineNumber(int lineNumber) {
        ListNode* newNode = new ListNode(lineNumber);
        ListNode* current = this;
        while (current->next != nullptr) {
            current = current->next;
        }
        current->next = newNode;
    }
};

struct Node {

    int keyCount;
    std::string keys[3];
    ListNode* info[3];
    Node* children[4];

    Node() {
        keyCount = 0;
        for (int i = 0; i < 3; ++i) {
            keys[i] = "";
            info[i] = nullptr;
        }
        for (int i = 0; i < 4; ++i) {
            children[i] = nullptr;
        }
    }
};

class TwoThreeTree {
public:

    TwoThreeTree() {
        root = nullptr;
    }

    ~TwoThreeTree() {
        destroyTree(root);
    }

    void print(std::vector<std::string>& wordList) {
        print(root, wordList);
    }

    void insert(const std::string& word, int lineNumber) {
        std::string lowercaseWord = word;
        for (char& c : lowercaseWord) {
            c = std::tolower(c);
        }

        if (search(root, lowercaseWord) != nullptr) {
            // Word already exists in the tree, update the line number list
            updateLineInfo(lowercaseWord, lineNumber);
            return;
        }

        // Word does not exist in the tree, perform insertion
        if (root == nullptr) {
            root = new Node();
            root->keys[0] = lowercaseWord;
            root->info[0] = new ListNode(lineNumber);
            root->keyCount = 1;
        }
        else {
            if (root->keyCount == 2) {
                Node* newRoot = new Node();
                newRoot->children[0] = root;
                splitChild(newRoot, 0, root);
                root = newRoot; // Update the root
            }
            insertNonFull(root, lowercaseWord, lineNumber);
        }

    }

    Node* search(const std::string& word) {
        return search(root, word);
    }

    void remove(const std::string& word) {
        remove(root, word);
    }

    Node* getRoot() {
        return root;
    }

private:
    Node* root;

    void destroyTree(Node* node) {
        if (node != nullptr) {
            for (int i = 0; i < node->keyCount; ++i) {
                destroyTree(node->children[i]);
            }
            destroyTree(node->children[node->keyCount]);
            delete node;
        }
    }

    void splitChild(Node* parent, int index, Node* child) {
        Node* newNode = new Node();
        newNode->keyCount = 1;
        newNode->children[0] = nullptr;

        newNode->keys[0] = child->keys[1];
        newNode->info[0] = child->info[1];
        newNode->children[1] = child->children[2];

        child->keys[1] = "";
        child->info[1] = nullptr;
        child->children[2] = nullptr;

        child->keyCount--;

        for (int i = parent->keyCount; i > index; --i) {
            parent->children[i + 1] = parent->children[i];
            parent->keys[i] = parent->keys[i - 1];
            parent->info[i] = parent->info[i - 1];
        }

        parent->children[index + 1] = newNode;
        parent->keys[index] = child->keys[0];
        parent->info[index] = child->info[0];
        parent->keyCount++;
    }


    void insertNonFull(Node* node, const std::string& word, int lineNumber) {

        int i = node->keyCount - 1;
        if (node->children[0] == nullptr) {
            // Leaf node
            while (i >= 0 && word < node->keys[i]) {
                node->keys[i + 1] = node->keys[i];
                node->info[i + 1] = node->info[i];
                i--;
            }
            node->keys[i + 1] = word;
            node->info[i + 1] = new ListNode(lineNumber);
            node->keyCount++;
        }
        else {
            // Internal node
            while (i >= 0 && word < node->keys[i]) {
                i--;
            }
            i++;

            if (node->children[i]->keyCount == 2) {
                splitChild(node, i, node->children[i]);
                if (word > node->keys[i]) {
                    i++;
                }
            }
            insertNonFull(node->children[i], word, lineNumber);
        }
    }

    void updateLineInfo(const std::string& word, int lineNumber) {
        Node* node = search(word);
        if (node) {
            for (int i = 0; i < node->keyCount; ++i) {
                if (node->keys[i] == word) {
                    ListNode* current = node->info[i];
                    while (current != nullptr) {
                        if (current->lineNumber == lineNumber) {
                            // Line number already exists, no need to add it again
                            return;
                        }
                        current = current->next;
                    }
                    // Line number doesn't exist, add it to the list
                    node->info[i]->addLineNumber(lineNumber);
                    break;
                }
            }
        }
    }

    Node* search(Node* node, const std::string& word) {
        if (!node)
            return nullptr;

        int i = 0;
        while (i < node->keyCount && word > node->keys[i])
            i++;

        if (i < node->keyCount && word == node->keys[i])
            return node;

        if (node->children[i])
            return search(node->children[i], word);

        return nullptr;
    }

    void remove(Node* node, const std::string& word) {
        if (!node)
            return;

        int i = 0;
        while (i < node->keyCount && word > node->keys[i])
            i++;

        if (i < node->keyCount && word == node->keys[i]) {
            if (node->children[0] == nullptr) {
                // Leaf node
                removeKeyFromLeafNode(node, i);
            }
            else {
                // Internal node
                removeKeyFromInternalNode(node, i);
            }
        }
        else {
            if (node->children[i]) {
                if (node->children[i]->keyCount == 1) {
                    // Rebalance the tree if necessary
                    if (i > 0 && node->children[i - 1]->keyCount > 1) {
                        // Borrow a key from the left sibling
                        borrowFromLeftSibling(node, i);
                    }
                    else if (i < node->keyCount && node->children[i + 1]->keyCount > 1) {
                        // Borrow a key from the right sibling
                        borrowFromRightSibling(node, i);
                    }
                    else {
                        // Merge with siblings
                        mergeWithSiblings(node, i);
                    }
                }
                remove(node->children[i], word);
            }
        }
    }

    void removeKeyFromLeafNode(Node* node, int index) {
        node->keyCount--;
        delete node->info[index];
        for (int i = index; i < node->keyCount; ++i) {
            node->keys[i] = node->keys[i + 1];
            node->info[i] = node->info[i + 1];
        }
        node->keys[node->keyCount] = "";
        node->info[node->keyCount] = nullptr;
    }

    void removeKeyFromInternalNode(Node* node, int index) {
        Node* predecessor = findPredecessor(node, index);
        std::string predecessorKey = predecessor->keys[predecessor->keyCount - 1];
        ListNode* predecessorInfo = predecessor->info[predecessor->keyCount - 1];

        if (predecessor->children[0] == nullptr) {
            // Predecessor is a leaf node
            removeKeyFromLeafNode(predecessor, predecessor->keyCount - 1);
        }
        else {
            // Predecessor is an internal node
            removeKeyFromInternalNode(predecessor, predecessor->keyCount - 1);
        }

        node->keys[index] = predecessorKey;
        node->info[index] = predecessorInfo;
    }

    Node* findPredecessor(Node* node, int index) {
        Node* current = node->children[index];
        while (current->children[current->keyCount] != nullptr) {
            current = current->children[current->keyCount];
        }
        return current;
    }

    void borrowFromLeftSibling(Node* node, int index) {
        Node* child = node->children[index];
        Node* leftSibling = node->children[index - 1];

        child->children[child->keyCount + 1] = child->children[child->keyCount];
        for (int i = child->keyCount; i > 0; --i) {
            child->keys[i] = child->keys[i - 1];
            child->info[i] = child->info[i - 1];
            child->children[i] = child->children[i - 1];
        }
        child->children[0] = leftSibling->children[leftSibling->keyCount];
        child->keys[0] = node->keys[index - 1];
        child->info[0] = node->info[index - 1];
        child->keyCount++;

        node->keys[index - 1] = leftSibling->keys[leftSibling->keyCount - 1];
        node->info[index - 1] = leftSibling->info[leftSibling->keyCount - 1];

        leftSibling->keys[leftSibling->keyCount - 1] = "";
        leftSibling->info[leftSibling->keyCount - 1] = nullptr;
        leftSibling->children[leftSibling->keyCount] = nullptr;
        leftSibling->keyCount--;
    }

    void borrowFromRightSibling(Node* node, int index) {
        Node* child = node->children[index];
        Node* rightSibling = node->children[index + 1];

        child->keys[child->keyCount] = node->keys[index];
        child->info[child->keyCount] = node->info[index];
        child->children[child->keyCount + 1] = rightSibling->children[0];
        child->keyCount++;

        node->keys[index] = rightSibling->keys[0];
        node->info[index] = rightSibling->info[0];

        for (int i = 0; i < rightSibling->keyCount - 1; ++i) {
            rightSibling->keys[i] = rightSibling->keys[i + 1];
            rightSibling->info[i] = rightSibling->info[i + 1];
            rightSibling->children[i] = rightSibling->children[i + 1];
        }
        rightSibling->children[rightSibling->keyCount - 1] = rightSibling->children[rightSibling->keyCount];
        rightSibling->keys[rightSibling->keyCount - 1] = "";
        rightSibling->info[rightSibling->keyCount - 1] = nullptr;
        rightSibling->children[rightSibling->keyCount] = nullptr;
        rightSibling->keyCount--;
    }

    void mergeWithSiblings(Node* node, int index) {
        if (index > 0) {
            // Merge with the left sibling
            Node* child = node->children[index];
            Node* leftSibling = node->children[index - 1];

            leftSibling->keys[leftSibling->keyCount] = node->keys[index - 1];
            leftSibling->info[leftSibling->keyCount] = node->info[index - 1];
            leftSibling->children[leftSibling->keyCount + 1] = child->children[0];

            for (int i = 0; i < child->keyCount; ++i) {
                leftSibling->keys[leftSibling->keyCount + i + 1] = child->keys[i];
                leftSibling->info[leftSibling->keyCount + i + 1] = child->info[i];
                leftSibling->children[leftSibling->keyCount + i + 2] = child->children[i + 1];
            }

            leftSibling->keyCount += child->keyCount + 1;

            delete child;
            node->children[index] = nullptr;

            for (int i = index - 1; i < node->keyCount - 1; ++i) {
                node->keys[i] = node->keys[i + 1];
                node->info[i] = node->info[i + 1];
                node->children[i + 1] = node->children[i + 2];
            }
            node->keys[node->keyCount - 1] = "";
            node->info[node->keyCount - 1] = nullptr;
            node->children[node->keyCount] = nullptr;
            node->keyCount--;
        }
        else {
            // Merge with the right sibling
            Node* child = node->children[index];
            Node* rightSibling = node->children[index + 1];

            child->keys[child->keyCount] = node->keys[index];
            child->info[child->keyCount] = node->info[index];
            child->children[child->keyCount + 1] = rightSibling->children[0];

            for (int i = 0; i < rightSibling->keyCount; ++i) {
                child->keys[child->keyCount + i + 1] = rightSibling->keys[i];
                child->info[child->keyCount + i + 1] = rightSibling->info[i];
                child->children[child->keyCount + i + 2] = rightSibling->children[i + 1];
            }

            child->keyCount += rightSibling->keyCount + 1;

            delete rightSibling;
            node->children[index + 1] = nullptr;

            for (int i = index; i < node->keyCount - 1; ++i) {
                node->keys[i] = node->keys[i + 1];
                node->info[i] = node->info[i + 1];
                node->children[i + 1] = node->children[i + 2];
            }
            node->keys[node->keyCount - 1] = "";
            node->info[node->keyCount - 1] = nullptr;
            node->children[node->keyCount] = nullptr;
            node->keyCount--;
        }
    }

    void print(Node* node, std::vector<std::string>& wordList) {
        if (node != nullptr) {
            int i = 0;
            while (i < node->keyCount) {
                print(node->children[i], wordList);
                std::string word = node->keys[i];
                if (std::find(wordList.begin(), wordList.end(), word) == wordList.end()) {
                    wordList.push_back(word);
                    std::cout << "Word: " << word << "; Line Numbers: ";
                    printLineNumbers(node->info[i]);

                }
                i++;
            }
            print(node->children[i], wordList);
        }
    }

    void printLineNumbers(ListNode* node) {
        ListNode* current = node;
        while (current != nullptr) {
            std::cout << current->lineNumber << " ";
            current = current->next;
        }
        std::cout << std::endl;
    }

};