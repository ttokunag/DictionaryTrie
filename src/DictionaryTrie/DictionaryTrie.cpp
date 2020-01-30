/**
 * TODO: File Header
 *
 * Author:
 */
#include "DictionaryTrie.hpp"
#include <iostream>

/* TODO */
DictionaryTrie::DictionaryTrie() {
    root = nullptr;
    trieSize = 0;
    visited = 0;
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

        TrieNode* lastNode = createMiddleLine(word, letterIndex, node);

        // set a frequency of a word in a destination node
        lastNode->setFreq(freq);
        trieSize++;
        return true;
    }

    while (letterIndex < word.size()) {
        char letter = word.at(letterIndex);
        char nodeLetter = node->getData();
        // when a current node and a current letter match
        if (letter == nodeLetter) {
            if (letterIndex == word.size() - 1) {
                // when a given word is already in a trie
                if (node->getFreq() > 0) {
                    return false;
                }
                // otherwise mark a node by setting a node's frequency
                node->setFreq(freq);
                trieSize++;
                return true;
            }

            if (node->middle == nullptr) {
                char newLetter = word.at(++letterIndex);
                node->middle = new TrieNode(newLetter);
                node = node->middle;

                // a last node for a last letter of a given word
                TrieNode* lastNode =
                    createMiddleLine(word, letterIndex + 1, node);

                lastNode->setFreq(freq);

                trieSize++;
                return true;
            }

            node = node->middle;
            letterIndex++;
        } else {
            // when a letter is smaller than a current node letter
            if (letter < nodeLetter) {
                // when a left node hasn't been set yet
                if (node->left == nullptr) {
                    node->left = new TrieNode(letter);
                    node = node->left;

                    // a last node for a last letter of a given word
                    TrieNode* lastNode =
                        createMiddleLine(word, letterIndex + 1, node);

                    lastNode->setFreq(freq);

                    trieSize++;
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

                    trieSize++;
                    return true;
                }
                node = node->right;
            }
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
    // if (freq > minFreq) {
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
                // // a last word is always least frequent in a vector
                // result->pop_back();
                // // update the least frequency
                // minFreq = result->end()->second;

                // a word at the end of a vector is least frequent
                // pair<string, int> last = *(result->end());
                pair<string, int> last = result->at(numCompletions - 1);

                // when a current prefix is more frequent
                if (freq > last.second) {
                    result->pop_back();
                }
                // when a current prefix has the same frequency
                else if (freq == last.second) {
                    // when a currStr comes earlier
                    if (currStr < last.first) {
                        result->pop_back();
                    }
                }
            }

            // if a vector still has capacity, then add
            if (result->size() < numCompletions) {
                int vecSize = result->size();
                // insert a new word to a result vector
                for (int i = 0; i < vecSize; i++) {
                    // a current pair we need to compare
                    pair<string, int> curr = result->at(i);
                    // insert a pair into an appropriate position
                    if (freq > curr.second) {
                        result->insert(result->begin() + i,
                                       pair<string, int>(currStr, freq));
                        break;
                    }
                    // when there exists a word with the same frequency
                    else if (freq == curr.second) {
                        // a word which is alphabetially earlier should come
                        // first
                        if (currStr < curr.first) {
                            result->insert(result->begin() + i,
                                           pair<string, int>(currStr, freq));
                            break;
                        }
                    }
                    // when a current word is least frequent in a vector
                    // if (i == size - 1) {
                    if (i == vecSize - 1) {
                        result->push_back(pair<string, int>(currStr, freq));
                    }
                }

                // update the least frequency if a current word is it
                minFreq = (minFreq > freq) ? minFreq : freq;
            }
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

TrieNode* DictionaryTrie::getRoot() { return root; }