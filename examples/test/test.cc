#include <ctime>
#include <fstream>
#include <ios>
#include <iostream>
#include <vector>

#include "../../src/trie.tcc"
#include "../lib/ngs/ngs.h"

using std::cout;
using std::cerr;
using std::ios;

const char file[] = "../../data/10M.bin";

class NLeaf : public Leaf<size_t> {
  public:
    vector<size_t> lines;

    void add(size_t& l) { lines.push_back(l); }
};


void visit(vector<uint8_t>& word, NLeaf& leaf, vector<size_t>& neighbours) {
  neighbours.insert(neighbours.end(), leaf.lines.begin(), leaf.lines.end());
}


int main(void) {
  Trie<4, NLeaf> trie;

  ifstream handle(file, ios::in | ios::binary);
  cerr << time(NULL) << " Reading data.\n";
  size_t line = 0;
  while (!handle.eof()) {
    vector<uint8_t> word = readWord(handle, 24);
    trie.add(word, line);
    line++;
    handle.peek();
  }
  handle.seekg(0);
  cerr << time(NULL) << " Calculating neighbours.\n";
  line = 0;
  while (!handle.eof()) {
    vector<uint8_t> word = readWord(handle, 24);
    vector<size_t> neighbours = trie.hamming(word, 1, visit);
    cout << line;
    for (size_t neighbour: neighbours) {
      cout << ' ' << neighbour;
    }
    cout << '\n';
    line++;
    handle.peek();
  }
  handle.close();

  cerr << time(NULL) << " Done.\n";

  return 0;
}
