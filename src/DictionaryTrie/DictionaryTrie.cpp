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

        TrieNode* lastNode = placeAllOnMiddleLine(word, letterIndex, node);

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
                    TrieNode* lastNode =
                        placeAllOnMiddleLine(word, letterIndex + 1, node);

                    lastNode->setFreq(freq);

                    return true;
                    // letterIndex++;
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
                        placeAllOnMiddleLine(word, letterIndex + 1, node);

                    lastNode->setFreq(freq);

                    // letterIndex++;
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
    // a vector which contains predictions
    // vector<string> result;

    vector<pair<string, int>> vec;

    TrieNode* lastPrefixNode =
        (prefix.size() != 0) ? endOfPrefixNode(prefix, 0, root) : root;
    // include a given prefix in a prediction if necessary
    if (lastPrefixNode != nullptr && lastPrefixNode->getFreq() > 0) {
        // result.push_back(prefix);
        vec.push_back(pair<string, int>(prefix, lastPrefixNode->getFreq()));
    }

    TrieNode* rootForSearch =
        (prefix.size() != 0) ? lastPrefixNode->middle : root;

    dfsForPredictCompletion(rootForSearch, prefix, &vec, 0, numCompletions);

    vector<string> result;
    for (pair<string, int> p : vec) {
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

void DictionaryTrie::dfsForPredictCompletion(TrieNode* root, string prefix,
                                             vector<string>* result) {
    if (root == nullptr) {
        return;
    }

    char letter = root->getData();
    if (root->getFreq() > 0) {
        result->push_back(prefix + letter);
    }

    dfsForPredictCompletion(root->left, prefix, result);
    dfsForPredictCompletion(root->middle, prefix + letter, result);
    dfsForPredictCompletion(root->right, prefix, result);
}

void DictionaryTrie::dfsForPredictCompletion(TrieNode* root, string prefix,
                                             vector<pair<string, int>>* vec,
                                             int minFreq, int numCompletions) {
    if (root == nullptr) {
        return;
    }

    char letter = root->getData();
    int freq = root->getFreq();
    if (freq > minFreq) {
        int size = vec->size();

        if (size == 0) {
            vec->push_back(pair<string, int>(prefix + letter, freq));
            minFreq = freq;
        } else {
            // remove the least frequent word if a vector is full
            if (size == numCompletions) {
                vec->pop_back();
                // update the least frequency
                minFreq = vec->end()->second;
            }

            bool inserted = false;
            for (int i = 0; i < size; i++) {
                // insert a pair into an appropriate position
                if (freq >= vec->at(i).second) {
                    vec->insert(vec->begin() + i,
                                pair<string, int>(prefix + letter, freq));
                    inserted = true;
                    break;
                }
            }

            // when a given prefix is least frequent
            if (!inserted) {
                vec->push_back(pair<string, int>(prefix + letter, freq));
            }

            // update the least frequency if a current word is it
            minFreq = (minFreq > freq) ? minFreq : freq;
        }
    }

    dfsForPredictCompletion(root->left, prefix, vec, minFreq, numCompletions);
    dfsForPredictCompletion(root->middle, prefix + letter, vec, minFreq,
                            numCompletions);
    dfsForPredictCompletion(root->right, prefix, vec, minFreq, numCompletions);
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

TrieNode* DictionaryTrie::placeAllOnMiddleLine(string str, int index,
                                               TrieNode* node) {
    for (int i = index; i < str.size(); i++) {
        // keeps going down a middle node
        node->middle = new TrieNode(str.at(i));
        node = node->middle;
    }

    return node;
}

void DictionaryTrie::swap(pair<string, int>* arr, int i, int j) {
    pair<string, int> temp(arr->first, arr->second);
    arr[i] = arr[j];
    arr[j] = temp;
}