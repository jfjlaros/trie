#include <catch.hpp>
#include <vector>

#include "../src/trie.tcc"

using std::vector;


TEST_CASE("Trie", "[find][add]") {
  Trie<4, Leaf> trie;

  SECTION("Find non-existent word") {
    vector<uint8_t> word {0x00, 0x01, 0x02};
    REQUIRE(trie.find(word) == nullptr);
  }

  SECTION("Add word") {
    vector<uint8_t> word {0x00, 0x01, 0x02};
    Leaf* leaf {trie.add(word)};
    REQUIRE(leaf);
    REQUIRE(leaf->count == 1);

    SECTION("Find newly added word") {
      vector<uint8_t> word {0x00, 0x01, 0x02};
      Node<4, Leaf>* node {trie.find(word)};
      REQUIRE(node);
      REQUIRE(node->leaf->count == 1);
    }

    SECTION("Add word again") {
      vector<uint8_t> word {0x00, 0x01, 0x02};
      Leaf* leaf {trie.add(word)};
      REQUIRE(leaf->count == 2);

      SECTION("Find newly added word again") {
        vector<uint8_t> word {0x00, 0x01, 0x02};
        Node<4, Leaf>* node {trie.find(word)};
        REQUIRE(node);
        REQUIRE(node->leaf->count == 2);
      }

      SECTION("Remove word") {
        vector<uint8_t> word {0x00, 0x01, 0x02};
        trie.remove(word);

        Node<4, Leaf>* node {trie.find(word)};
        REQUIRE(node);
        REQUIRE(node->leaf->count == 1);

        SECTION("Remove word again") {
          vector<uint8_t> word {0x00, 0x01, 0x02};
          trie.remove(word);

          Node<4, Leaf>* node {trie.find(word)};
          REQUIRE(node == nullptr);

          SECTION("No prefixes remaining") {
            vector<uint8_t> prefix {0x00, 0x01};

            Node<4, Leaf>* node {trie.find(prefix)};
            REQUIRE(node == nullptr);

            prefix = {0x00};
            node = trie.find(prefix);
            REQUIRE(node == nullptr);
          }

          SECTION("Add word after emptying trie") {
            vector<uint8_t> word {0x00, 0x01, 0x02};
            Leaf* leaf {trie.add(word)};
            REQUIRE(leaf);
            REQUIRE(leaf->count == 1);
          }
        }
      }
    }
  }

  SECTION("Add multiple words") {
    vector<vector<uint8_t>> words {
      {0x00, 0x01, 0x02},
      {0x00, 0x01, 0x03},
      {0x00, 0x02, 0x02},
      {0x00, 0x02, 0x03},
      {0x01, 0x01, 0x02}};

    for (vector<uint8_t> const& word: words) {
      trie.add(word);
      REQUIRE(trie.find(word));
    }

    SECTION("Traverse") {
      size_t i {0};
      for (Result<Leaf> const& result: trie.walk()) {
        REQUIRE(result.leaf->count == 1);
        REQUIRE(result.path == words[i++]);
      }
      REQUIRE(i == words.size());
    }

    SECTION("Hamming") {
      vector<uint8_t> word {0x00, 0x01, 0x03};
      vector<vector<uint8_t>> words {
        {0x00, 0x01, 0x02},
        {0x00, 0x01, 0x03},
        {0x00, 0x02, 0x03}};

      size_t i {0};
      for (Result<Leaf> const& result: trie.hamming(word, 1)) {
        REQUIRE(result.path == words[i++]);
      }
      REQUIRE(i == words.size());
    }

    SECTION("Asymmetric Hamming") {
      vector<uint8_t> word {0x00, 0x01, 0x03};
      vector<vector<uint8_t>> words {
        {0x00, 0x01, 0x03},
        {0x00, 0x02, 0x03}};

      size_t i {0};
      for (Result<Leaf> const& result: trie.asymmetricHamming(word, 1)) {
        REQUIRE(result.path == words[i++]);
      }
      REQUIRE(i == words.size());
    }
  }

  SECTION("Add multiple words of different length") {
    vector<vector<uint8_t>> words {
      {0x00, 0x00, 0x01, 0x02},
      {0x00, 0x00, 0x02},
      {0x00, 0x00, 0x03, 0x02},
      {0x00, 0x01},
      {0x00, 0x01, 0x02},
      {0x00, 0x01, 0x02, 0x03},
      {0x00, 0x02},
      {0x00, 0x02, 0x02},
      {0x00, 0x03}};

    for (vector<uint8_t> const& word: words) {
      trie.add(word);
      REQUIRE(trie.find(word));
    }

    SECTION("Traverse") {
      size_t i {0};
      for (Result<Leaf> const& result: trie.walk()) {
        REQUIRE(result.leaf->count == 1);
        REQUIRE(result.path == words[i++]);
      }
      REQUIRE(i == words.size());
    }

    SECTION("Levenshtein") {
      vector<uint8_t> word {0x00, 0x01, 0x02};
      vector<vector<uint8_t>> words {
        {0x00, 0x02},
        {0x00, 0x00, 0x02},
        {0x00, 0x00, 0x01, 0x02},
        {0x00, 0x01},
        {0x00, 0x01, 0x02},
        {0x00, 0x01, 0x02, 0x03},
        {0x00, 0x01, 0x02},
        {0x00, 0x02, 0x02},
        {0x00, 0x00, 0x01, 0x02}};

      size_t i {0};
      for (Result<Leaf> const& result: trie.levenshtein(word, 1)) {
        REQUIRE(result.path == words[i++]);
      }
      REQUIRE(i == words.size());
    }

    SECTION("Asymmetric Levenshtein") {
      vector<uint8_t> word {0x00, 0x01, 0x02};
      vector<vector<uint8_t>> words {
        {0x00, 0x02},
        {0x00, 0x01},
        {0x00, 0x01, 0x02},
        {0x00, 0x01, 0x02, 0x03},
        {0x00, 0x01, 0x02},
        {0x00, 0x02, 0x02},
        {0x00, 0x00, 0x01, 0x02}};

      size_t i {0};
      for (Result<Leaf> const& result: trie.asymmetricLevenshtein(word, 1)) {
        REQUIRE(result.path == words[i++]);
      }
      REQUIRE(i == words.size());
    }
  }
}
