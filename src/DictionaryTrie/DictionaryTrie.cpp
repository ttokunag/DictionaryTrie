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

/*
 * Finds if a given word is in a DictionaryTrie. If it exists in a trie, this
 * returns true. Otherwise return false
 *
 * @param string: a word which this function looks for
 */
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

/*
 * Returns a vector of up to numCompletions of words which contain a given
 * prefix.
 *
 * @param string: a prefix which this function uses to find predictions
 * @param int: a max number of predictions
 */
vector<string> DictionaryTrie::predictCompletions(string prefix,
                                                  unsigned int numCompletions) {
    // a vector which contains a pair of word and frequency of words starting
    // with a given prefix
    vector<pair<string, int>> predictions;
    // find a node which will be a root node for searching
    // if the prefix is empty, it is a root node
    // TrieNode* lastPrefixNode =
    //     (prefix.size() != 0) ? endOfPrefixNode(prefix, 0, root) : root;

    TrieNode* lastPrefixNode = endOfPrefixNode(prefix, 0, root);

    // a node which is used as a root for searching
    if (lastPrefixNode == nullptr) {
        return {};
    }

    // include a given prefix in a prediction if it's counted
    if (lastPrefixNode->getFreq() > 0) {
        predictions.push_back(
            pair<string, int>(prefix, lastPrefixNode->getFreq()));
    }

    TrieNode* dfsRoot = (prefix.size() != 0) ? lastPrefixNode->middle : root;
    // DFS to find most frequent words
    completionHelper(dfsRoot, prefix, &predictions, numCompletions);

    // a vector which contains words most frequently used
    vector<string> result;

    for (pair<string, int> p : predictions) {
        result.push_back(p.first);
    }

    return result;
}

/*
 * Returns a reference of a node whose data matches a last letter of a give
 * prefix
 *
 * @param string: a word whose last letter will be contained in a return node
 * @param int: an index of a current letter in a given prefix
 * @param TrieNode*: a reference of a current node
 */
TrieNode* DictionaryTrie::endOfPrefixNode(string prefix, int index,
                                          TrieNode* node) {
    // base case
    if (node == nullptr || prefix.size() == 0) {
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

/*
 * A helper function for predictCompletions(). This function traverses a
 * DictionaryTrie and add a word to a prediction vector once this finds a valid
 * one.
 *
 * @param TrieNode*: a reference of a current node
 * @param string: a partial prediction
 *  (which will be added more letters as it goes down a trie)
 * @param vector<pair<string, int>>*: a reference of a vector of pairs
 * @param int: a maximum size a result vector can be
 */
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
                pair<string, int> last = result->at(size - 1);

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

/*
 * Returns a vector of up to numCompletions of words which follows a given
 * pattern (underscored position will be filled with all possible letters).
 *
 * @param string: a pattern this function uses for prediction
 * @param int: a max size of a result vector can be
 */
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

/*
 * A helper function for predictUnderscores(). This calls itself recursively
 * until finding all potential predictions can be derived from a given pattern.
 *
 * @param TrieNode*: a reference of a current node
 * @param string: a partial prediction
 *  (which will be added more letters as it goes down a trie)
 * @param vector<pair<string, int>>*: a reference of a vector of pairs
 * @param string: a pattern this function uses for prediction
 * @param int: a maximum size a result vector can be
 */
void DictionaryTrie::underscoreRec(TrieNode* node, string predict,
                                   vector<pair<string, int>>* vec,
                                   string pattern, int maxSize) {
    // base case:
    // 1. when stepping into an empty node
    // 2. when a pattern is empty
    if (node == nullptr || pattern.size() == 0) {
        return;
    }

    char headChar = pattern.at(0);
    char nodeChar = node->getData();

    // when a letter being looked for is an underscore
    if (headChar == '_') {
        // cut off the head character ex) "CS_100" -> "S_100"
        string nextPattern = pattern.substr(1, pattern.size());
        // add a current node character to a prediction
        string nextPredict = predict + nodeChar;

        // exhaustively search all valid nodes with dfs
        underscoreRec(node->left, predict, vec, pattern, maxSize);
        underscoreRec(node->right, predict, vec, pattern, maxSize);

        // when we're looking at a last letter of a given pattern
        if (pattern.size() == 1 && node->getFreq() > 0) {
            // add a prediction to a vector
            addPredict(vec, nextPredict, node->getFreq(), maxSize);
        }

        underscoreRec(node->middle, nextPredict, vec, nextPattern, maxSize);

    }
    // when a letter being looked for is a specific letter
    else {
        // find a node with a headChar
        node = findNode(node, headChar);

        // if such a node doesn't exist, finish this function call
        if (node == nullptr) {
            return;
        }

        string nextPattern = pattern.substr(1, pattern.size());
        string nextPredict = predict + headChar;

        // when we're looking at a last letter of a given pattern
        if (pattern.size() == 1 && node->getFreq() > 0) {
            // add a prediction to a vector
            addPredict(vec, nextPredict, node->getFreq(), maxSize);
        }

        underscoreRec(node->middle, nextPredict, vec, nextPattern, maxSize);
    }
}

/*
 * Inserts a given string into an appropriate position of a given vector.
 *
 * @param vector<pair<string, int>>*: a reference of a vector of pairs
 * @param string: a prediction to be inserted
 * @param int: frequency of a given prediction
 * @param int: a maximum size a result vector can be
 */
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

/*
 * Finds a node which contains a given character
 *
 * @param TrieNode*: a reference of a current node
 * @param char: a character we're looking for
 */
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

/*
 * Destructor of a DictionaryTrie
 */
DictionaryTrie::~DictionaryTrie() {
    deleteAll(root);
    delete root;
}

/*
 * Free all memory which is related a given node
 *
 * @param TrieNode*: a root node of a DictionaryTrie we want to deconstruct
 */
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

/*
 * A helper function which repeatedly add new nodes which contains each
 * character of a given word to a middle of a given node
 *
 * @param string: a word we want to add
 * @param int: an index of a current letter of a given word
 * @param TrieNode*: a reference of a current node
 */
TrieNode* DictionaryTrie::createMiddleLine(string str, int index,
                                           TrieNode* node) {
    for (int i = index; i < str.size(); i++) {
        // keeps going down a middle node
        node->middle = new TrieNode(str.at(i));
        node = node->middle;
    }

    return node;
}

/*
 * Returns a root of this DictionaryTrie
 */
TrieNode* DictionaryTrie::getRoot() { return root; }

/*
 * A helper function which add a new string integer pair into a given vector. An
 * insertion place of a new pair is determined by the alphabetical order of a
 * given string and its frequency
 *
 * @param vector<pair<string, int>>*: a vector of pairs of string and int
 * @param string: a word for a new pair
 * @param int: frequency of a given word
 */
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

/*
 * Adds a new node to a given node
 *
 * @param TrieNode*: a reference of a current node
 * @param int: flag to decide which child a new node added to (left,right,
 * middle)
 * @param string: a word to be added
 * @param int: frequency of a given word
 * @param int: an index of a current letter of a given word
 */
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