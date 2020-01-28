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

    // iterate until reaching the last letter of a given word
    while (letterIndex < word.size()) {
        char letter = word.at(letterIndex);

        if (root == nullptr) {  // when a root is empty
            root = new TrieNode(letter);
            node = root;
        }

        else {
            // the next node to go based on a current letter
            TrieNode* next = node->nextNode(letter);
            // set a current node to the next node
            node = next;
        }

        letterIndex++;
    }

    if (node->getFreq() > 0) {
        return false;
    }

    node->setFreq(freq);
    return true;
}

/* TODO */
bool DictionaryTrie::find(string word) const { return false; }

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