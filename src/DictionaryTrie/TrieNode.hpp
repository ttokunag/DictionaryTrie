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

    TrieNode(const char& letter) {
        data = letter;
        freq = NULL;
        left = middle = right = nullptr;
    }

    void setData(const char& d) { data = d; }

    void setFreq(const int& f) { freq = f; }

    char getData() { return data; }

    int getFreq() { return freq; }

    TrieNode* nextNode(const char& d) {
        if (middle == nullptr || d == data) {
            if (middle == nullptr) {
                middle = new TrieNode(d);
            }
            return middle;
        } else if (d < data) {
            if (left == nullptr) {
                left = new TrieNode(d);
            }
            return left;
        } else {
            if (right == nullptr) {
                right = new TrieNode(d);
            }
            return right;
        }
    }
};

#endif