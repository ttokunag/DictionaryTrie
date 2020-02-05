/**
 * TODO: File Header
 *
 * Author:
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
    // TODO: add private members and helper methods here
    TrieNode* root;

    void deleteAll(TrieNode* node);

    TrieNode* createMiddleLine(string str, int index, TrieNode* node);

    void completionHelper(TrieNode* root, string prefix,
                          vector<pair<string, int>>* vec, int numCompletions);

    void insertInCorrectPlace(vector<pair<string, int>>* pairs, string word,
                              int freq);

    int addNewNodes(TrieNode* node, int flag, string word, int freq,
                    int letterIndex);

    void underscoreRec(TrieNode* node, string predict,
                       vector<pair<string, int>>* vec, string prefix,
                       int maxSize);

    TrieNode* findNode(TrieNode* node, char c);

    void addPredict(vector<pair<string, int>>* vec, string predict, int freq,
                    int maxSize);

  public:
    /* TODO: add function header */
    DictionaryTrie();

    /* TODO: add function header */
    bool insert(string word, unsigned int freq);

    /* TODO: add function header */
    bool find(string word) const;

    /* TODO: add function header */
    vector<string> predictCompletions(string prefix,
                                      unsigned int numCompletions);

    /* TODO: add function header */
    vector<string> predictUnderscores(string pattern,
                                      unsigned int numCompletions);

    /* TODO: add function header */
    ~DictionaryTrie();

    TrieNode* getRoot();

    TrieNode* endOfPrefixNode(string prefix, int index, TrieNode* node);
};

#endif  // DICTIONARY_TRIE_HPP
