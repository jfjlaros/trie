#include <fstream>
#include <ios>
#include <iostream>
#include <map>
#include <vector>
#include <ctime>

#include "../../src/trie.tcc"
#include "../lib/ngs/ngs.h"

using std::cout;
using std::cerr;
using std::ios;
using std::map;
using std::pair;

const char file[] = "../../data/10M.bin";

class NLeaf : public Leaf<size_t> {
  public:
    vector<size_t> lines;

    void add(size_t& l) { lines.push_back(l); }
};


int main(void) {
  Trie<4, NLeaf> trie;

  ifstream handle(file, ios::in | ios::binary);
  cerr << time(NULL) << " Reading data.\n";
  size_t line = 0;
  while (!handle.eof()) {
    vector<uint8_t> word = readWord(handle, 24);
    trie.add(word, line++);
    handle.peek();
  }

  cerr << time(NULL) << " Calculating neighbours.\n";
  for (pair<vector<uint8_t>, NLeaf> leaf: trie.walk()) {
    for (pair<vector<uint8_t>, NLeaf> neigh: trie.hamming(leaf.first, 1)) {
      cout << neigh.second.lines[0];
      printWord(neigh.first);
    }
    cout << "---\n";
  }

/*
  for (pair<vector<uint8_t>, NLeaf> hit: trie.hamming(words[0], 1)) {
    cout << hit.second.lines[0];
    printWord(hit.first);
  }
  */
/*
  line = 0;
  for (vector<uint8_t> word: words) {
    vector<size_t> neighbours = trie.hamming(word, 1, visit);
    cout << line;
    for (size_t neighbour: neighbours) {
      cout << ' ' << neighbour;
    }
    cout << '\n';
    line++;
  }
  */

  //map<size_t, size_t> counts = trie.traverse(count);
  //printCounts(counts);

  /*
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
  */

  /*
  cerr << time(NULL) << " Counting neighbours.\n";
  map<size_t, size_t> counts = trie.traverse(count);
  printCounts(counts);
  cout << "\nTotal: " << line << '\n';
  */
  cerr << time(NULL) << " Done.\n";

  return 0;
}
