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
        root = new TrieNode();
        root->setData(word.at(letterIndex++));
        node = root;

        for (int i = letterIndex; i < word.size(); i++) {
            node->middle = new TrieNode();
            node->middle->setData(word.at(i));
            node = node->middle;
        }

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
            if (letter < node->getData()) {  // a letter is smaller
                if (node->left == nullptr) {
                    node->left = new TrieNode();
                    node->left->setData(letter);
                    letterIndex++;
                }
                node = node->left;
            } else {  // a letter is larger
                if (node->right == nullptr) {
                    node->right = new TrieNode();
                    node->right->setData(letter);
                    letterIndex++;
                }
                node = node->right;
            }
        }
    }

    if (node->getFreq() > 0) {
        return false;
    }

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
            if (letterIndex == word.size() && node->getFreq() == 0) {
                return false;
            }
            node = node->middle;
        } else if (letter < nodeLetter) {
            node = node->left;
        } else {
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