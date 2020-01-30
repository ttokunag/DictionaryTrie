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
    // an index pointing a letter of a given word;
    int letterIndex = 0;
    // a node we are currently at
    TrieNode* node = root;

    // when a root is empty
    if (root == nullptr) {
        // initializes a root node
        root = new TrieNode(word.at(letterIndex++));
        node = root;

        TrieNode* lastNode = createMiddleLine(word, letterIndex, node);

        // set a frequency of a word in a destination node
        lastNode->setFreq(freq);
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
                // when a left node hasn't been set yet
                if (node->left == nullptr) {
                    node->left = new TrieNode(letter);
                    node = node->left;

                    // a last node for a last letter of a given word
                    TrieNode* lastNode =
                        createMiddleLine(word, letterIndex + 1, node);

                    lastNode->setFreq(freq);

                    return true;
                }
                node = node->left;
            }
            // when a letter is larger than a current node letter
            else {
                // when a right node hasn't been set yet
                if (node->right == nullptr) {
                    node->right = new TrieNode(letter);
                    node = node->right;

                    TrieNode* lastNode =
                        createMiddleLine(word, letterIndex + 1, node);

                    lastNode->setFreq(freq);

                    return true;
                }
                node = node->right;
            }
        }
    }
    return false;
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
    // a vector which contains a pair of word and frequency of words starting
    // with a given prefix
    vector<pair<string, int>> wordFreqPairs;
    // find a node which will be a root node for searching
    // if the prefix is empty, it is a root node
    TrieNode* lastPrefixNode =
        (prefix.size() != 0) ? endOfPrefixNode(prefix, 0, root) : root;

    // include a given prefix in a prediction if it's counted
    if (lastPrefixNode != nullptr && lastPrefixNode->getFreq() > 0) {
        wordFreqPairs.push_back(
            pair<string, int>(prefix, lastPrefixNode->getFreq()));
    }

    // a node which is used as a root for searching
    TrieNode* dfsRoot = (prefix.size() != 0) ? lastPrefixNode->middle : root;
    // DFS to find most frequent words
    completionHelper(dfsRoot, prefix, &wordFreqPairs, 0, numCompletions);

    // a vector which contains words most frequently used
    vector<string> result;

    for (pair<string, int> p : wordFreqPairs) {
        result.push_back(p.first);
    }

    return result;
}

TrieNode* DictionaryTrie::endOfPrefixNode(string prefix, int index,
                                          TrieNode* node) {
    // base case
    if (node == nullptr) {
        return node;
    }

    char currChar = prefix.at(index);
    char nodeChar = node->getData();

    // when a current letter equals a node letter
    if (currChar == nodeChar) {
        // when reached the destination node
        if (index == prefix.size() - 1) {
            return node;
        } else {
            return endOfPrefixNode(prefix, index + 1, node->middle);
        }
    }
    // when a current letter is smaller than a node letter
    else if (currChar < nodeChar) {
        return endOfPrefixNode(prefix, index, node->left);
    }
    // when a current letter is larger than a node letter
    else {
        return endOfPrefixNode(prefix, index, node->right);
    }
}

void DictionaryTrie::completionHelper(TrieNode* root, string prefix,
                                      vector<pair<string, int>>* result,
                                      int minFreq, int numCompletions) {
    // base case
    if (root == nullptr) {
        return;
    }

    char letter = root->getData();
    int freq = root->getFreq();

    // nodes whose frequency is higher than a minimum frequency should be added
    // to a result vector
    if (freq > minFreq) {
        // a current size of the result vector
        int size = result->size();

        // a node should be added when a vector is empty
        if (size == 0) {
            result->push_back(pair<string, int>(prefix + letter, freq));
        } else {
            // remove the least frequent word if a vector is full
            if (size == numCompletions) {
                // a last word is always least frequent in a vector
                result->pop_back();
                // update the least frequency
                minFreq = result->end()->second;
            }

            for (int i = 0; i < size; i++) {
                // insert a pair into an appropriate position
                if (freq >= result->at(i).second) {
                    result->insert(result->begin() + i,
                                   pair<string, int>(prefix + letter, freq));
                    break;
                }
                // when a current word is least frequent in a vector
                else if (i == size - 1) {
                    result->push_back(pair<string, int>(prefix + letter, freq));
                }
            }

            // update the least frequency if a current word is it
            minFreq = (minFreq > freq) ? minFreq : freq;
        }
    }

    // recursive phase: left -> middle -> right
    completionHelper(root->left, prefix, result, minFreq, numCompletions);
    completionHelper(root->middle, prefix + letter, result, minFreq,
                     numCompletions);
    completionHelper(root->right, prefix, result, minFreq, numCompletions);
}
/* TODO */
std::vector<string> DictionaryTrie::predictUnderscores(
    string pattern, unsigned int numCompletions) {
    return {};
}

/* TODO */
DictionaryTrie::~DictionaryTrie() {
    deleteAll(root);
    delete root;
}

void DictionaryTrie::deleteAll(TrieNode* node) {
    // base case when a node is nothing
    if (node == nullptr) {
        return;
    }

    // recursively call itself and free memories allocated for a left,
    // middle, and right child node
    deleteAll(node->left);
    deleteAll(node->middle);
    deleteAll(node->right);

    delete node->left;
    delete node->middle;
    delete node->right;
}

TrieNode* DictionaryTrie::createMiddleLine(string str, int index,
                                           TrieNode* node) {
    for (int i = index; i < str.size(); i++) {
        // keeps going down a middle node
        node->middle = new TrieNode(str.at(i));
        node = node->middle;
    }

    return node;
}