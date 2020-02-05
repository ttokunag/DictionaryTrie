/**
 * This file implements all methods related to a dictionary trie data structure,
 * which allows us to use autocompletion. TrieNode class, a very significant
 * component of this data structure is defined in TrieNode.hpp. You may refer to
 * it for some operations related to TrieNode.
 *
 * Author: Tomoya Tokunaga
 */
#include "DictionaryTrie.hpp"
#include <iostream>

/*
 *  A constructor of DictionaryTrie. Initializes a root.
 */
DictionaryTrie::DictionaryTrie() { root = nullptr; }

/*
 * Inserts a new word to a DictionaryTrie. A node which corresponds with a last
 * letter of a given word will be set frequency
 *
 * @param string: a word to be inserted to a DictionaryTrie
 * @param int: frequency of a given word
 */
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
        char nodeLetter = node->getData();

        // when a current node and a current letter match
        if (letter == nodeLetter) {
            // when reached the destination
            if (letterIndex == word.size() - 1) {
                // when a given word is already in a trie
                if (node->getFreq() > 0) {
                    return false;
                }
                // otherwise mark a node by setting a node's frequency
                node->setFreq(freq);
                return true;
            }

            // when a current node has no middle child but letters equal
            if (node->middle == nullptr) {
                // add new Trie nodes
                letterIndex = addNewNodes(node, 1, word, freq, letterIndex);
                return true;
            }

            node = node->middle;
            letterIndex++;
        }
        // when a current letter is smaller than a node letter
        else if (letter < nodeLetter) {
            if (node->left == nullptr) {
                // add new Trie nodes
                letterIndex = addNewNodes(node, 0, word, freq, letterIndex);
                return true;
            }
            node = node->left;
        }
        // when a current letter is larger than a node letter
        else {
            // when a right node hasn't been set yet
            if (node->right == nullptr) {
                // add new Trie nodes
                letterIndex = addNewNodes(node, 2, word, freq, letterIndex);
                return true;
            }
            node = node->right;
        }
    }
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
    if (lastPrefixNode == nullptr) {
        return {};
    }
    TrieNode* dfsRoot = (prefix.size() != 0) ? lastPrefixNode->middle : root;
    // DFS to find most frequent words
    completionHelper(dfsRoot, prefix, &wordFreqPairs, numCompletions);

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
                                      int numCompletions) {
    // base case
    if (root == nullptr) {
        return;
    }

    char letter = root->getData();
    int freq = root->getFreq();

    // nodes whose frequency is higher than a minimum frequency should be added
    // to a result vector
    if (freq > 0) {
        // a current size of the result vector
        int size = result->size();
        string currStr = prefix + letter;

        // a node should be added when a vector is empty
        if (size == 0) {
            result->push_back(pair<string, int>(currStr, freq));
        }
        // whenever a vecter is nonempty
        else {
            // remove the least frequent word if a vector is full
            if (size == numCompletions) {
                pair<string, int> last = result->at(numCompletions - 1);

                // when a current prefix is more or equal to frequent
                if (freq > last.second ||
                    (freq == last.second && currStr < last.first)) {
                    result->pop_back();
                }
            }

            // if a vector still has capacity, then add
            if (result->size() < numCompletions) {
                // isnert a current prefix to a result vector
                insertInCorrectPlace(result, currStr, freq);
            }
        }
    }

    // recursive phase: left -> middle -> right
    completionHelper(root->left, prefix, result, numCompletions);
    completionHelper(root->middle, prefix + letter, result, numCompletions);
    completionHelper(root->right, prefix, result, numCompletions);
}

/* TODO */
std::vector<string> DictionaryTrie::predictUnderscores(
    string pattern, unsigned int numCompletions) {
    // a vector holding up to numCompletions of predictions with frequency
    vector<pair<string, int>> predictions;

    // find and add valid predictions to a vector
    underscoreRec(root, "", &predictions, pattern, numCompletions);

    // extract a string part from a resultant prediction vector
    vector<string> result;
    for (pair<string, int> p : predictions) {
        result.push_back(p.first);
    }

    return result;
}

