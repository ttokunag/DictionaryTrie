#ifndef TRIENODE_HPP
#define TRIENODE_HPP

class TrieNode {
  private:
    char data;

  public:
    TrieNode* left;
    TrieNode* middle;
    TrieNode* right;

    TrieNode(const char& d) { data = d; }

    void setData(const char& d) { data = d; }

    char getData() { return data; }
};

#endif