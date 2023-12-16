# Indexing
**Objective:** work with binary search trees

Write a C++ program that reads words from a text file and store them in lowercase with their file location in a binary search tree. For each word, store the line numbers and number of occurrences within each line. A word my exist in multiple lines and multiple times per line. Remove the possessive ('s) from all words. If a word begins or ends in a punctuation (period, comma, question mark, exclamation point, brackets, etc.), remove it before storing it in the index tree.

Your program should ignore the words stored in "ignore.txt". Some of these words include:

`The, a, that, at, an, on, in, over, under, for, of, from, to, into, he, she, they, them, him, his, her, hers, their, ...`

Each node in the tree should include a word and a list of locations (line number, number of occurrences).

Write a program that shows a menu of options to do the following:

1. Display index in order (see sample output)
2. Search for a word (ask the user for a word and display the node information where the word was found)
3. Most common word (return the first most common word and how many times it occurred)
4. Quit

Submit the two files
- `indexing.cc`: your main program
- `index-tree.h`: the tree class and all the functions related to the tree
