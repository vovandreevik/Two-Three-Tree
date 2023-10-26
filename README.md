# Two-Three Tree

This C++ program implements a Two-Three Tree data structure, a self-balancing search tree, for efficiently storing and retrieving words along with their associated line numbers from a text document. 

The program provides insertion, search, and removal operations.


## Two-Three Tree Overview

A Two-Three Tree is a self-balancing search tree in which each node can have one or two keys and up to three children. 

In this program, the tree is used to efficiently store words from a text document, along with the line numbers where each word appears.


## Input File Format
The input file should contain the text you want to process. The program reads and analyzes this text, extracting words and their associated line numbers.

### Example Input File (test.txt):

```
cherry tree
Cherry;
mango cherry tree,
apple. %^**(@#
free 324235
```
## Examples
Example operations and expected outputs:

- **Building Tree from File**: The program builds a Two-Three Tree from the input file and associates words with their respective line numbers.
  
- **Insertion**: The program inserts the word "banana" with line number 20.
  
- **Searching**: The program searches for the word "banana" and displays whether it was found or not.
  
- **Removal**: The program removes the word "banana."

## Operations
- **Insertion**: Add a word along with its line number to the Two-Three Tree. If the word already exists, it updates the line number list.

- **Searching**: Find a word in the tree and display its line numbers.

- **Removal**: Remove a word from the tree and rebalance the tree if necessary.

- **Print**: Display the contents of the Two-Three Tree, including the stored words and their associated line numbers. This operation is useful for visualizing the tree's structure and the word-line number associations.