void DictionaryTrie::underscoreRec(TrieNode* node, string predict,
                                   vector<pair<string, int>>* vec,
                                   string prefix, int maxSize) {
    // base case:
    // 1. when stepping into an empty node
    // 2. when a prefix is empty
    if (node == nullptr || prefix.size() == 0) {
        return;
    }

    char headChar = prefix.at(0);
    char nodeChar = node->getData();

    // when a letter being looked for is an underscore
    if (headChar == '_') {
        // cut off the head character ex) "CS_100" -> "S_100"
        string nextPrefix = prefix.substr(1, prefix.size());
        // add a current node character to a prediction
        string nextPredict = predict + nodeChar;

        // exhaustively search all valid nodes with dfs
        underscoreRec(node->left, predict, vec, prefix, maxSize);
        underscoreRec(node->right, predict, vec, prefix, maxSize);

        // when we're looking at a last letter of a given prefix
        if (prefix.size() == 1 && node->getFreq() > 0) {
            // add a prediction to a vector
            addPredict(vec, nextPredict, node->getFreq(), maxSize);
        }

        underscoreRec(node->middle, nextPredict, vec, nextPrefix, maxSize);

    }
    // when a letter being looked for is a specific letter
    else {
        // find a node with a headChar
        node = findNode(node, headChar);

        // if such a node doesn't exist, finish this function call
        if (node == nullptr) {
            return;
        }

        string nextPrefix = prefix.substr(1, prefix.size());
        string nextPredict = predict + headChar;

        // when we're looking at a last letter of a given prefix
        if (prefix.size() == 1 && node->getFreq() > 0) {
            // add a prediction to a vector
            addPredict(vec, nextPredict, node->getFreq(), maxSize);
        }

        underscoreRec(node->middle, nextPredict, vec, nextPrefix, maxSize);
    }
}

void DictionaryTrie::addPredict(vector<pair<string, int>>* vec, string predict,
                                int freq, int maxSize) {
    // when a vector is empty --> safe to add a new prediction
    if (vec->size() == 0) {
        vec->push_back(pair<string, int>(predict, freq));
        return;
    }

    pair<string, int> last = vec->at(vec->size() - 1);

    if (vec->size() == maxSize) {
        // when a given prediction's frequency is smaller than a least frequent
        // prediction in a vector --> the prediction is not qualified
        if (freq < last.second) {
            return;
        }
        // when two predictions have equal frequency
        else if (freq == last.second) {
            // when a given prediction alphabetically comes earlier
            if (predict < last.first) {
                vec->pop_back();
            } else {
                return;
            }
        }
        // when a given predict has higher frequency than a current last element
        else {
            vec->pop_back();
        }
    }

    int vecSize = vec->size();
    for (int i = 0; i < vecSize; i++) {
        pair<string, int> curr = vec->at(i);
        if (freq > curr.second) {
            vec->insert(vec->begin() + i, pair<string, int>(predict, freq));
            break;
        } else if (freq == curr.second && predict < curr.first) {
            vec->insert(vec->begin() + i, pair<string, int>(predict, freq));
            break;
        }

        if (i == vecSize - 1) {
            vec->push_back(pair<string, int>(predict, freq));
        }
    }
}

TrieNode* DictionaryTrie::findNode(TrieNode* node, char c) {
    // base case:
    // 1. when a null pointer is passed --> return nullptr
    // 2. when a node holds a character we're looking for
    if (node == nullptr) {
        return nullptr;
    }

    char nodeChar = node->getData();

    if (c == nodeChar) {
        return node;
    } else if (c < nodeChar) {
        return findNode(node->left, c);
    } else {
        return findNode(node->right, c);
    }
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

TrieNode* DictionaryTrie::getRoot() { return root; }

void DictionaryTrie::insertInCorrectPlace(vector<pair<string, int>>* pairs,
                                          string word, int freq) {
    int vecSize = pairs->size();
    // insert a new word to a result vector
    for (int i = 0; i < vecSize; i++) {
        // a current pair we need to compare
        pair<string, int> curr = pairs->at(i);
        // insert a pair into an appropriate position
        if (freq > curr.second) {
            pairs->insert(pairs->begin() + i, pair<string, int>(word, freq));
            break;
        }
        // when there exists a word with the same frequency
        else if (freq == curr.second) {
            // a word alphabetially earlier should come first
            if (word < curr.first) {
                pairs->insert(pairs->begin() + i,
                              pair<string, int>(word, freq));
                break;
            }
        }
        // when a current word is least frequent in a vector
        if (i == vecSize - 1) {
            pairs->push_back(pair<string, int>(word, freq));
        }
    }
}

int DictionaryTrie::addNewNodes(TrieNode* node, int flag, string word, int freq,
                                int letterIndex) {
    switch (flag) {
        case 0: {
            node->left = new TrieNode(word.at(letterIndex));
            node = node->left;

            // a last node for a last letter of a given word
            TrieNode* lastNode = createMiddleLine(word, letterIndex + 1, node);

            lastNode->setFreq(freq);
            return letterIndex;
        }

        case 1: {
            char newLetter = word.at(++letterIndex);
            node->middle = new TrieNode(newLetter);
            node = node->middle;

            // a last node for a last letter of a given word
            TrieNode* lastNode = createMiddleLine(word, letterIndex + 1, node);

            lastNode->setFreq(freq);
            return letterIndex;
        }

        case 2: {
            node->right = new TrieNode(word.at(letterIndex));
            node = node->right;

            TrieNode* lastNode = createMiddleLine(word, letterIndex + 1, node);

            lastNode->setFreq(freq);
            return letterIndex;
        }

        default:
            return 0;
    }
}