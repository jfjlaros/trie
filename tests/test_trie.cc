#include <catch.hpp>
#include <vector>

#include "../src/trie.tcc"

using std::vector;

Trie<4, Leaf> trie;


TEST_CASE("Find non-existent word", "[find]") {
  vector<uint8_t> word = {0x00, 0x01, 0x02};
  REQUIRE(trie.find(word) == NULL);
}

TEST_CASE("Add word", "[add]") {
  vector<uint8_t> word = {0x00, 0x01, 0x02};
  Leaf* leaf = trie.add(word);
  REQUIRE(leaf != NULL);
  REQUIRE(leaf->count == 1);
}

TEST_CASE("Find newly added word", "[find]") {
  vector<uint8_t> word = {0x00, 0x01, 0x02};
  Node<4, Leaf>* node = trie.find(word);
  REQUIRE(node != NULL);
  REQUIRE(node->leaf->count == 1);
}

TEST_CASE("Add word again", "[add]") {
  vector<uint8_t> word = {0x00, 0x01, 0x02};
  Leaf* leaf = trie.add(word);
  REQUIRE(leaf->count == 2);
}

TEST_CASE("Find newly added word again", "[find]") {
  vector<uint8_t> word = {0x00, 0x01, 0x02};
  Node<4, Leaf>* node = trie.find(word);
  REQUIRE(node != NULL);
  REQUIRE(node->leaf->count == 2);
}

TEST_CASE("Remove word", "[remove]") {
  vector<uint8_t> word = {0x00, 0x01, 0x02};
  trie.remove(word);

  Node<4, Leaf>* node = trie.find(word);
  REQUIRE(node != NULL);
  REQUIRE(node->leaf->count == 1);
}

TEST_CASE("Remove word again", "[remove]") {
  vector<uint8_t> word = {0x00, 0x01, 0x02};
  trie.remove(word);

  Node<4, Leaf>* node = trie.find(word);
  REQUIRE(node == NULL);
}

TEST_CASE("No prefixes remaining", "[remove]") {
  vector<uint8_t> prefix = {0x00, 0x01};

  Node<4, Leaf>* node = trie.find(prefix);
  REQUIRE(node == NULL);

  prefix = {0x00};
  node = trie.find(prefix);
  REQUIRE(node == NULL);
}

TEST_CASE("Add word after emptying trie", "[add]") {
  vector<uint8_t> word = {0x00, 0x01, 0x02};
  Leaf* leaf = trie.add(word);
  REQUIRE(leaf != NULL);
  REQUIRE(leaf->count == 1);
}

TEST_CASE("Add more words", "[add]") {
  vector<uint8_t> word = {0x00, 0x01, 0x03};
  trie.add(word);
  REQUIRE(trie.find(word) != NULL);

  word = {0x00, 0x02, 0x02};
  trie.add(word);
  REQUIRE(trie.find(word) != NULL);

  word = {0x00, 0x02, 0x03};
  trie.add(word);
  REQUIRE(trie.find(word) != NULL);

  word = {0x01, 0x01, 0x02};
  trie.add(word);
  REQUIRE(trie.find(word) != NULL);
}

TEST_CASE("Traverse", "[walk]") {
  vector<vector<uint8_t>> words = {
    {0x00, 0x01, 0x02},
    {0x00, 0x01, 0x03},
    {0x00, 0x02, 0x02},
    {0x00, 0x02, 0x03},
    {0x01, 0x01, 0x02}};

  size_t i = 0;
  for (Result<Leaf> result: trie.walk()) {
    REQUIRE(result.leaf->count == 1);
    REQUIRE(result.path == words[i++]);
  }
  REQUIRE(i == words.size());
}

TEST_CASE("Hamming", "[walk]") {
  vector<uint8_t> word = {0x00, 0x01, 0x02};
  vector<vector<uint8_t>> words = {
    {0x00, 0x01, 0x02},
    {0x00, 0x01, 0x03},
    {0x00, 0x02, 0x02},
    {0x01, 0x01, 0x02}};

  size_t i = 0;
  for (Result<Leaf> result: trie.hamming(word, 1)) {
    REQUIRE(result.path == words[i++]);
  }
  REQUIRE(i == words.size());
}
