/**
 * TODO: File HEADER
 *
 * Author:
 */

#include <algorithm>
#include <fstream>
#include <iostream>
#include <set>
#include <string>
#include <vector>

#include <gtest/gtest.h>
#include "DictionaryTrie.hpp"
#include "util.hpp"

using namespace std;
using namespace testing;

/* Empty test */
TEST(DictTrieTests, EMPTY_TEST) {
    DictionaryTrie dict;
    ASSERT_EQ(dict.find("abrakadabra"), false);
}

/* TODO */
TEST(DictTrieTests, INSERT_TEST) {
    DictionaryTrie dict;

    ASSERT_EQ(dict.insert("apple", 1), true);
    ASSERT_EQ(dict.insert("ape", 2), true);
    ASSERT_EQ(dict.insert("apa", 2), true);
    ASSERT_EQ(dict.insert("book", 2), true);

    // check if insert() return false if duplication exists
    ASSERT_EQ(dict.insert("apple", 1), false);
    ASSERT_EQ(dict.insert("ape", 2), false);
    ASSERT_EQ(dict.insert("apa", 2), false);
    ASSERT_EQ(dict.insert("book", 2), false);
}

TEST(DictTrieTests, FULL_TEST) {
    DictionaryTrie dict;

    dict.insert("a", 1);
    dict.insert("a a", 1);

    ASSERT_EQ(dict.insert("pain", 1), true);
    ASSERT_EQ(dict.insert("lol", 2), true);
    ASSERT_EQ(dict.insert("pls", 2), true);
    ASSERT_EQ(dict.insert("goal", 2), true);
    ASSERT_EQ(dict.insert("rick", 2), true);
    ASSERT_EQ(dict.insert("richard", 2), true);

    // false test for insert()
    ASSERT_EQ(dict.insert("richard", 2), false);

    ASSERT_EQ(dict.find("pain"), true);
    ASSERT_EQ(dict.find("lol"), true);
    ASSERT_EQ(dict.find("pls"), true);
    ASSERT_EQ(dict.find("goal"), true);
    ASSERT_EQ(dict.find("rick"), true);
    ASSERT_EQ(dict.find("richard"), true);

    // false test for find()
    ASSERT_EQ(dict.find("pa"), false);
}

TEST(DictTrieTests, FIND_TEST) {
    DictionaryTrie dict;

    dict.insert("apple", 1);
    dict.insert("ape", 2);
    dict.insert("apa", 2);
    dict.insert("book", 2);

    ASSERT_EQ(dict.find("apple"), true);
    ASSERT_EQ(dict.find("ape"), true);
    ASSERT_EQ(dict.find("apa"), true);
    ASSERT_EQ(dict.find("book"), true);
    ASSERT_EQ(dict.find("apb"), false);
    ASSERT_EQ(dict.find("app"), false);
}

TEST(DictTrieTests, DFS_PREDICT_COMPLETION_TEST) {
    DictionaryTrie dict;
    dict.insert("apple", 143);
    dict.insert("ap", 143);
    dict.insert("book", 600);
    dict.insert("ape", 144);

    vector<string> pred = dict.predictCompletions("ap", 3);
    string tests[3] = {"ape", "ap", "apple"};
    for (int i = 0; i < pred.size(); i++) {
        cout << pred.at(i) << endl;
        ASSERT_EQ(pred.at(i), tests[i]);
    }

    vector<string> test = dict.predictCompletions("and", 3);
}

TEST(DictTrieTests, END_OF_PREFIX_TEST) {
    DictionaryTrie dict;
    dict.insert("apple", 143);
    dict.insert("ap", 143);
    dict.insert("book", 600);
    dict.insert("ape", 144);

    TrieNode* coNode = dict.endOfPrefixNode("co", 0, dict.getRoot());
    TrieNode* apNode = dict.endOfPrefixNode("ap", 0, dict.getRoot());

    ASSERT_EQ(coNode, nullptr);
    ASSERT_EQ(apNode->getData(), 'p');
}

TEST(DictTrieTests, UNDERSCORE_TEST) {
    DictionaryTrie dict;

    ASSERT_EQ(dict.insert("pale", 1), true);
    ASSERT_EQ(dict.insert("plus", 1), true);
    ASSERT_EQ(dict.insert("pack", 1), true);
    ASSERT_EQ(dict.insert("park", 1), true);
    ASSERT_EQ(dict.insert("pick", 1), true);
    ASSERT_EQ(dict.insert("pork", 1), true);
    ASSERT_EQ(dict.insert("peek", 1), true);
    ASSERT_EQ(dict.insert("jack", 1), true);
    ASSERT_EQ(dict.insert("back", 1), true);

    vector<string> test1 = dict.predictUnderscores("pa_k", 5);
    string ans1[2] = {"pack", "park"};
    for (int i = 0; i < test1.size(); i++) {
        ASSERT_EQ(test1.at(i), ans1[i]);
    }

    vector<string> test2 = dict.predictUnderscores("p__k", 5);
    string ans2[5] = {"pack", "park", "peek", "pick", "pork"};
    for (int i = 0; i < test2.size(); i++) {
        ASSERT_EQ(test2.at(i), ans2[i]);
    }

    ASSERT_EQ(dict.find("pack"), true);

    vector<string> test3 = dict.predictUnderscores("_ack", 5);
    string ans3[3] = {"back", "jack", "pack"};
    for (int i = 0; i < test3.size(); i++) {
        ASSERT_EQ(test3.at(i), ans3[i]);
    }
}