#include <iostream>

#include "../../src/trie.tcc"

using std::cout;

struct NLeaf : Leaf {
  vector<size_t> neighbours;
};


int main(void) {
  Trie<4, NLeaf> trie;

  vector<vector<uint8_t>> words = {
    {0, 0, 0, 0},   // 4
    {0, 1, 0, 0},   // 3
    {0, 0, 1, 0},   // 2
    {0, 0, 0, 1},   // 3
    {0, 1, 0, 1}};  // 3

  size_t line = 0;
  for (vector<uint8_t> word: words) {
    NLeaf* leaf = trie.add(word);
    leaf->neighbours.push_back(line++);
  }

  for (Result<NLeaf> walkResult: trie.walk()) {
    for (Result<NLeaf> hammingResult: trie.hamming(walkResult.path, 1)) {
      if (walkResult.leaf != hammingResult.leaf) {
        walkResult.leaf->neighbours.insert(
          walkResult.leaf->neighbours.end(),
          hammingResult.leaf->neighbours.begin(),
          hammingResult.leaf->neighbours.end());
      }
    }
  }

  for (Result<NLeaf> result: trie.walk()) {
    cout << result.leaf->neighbours[0] << ':';
    for (size_t neighbour: result.leaf->neighbours) {
      cout << ' ' << neighbour;
    }
    cout << '\n';
  }

  return 0;
}
