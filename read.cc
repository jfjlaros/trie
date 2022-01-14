#include <fstream>
#include <ios>
#include <iostream>
#include <vector>

#include "trie.tcc"

using std::cout;
using std::ifstream;
using std::ios;
using std::pair;
using std::vector;

const char file[] = "../data/10M.bin";


vector<uint8_t> readWord(ifstream& handle, size_t size) {
  uint8_t buffer[size];
  handle.read((char*)buffer, size);
  return vector<uint8_t>(buffer, buffer + size);
}

void printWord(vector<uint8_t>& word) {
  for (uint8_t letter: word) {
    cout << (int)letter << ' ';
  }
  cout << '\n';
}

void printCounts(map<size_t, size_t>& counts) {
  size_t total = 0;
  size_t unique = 0;
  for (pair<size_t, size_t> count: counts) {
    cout << count.first << ' ' << count.second << '\n';
    total += count.first * count.second;
    unique += count.second;
  }
  cout << "\nTotal: " << total << "\nUnique: " << unique << '\n';
}


int main(void) {
  Trie<4, Leaf<char>> trie;

  ifstream handle(file, ios::in | ios::binary);
  while (!handle.eof()) {
    vector<uint8_t> word = readWord(handle, 24);
    trie.add(word, '\0');
    handle.peek();
  }
  handle.close();

  map<size_t, size_t> counts = trie.count();
  printCounts(counts);

  return 0;
}
