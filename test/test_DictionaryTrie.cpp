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
    ASSERT_EQ(dict.insert("ape", 2), false);
}

// TEST(DictTrieTests, FIND_TEST) {
//     DictionaryTrie dict;

//     dict.insert("apple", 1);

//     ASSERT_EQ(dict.find("apple"), true);
// }