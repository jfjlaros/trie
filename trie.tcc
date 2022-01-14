#ifndef TRIE_TCC_
#define TRIE_TCC_

#include <vector>
#include <map>

#include "node.tcc"

using std::vector;
using std::map;

/*!
 * Trie.
 */
template <uint8_t alphabetSize, class T>
class Trie {
  public:
    Trie(void);
    ~Trie(void);
    template <class U>
      void add(vector<uint8_t>&, U);
    Node<alphabetSize, T>* find(vector<uint8_t>&);
    void remove(vector<uint8_t>&);
    vector<vector<uint8_t>> iterate(void);
    map<size_t, size_t> count(void);

  private:
    Node<alphabetSize, T>* _root = NULL;

    void _delete(Node<alphabetSize, T>*);
    bool _remove(Node<alphabetSize, T>*, vector<uint8_t>&, size_t);
    void _iterate(
      Node<alphabetSize, T>*, vector<uint8_t>&, vector<vector<uint8_t>>&);
    void _count(Node<alphabetSize, T>*, map<size_t, size_t>&);
};


template <uint8_t alphabetSize, class T>
void Trie<alphabetSize, T>::_delete(Node<alphabetSize, T>* node) {
  for (Node<alphabetSize, T>* child: node->child) {
    if (child) {
      _delete(child);
    }
  }
  if (node->leaf) {
    delete node->leaf;
  }
  delete node;
}

template <uint8_t alphabetSize, class T>
bool Trie<alphabetSize, T>::_remove(
    Node<alphabetSize, T>* node, vector<uint8_t>& word, size_t start) {
  if (start == word.size()) {
    if (node->leaf) {
      node->leaf->pop();
      if (!node->leaf->count) {
        delete node->leaf;
        node->leaf = NULL;
      }
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

template <uint8_t alphabetSize, class T>
void Trie<alphabetSize, T>::_iterate(
    Node<alphabetSize, T>* node, vector<uint8_t>& word,
    vector<vector<uint8_t>>& result) {
  if (node->leaf) {
    for (size_t i = 0; i < node->leaf->count; i++) {
      result.push_back(word);
    }
  }
  for (size_t i = 0; i < alphabetSize; i++) {
    if (node->child[i]) {
      word.push_back(i);
      _iterate(node->child[i], word, result);
      word.pop_back();
    }
  }
}

template <uint8_t alphabetSize, class T>
void Trie<alphabetSize, T>::_count(
    Node<alphabetSize, T>* node, map<size_t, size_t>& counts) {
  if (node->leaf) {
    counts[node->leaf->count]++;
  }
  for (size_t i = 0; i < alphabetSize; i++) {
    if (node->child[i]) {
      _count(node->child[i], counts);
    }
  }
}


/*!
 * Constructor.
 */
template <uint8_t alphabetSize, class T>
Trie<alphabetSize, T>::Trie(void) {
  _root = new Node<alphabetSize, T>;
}

/*!
 * Destructor.
 */
template <uint8_t alphabetSize, class T>
Trie<alphabetSize, T>::~Trie(void) {
  _delete(_root);
}

/*!
 * Add a word.
 *
 * \param word Word.
 * \param data Data.
 */
template <uint8_t alphabetSize, class T>
template <class U>
void Trie<alphabetSize, T>::add(vector<uint8_t>& word, U data) {
  Node<alphabetSize, T>* p = _root;
  for (uint8_t letter: word) {
    if (!p->child[letter]) {
      p->child[letter] = new Node<alphabetSize, T>;
    }
    p = p->child[letter];
  }
  if (!p->leaf) {
    p->leaf = new T;
  }
  p->leaf->push(data);
}

/*!
 * Find a word.
 *
 * \param word Word.
 *
 * \return End of `word` node when found, `NULL` otherwise.
 */
template <uint8_t alphabetSize, class T>
Node<alphabetSize, T>* Trie<alphabetSize, T>::find(vector<uint8_t>& word) {
  Node<alphabetSize, T>* p = _root;
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
template <uint8_t alphabetSize, class T>
void Trie<alphabetSize, T>::remove(vector<uint8_t>& word) {
  _remove(_root, word, 0);
}

template <uint8_t alphabetSize, class T>
vector<vector<uint8_t>> Trie<alphabetSize, T>::iterate(void) {
  vector<uint8_t> word;
  vector<vector<uint8_t>> result;
  _iterate(_root, word, result);
  return result;
}

/*!
 * Count statistics.
 *
 * \return Count statistics.
 */
template <uint8_t alphabetSize, class T>
map<size_t, size_t> Trie<alphabetSize, T>::count(void) {
  map<size_t, size_t> counts;
  _count(_root, counts);
  return counts;
}

#endif
