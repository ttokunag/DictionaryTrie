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

    // TrieNode(const char& letter) {
    //     data = letter;
    //     freq = 0;
    //     left = middle = right = nullptr;
    // }

    TrieNode() {
        data = '\0';
        freq = 0;
        left = middle = right = nullptr;
    }

    void setData(const char& d) { data = d; }

    void setFreq(const int& f) { freq = f; }

    char getData() { return data; }

    int getFreq() { return freq; }

    TrieNode* nextNode(const char& d) {
        if (d == data) {
            if (middle == nullptr) {
                middle = new TrieNode();
                middle->setData(d);
            }
            return middle;
        } else if (d < data) {
            if (left == nullptr) {
                left = new TrieNode();
                left->setData(d);
            }
            return left;
        } else {
            if (right == nullptr) {
                right = new TrieNode();
                right->setData(d);
            }
            return right;
        }
    }
};

#endif