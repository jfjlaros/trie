#include <catch.hpp>
#include <vector>

#define private public
#include "../src/trie.tcc"
#undef private

using std::vector;

Trie<4, Leaf> _trie;


TEST_CASE("Internal remove function", "[remove]") {
  vector<uint8_t> word = {0x00, 0x01, 0x02};
  _trie.add(word);
  _trie.add(word);

  word = {0x00, 0x01};
  REQUIRE(_remove(_trie._root, word, 0) == false);

  word = {0x00, 0x01, 0x02};
  REQUIRE(_remove(_trie._root, word, 0) == false);
  REQUIRE(_remove(_trie._root, word, 0) == true);
  REQUIRE(_remove(_trie._root, word, 0) == false);
}
