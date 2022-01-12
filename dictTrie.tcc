#ifndef DICT_TRIE_TCC_
#define DICT_TRIE_TCC_

#include <vector>

#include "node.tcc"

using std::vector;

/*!
 * Trie.
 */
template <uint8_t alphabetSize>
class Trie {
  public:
    Trie(void);
    ~Trie(void);
    void add(vector<uint8_t>&);
    Node<alphabetSize>* find(vector<uint8_t>&);
    void remove(vector<uint8_t>&);
    void iterate(void);

  private:
    Node<alphabetSize>* _root = NULL;

    void _delete(Node<alphabetSize>*);
    bool _remove(Node<alphabetSize>*, vector<uint8_t>&, size_t);
    void _iterate(Node<alphabetSize>*, vector<uint8_t>&);
};


template <uint8_t alphabetSize>
void Trie<alphabetSize>::_delete(Node<alphabetSize>* node) {
  for (Node<alphabetSize>* child: node->child) {
    if (child) {
      _delete(child);
    }
  }
  delete node;
}

template <uint8_t alphabetSize>
bool Trie<alphabetSize>::_remove(
    Node<alphabetSize>* node, vector<uint8_t>& word, size_t start) {
  if (start == word.size()) {
    if (node->size) {
      node->size--;
      return true;
    }
    return false;
  }

  if (!node->child[word[start]]) {
    return false;
  }

  bool result = _remove(node->child[word[start]], word, start + 1);
  if (result) {
    if (node->child[word[start]]->isEmpty()) {
      delete node->child[word[start]];
      node->child[word[start]] = NULL;
    }
  }

  return result;
}

#include <iostream>
inline void print(vector<uint8_t>& word) {
  for (uint8_t letter: word) {
    std::cout << (int)letter << ' ';
  }
  std::cout << '\n';
}

template <uint8_t alphabetSize>
void Trie<alphabetSize>::_iterate(
    Node<alphabetSize>* node, vector<uint8_t>& word) {
  for (size_t i = 0; i < node->size; i++) {
    print(word);
  }
  for (size_t i = 0; i < alphabetSize; i++) {
    if (node->child[i]) {
      word.push_back(i);
      _iterate(node->child[i], word);
      word.pop_back();
    }
  }
}


/*!
 * Constructor.
 */
template <uint8_t alphabetSize>
Trie<alphabetSize>::Trie(void) {
  _root = new Node<alphabetSize>;
}

/*!
 * Destructor.
 */
template <uint8_t alphabetSize>
Trie<alphabetSize>::~Trie(void) {
  _delete(_root);
}

/*!
 * Add a word.
 *
 * \param word Word.
 */
template <uint8_t alphabetSize>
void Trie<alphabetSize>::add(vector<uint8_t>& word) {
  Node<alphabetSize>* p = _root;
  for (uint8_t letter: word) {
    if (!p->child[letter]) {
      p->child[letter] = new Node<alphabetSize>;
    }
    p = p->child[letter];
  }
  p->size++;
}

/*!
 * Find a word.
 *
 * \param word Word.
 *
 * \return End of `word` node when found, `NULL` otherwise.
 */
template <uint8_t alphabetSize>
Node<alphabetSize>* Trie<alphabetSize>::find(vector<uint8_t>& word) {
  Node<alphabetSize>* p = _root;
  for (uint8_t letter: word) {
    if (!p->child[letter]) {
      return NULL;
    }
    p = p->child[letter];
  }
  return p;
}

/*!
 * Remove a word.
 *
 * \param word Word.
 *
 * \return `true` if the last occurrence of `word` is removed.
 */
template <uint8_t alphabetSize>
void Trie<alphabetSize>::remove(vector<uint8_t>& word) {
  _remove(_root, word, 0);
}

template <uint8_t alphabetSize>
void Trie<alphabetSize>::iterate(void) {
  vector<uint8_t> word;
  _iterate(_root, word);
}

#endif
