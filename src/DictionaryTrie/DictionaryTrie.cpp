/**
 * TODO: File Header
 *
 * Author:
 */
#include "DictionaryTrie.hpp"
#include <iostream>

/* TODO */
DictionaryTrie::DictionaryTrie() { root = nullptr; }

/* TODO */
bool DictionaryTrie::insert(string word, unsigned int freq) {
    // an index which points to each character of a given word
    int currIndex = 0;

    if (root == nullptr) {
        // set a root node
        root = new TrieNode(word.at(currIndex++));
        if (word.size() == 1) {
            root->setFreq(freq);
        }
    }

    TrieNode* currNode = root;

    while (currIndex < word.size()) {
        if (currNode == nullptr) {
            currNode = new TrieNode(word.at(currIndex));
        } else {
            currNode = currNode->nextNode(word.at(currIndex));
            std::cout << "printed?" << std::endl;
        }

        ++currIndex;
    }

    // currNode->setFreq(freq);
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