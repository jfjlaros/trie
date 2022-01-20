#pragma once

#include "../lib/CPP20Coroutines/include/generator.hpp"

#include "trieFunctions.tcc"

/*!
 * Trie.
 */
template <uint8_t alphabetSize, class T>
class Trie {
  public:
    Trie(void);
    ~Trie(void);
    T* add(vector<uint8_t>&);
    void remove(vector<uint8_t>&);
    Node<alphabetSize, T>* find(vector<uint8_t>&);
    generator<Result<T>> walk(void);
    generator<Result<T>> hamming(vector<uint8_t>&, int);

  private:
    Node<alphabetSize, T>* _root = NULL;
};


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
 *
 * \return Leaf.
 */
template <uint8_t alphabetSize, class T>
T* Trie<alphabetSize, T>::add(vector<uint8_t>& word) {
  return _add(_root, word);
}

/*!
 * Remove a word.
 *
 * \param word Word.
 */
template <uint8_t alphabetSize, class T>
void Trie<alphabetSize, T>::remove(vector<uint8_t>& word) {
  _remove(_root, word, 0);
}

/*!
 * Find a word.
 *
 * \param word Word.
 *
 * \return `node` if found, `NULL` otherwise.
 */
template <uint8_t alphabetSize, class T>
Node<alphabetSize, T>* Trie<alphabetSize, T>::find(vector<uint8_t>& word) {
  return _find(_root, word);
}

/*!
 * Traverse.
 *
 * \return Traversal results.
 */
template <uint8_t alphabetSize, class T>
generator<Result<T>> Trie<alphabetSize, T>::walk(void) {
  vector<uint8_t> path;
  co_yield _walk(_root, path);
}

/*!
 * Hamming.
 *
 * \param word Word.
 * \param distance Maximum distance.
 *
 * \return Traversal results.
 */
template <uint8_t alphabetSize, class T>
generator<Result<T>> Trie<alphabetSize, T>::hamming(
    vector<uint8_t>& word, int distance) {
  vector<uint8_t> path;
  co_yield _hamming(_root, word, 0, distance, path);
}
