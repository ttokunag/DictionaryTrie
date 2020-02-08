/**
 * This is a header file for DictionaryTrie.cpp
 *
 * Author: Tomoya Tokunaga
 */
#ifndef DICTIONARY_TRIE_HPP
#define DICTIONARY_TRIE_HPP

#include <string>
#include <utility>
#include <vector>
#include "TrieNode.hpp"

using namespace std;

/**
 * The class for a dictionary ADT, implemented as either
 * a mulit-way trie or a ternary search tree.
 */
class DictionaryTrie {
  private:
    // a reference of a root node of a DictionaryTrie
    TrieNode* root;

    /*
     * Free all memory which is related a given node
     *
     * @param TrieNode*: a root node of a DictionaryTrie we want to deconstruct
     */
    void deleteAll(TrieNode* node);

    /*
     * A helper function which repeatedly add new nodes which contains each
     * character of a given word to a middle of a given node
     *
     * @param string: a word we want to add
     * @param int: an index of a current letter of a given word
     * @param TrieNode*: a reference of a current node
     */
    TrieNode* createMiddleLine(string str, int index, TrieNode* node);

    /*
     * A helper function for predictCompletions(). This function traverses a
     * DictionaryTrie and add a word to a prediction vector once this finds a
     * valid one.
     *
     * @param TrieNode*: a reference of a current node
     * @param string: a partial prediction
     *  (which will be added more letters as it goes down a trie)
     * @param vector<pair<string, int>>*: a reference of a vector of pairs
     * @param int: a maximum size a result vector can be
     */
    void completionHelper(TrieNode* root, string prefix,
                          vector<pair<string, int>>* vec, int numCompletions);

    /*
     * A helper function which add a new string integer pair into a given
     * vector. An insertion place of a new pair is determined by the
     * alphabetical order of a given string and its frequency
     *
     * @param vector<pair<string, int>>*: a vector of pairs of string and int
     * @param string: a word for a new pair
     * @param int: frequency of a given word
     */
    void insertInCorrectPlace(vector<pair<string, int>>* pairs, string word,
                              int freq);

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
    int addNewNodes(TrieNode* node, int flag, string word, int freq,
                    int letterIndex);

    /*
     * A helper function for predictUnderscores(). This calls itself recursively
     * until finding all potential predictions can be derived from a given
     * pattern.
     *
     * @param TrieNode*: a reference of a current node
     * @param string: a partial prediction
     *  (which will be added more letters as it goes down a trie)
     * @param vector<pair<string, int>>*: a reference of a vector of pairs
     * @param string: a pattern this function uses for prediction
     * @param int: a maximum size a result vector can be
     */
    void underscoreRec(TrieNode* node, string predict,
                       vector<pair<string, int>>* vec, string prefix,
                       int maxSize);

    /*
     * Finds a node which contains a given character
     *
     * @param TrieNode*: a reference of a current node
     * @param char: a character we're looking for
     */
    TrieNode* findNode(TrieNode* node, char c);

    /*
     * Inserts a given string into an appropriate position of a given vector.
     *
     * @param vector<pair<string, int>>*: a reference of a vector of pairs
     * @param string: a prediction to be inserted
     * @param int: frequency of a given prediction
     * @param int: a maximum size a result vector can be
     */
    void addPredict(vector<pair<string, int>>* vec, string predict, int freq,
                    int maxSize);

  public:
    /*
     *  A constructor of DictionaryTrie. Initializes a root.
     */
    DictionaryTrie();

    /*
     * Inserts a new word to a DictionaryTrie. A node which corresponds with a
     * last letter of a given word will be set frequency
     *
     * @param string: a word to be inserted to a DictionaryTrie
     * @param int: frequency of a given word
     */
    bool insert(string word, unsigned int freq);

    /*
     * Finds if a given word is in a DictionaryTrie. If it exists in a trie,
     * this returns true. Otherwise return false
     *
     * @param string: a word which this function looks for
     */
    bool find(string word) const;

    /*
     * Returns a vector of up to numCompletions of words which contain a given
     * prefix.
     *
     * @param string: a prefix which this function uses to find predictions
     * @param int: a max number of predictions
     */
    vector<string> predictCompletions(string prefix,
                                      unsigned int numCompletions);

    /*
     * Returns a vector of up to numCompletions of words which follows a given
     * pattern (underscored position will be filled with all possible letters).
     *
     * @param string: a pattern this function uses for prediction
     * @param int: a max size of a result vector can be
     */
    vector<string> predictUnderscores(string pattern,
                                      unsigned int numCompletions);

    /*
     * Destructor of a DictionaryTrie
     */
    ~DictionaryTrie();

    /*
     * Returns a root of this DictionaryTrie
     */
    TrieNode* getRoot();

    /*
     * Returns a reference of a node whose data matches a last letter of a give
     * prefix
     *
     * @param string: a word whose last letter will be contained in a return
     * node
     * @param int: an index of a current letter in a given prefix
     * @param TrieNode*: a reference of a current node
     */
    TrieNode* endOfPrefixNode(string prefix, int index, TrieNode* node);
};

#endif  // DICTIONARY_TRIE_HPP
