/**
 * TODO: File Header
 *
 * Author:
 */
#include "DictionaryTrie.hpp"
#include <iostream>

/* TODO */
DictionaryTrie::DictionaryTrie() { root = nullptr; }

void DictionaryTrie::printFirstWord() {
    TrieNode* node = root;

    while (node != nullptr) {
        std::cout << node->getData() << std::endl;
    }
}

/* TODO */
bool DictionaryTrie::insert(string word, unsigned int freq) {
    // an index pointing a letter of a given word;
    int letterIndex = 0;
    // a node we are currently at
    TrieNode* node = root;

    // when a root is empty
    if (root == nullptr) {
        // initializes a root node
        root = new TrieNode(word.at(letterIndex++));
        node = root;

        for (int i = letterIndex; i < word.size(); i++) {
            // keeps going down a middle node
            node->middle = new TrieNode(word.at(i));
            node = node->middle;
        }

        // set a frequency of a word in a destination node
        node->setFreq(freq);
        return true;
    }

    while (letterIndex < word.size()) {
        char letter = word.at(letterIndex);
        // when a current node and a current letter match
        if (letter == node->getData()) {
            node = node->middle;
            letterIndex++;
        } else {
            // when a letter is smaller than a current node letter
            if (letter < node->getData()) {
                if (node->left == nullptr) {
                    node->left = new TrieNode(letter);
                    letterIndex++;
                }
                node = node->left;
            }
            // when a letter is larger than a current node letter
            else {
                if (node->right == nullptr) {
                    node->right = new TrieNode(letter);
                    letterIndex++;
                }
                node = node->right;
            }
        }
    }

    // determine if a given word is already inserted by inspecting if a
    // destination node is already set a frequency
    if (node->getFreq() > 0) {
        return false;
    }

    // set a frequency of a word
    node->setFreq(freq);

    return true;
}

/* TODO */
bool DictionaryTrie::find(string word) const {
    // a node we are currently at
    TrieNode* node = root;
    // an index which points to a letter of a given word
    int letterIndex = 0;

    while (node != nullptr && letterIndex < word.size()) {
        // a current letter of a given word
        char letter = word.at(letterIndex);
        // a letter contained by a current node
        char nodeLetter = node->getData();

        if (letter == nodeLetter) {
            letterIndex++;
            // detects a substring of an inserted word which is not inserted
            // ex) avoid returning true for "app" after inserting only "apple"
            if (letterIndex == word.size() && node->getFreq() == 0) {
                return false;
            }
            node = node->middle;
        }
        // when a letter is smaller than a node letter
        else if (letter < nodeLetter) {
            node = node->left;
        }
        // when a letter is larger than a node letter
        else {
            node = node->right;
        }
    }

    return letterIndex == word.size();
}

/* TODO */
vector<string> DictionaryTrie::predictCompletions(string prefix,
                                                  unsigned int numCompletions) {
    return {};
}

/* TODO */
std::vector<string> DictionaryTrie::predictUnderscores(
    string pattern, unsigned int numCompletions) {
    return {};
}

/* TODO */
DictionaryTrie::~DictionaryTrie() {}