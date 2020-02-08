/**
 * This is a header file for TrieNode class
 *
 * Author: Tomoya Tokunaga
 */

#ifndef TRIENODE_HPP
#define TRIENODE_HPP

#include <string>

class TrieNode {
  private:
    char data;
    int freq;

  public:
    TrieNode* left;
    TrieNode* middle;
    TrieNode* right;

    /*
     * a constructor of TrieNode class
     */
    TrieNode(const char& letter) {
        data = letter;
        freq = 0;
        left = middle = right = nullptr;
    }

    // set a data to a given character
    void setData(const char& d) { data = d; }

    // set a freq to a given frequency
    void setFreq(const int& f) { freq = f; }

    // get a data set to this node
    char getData() { return data; }

    // get frequency set to this node
    int getFreq() { return freq; }
};

#endif