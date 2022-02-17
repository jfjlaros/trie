#include <catch.hpp>
#include <vector>

#define private public
#include "../src/trie.tcc"
#undef private

using std::vector;


TEST_CASE("Internal remove function", "[remove]") {
  Trie<4, Leaf> trie;

  vector<uint8_t> word = {0x00, 0x01, 0x02};
  trie.add(word);
  trie.add(word);

  word = {0x00, 0x01};
  REQUIRE(_remove(trie._root, word, 0) == false);

  word = {0x00, 0x01, 0x02};
  REQUIRE(_remove(trie._root, word, 0) == false);
  REQUIRE(_remove(trie._root, word, 0) == true);
  REQUIRE(_remove(trie._root, word, 0) == false);
}
