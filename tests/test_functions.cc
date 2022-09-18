#include <catch.hpp>
#include <vector>

#include "../src/trie.tcc"

using std::vector;


TEST_CASE("Internal remove function", "[remove]") {
  Trie<4, Leaf> trie;

  vector<uint8_t> word = {0x00, 0x01, 0x02};
  trie.add(word);
  trie.add(word);

  word = {0x00, 0x01};
  REQUIRE(remove_(trie.root_, word, 0) == false);

  word = {0x00, 0x01, 0x02};
  REQUIRE(remove_(trie.root_, word, 0) == false);
  REQUIRE(remove_(trie.root_, word, 0) == true);
  REQUIRE(remove_(trie.root_, word, 0) == false);
}
