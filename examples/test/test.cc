#include <iostream>

#include "../../src/trie.tcc"

using std::cout;

struct NLeaf : Leaf {
  vector<size_t> lines;
  vector<NLeaf*> neighbours;
  size_t cluster = 0;
};


void assignCluster(NLeaf* leaf, size_t cluster) {
  leaf->cluster = cluster;
  for (NLeaf* neighbour: leaf->neighbours) {
    if (!neighbour->cluster) {
      assignCluster(neighbour, cluster);
    }
  }
}


int main(void) {
  Trie<4, NLeaf> trie;

  vector<vector<uint8_t>> words = {
    {0, 0, 0, 0},  // 0
    {0, 0, 0, 0},  // 1
    {0, 1, 0, 0},  // 2
    {0, 0, 1, 0},  // 3
    {0, 0, 0, 1},  // 4
    {0, 1, 0, 1},  // 5
    {3, 3, 3, 3},  // 6
    {3, 3, 2, 3},  // 7
    {3, 2, 3, 3}}; // 8

  size_t line = 0;
  for (vector<uint8_t> word: words) {
    NLeaf* leaf = trie.add(word);
    leaf->lines.push_back(line++);
  }

  for (Result<NLeaf> walkResult: trie.walk()) {
    for (Result<NLeaf> hammingResult: trie.hamming(walkResult.path, 1)) {
      if (walkResult.leaf != hammingResult.leaf) {
        walkResult.leaf->neighbours.push_back(hammingResult.leaf);
        hammingResult.leaf->neighbours.push_back(walkResult.leaf);
      }
    }
  }

  size_t cluster = 1;
  for (Result<NLeaf> result: trie.walk()) {
    if (!result.leaf->cluster) {
      assignCluster(result.leaf, cluster++);
    }
  }

  for (Result<NLeaf> result: trie.walk()) {
    for (size_t line: result.leaf->lines) {
      cout << line << ' ' << result.leaf->cluster << '\n';
    }
  }

  return 0;
}
