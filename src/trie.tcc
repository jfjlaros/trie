#pragma once

#include "../lib/CPP20Coroutines/include/generator.hpp"

#include "trieFunctions.tcc"

/*!
 * Trie.
 */
template <uint8_t alphabetSize, class T>
class Trie {
  Node<alphabetSize, T>* root_ = nullptr;

public:
  Trie();
  ~Trie();

  T* add(vector<uint8_t> const&) const;
  void remove(vector<uint8_t> const&) const;
  Node<alphabetSize, T>* find(vector<uint8_t> const&) const;
  generator<Result<T>> walk() const;
  generator<Result<T>> hamming(vector<uint8_t> const&, int const) const;
  generator<Result<T>> asymmetricHamming(
    vector<uint8_t> const&, int const) const;
  generator<Result<T>> levenshtein(vector<uint8_t> const&, int const) const;
  generator<Result<T>> asymmetricLevenshtein(
    vector<uint8_t> const&, int const) const;
};


/*!
 * Constructor.
 */
template <uint8_t alphabetSize, class T>
Trie<alphabetSize, T>::Trie() {
  root_ = new Node<alphabetSize, T>;
}

/*!
 * Destructor.
 */
template <uint8_t alphabetSize, class T>
Trie<alphabetSize, T>::~Trie() {
  delete_(root_);
}


/*!
 * Add a word.
 *
 * \param word Word.
 *
 * \return Leaf.
 */
template <uint8_t alphabetSize, class T>
T* Trie<alphabetSize, T>::add(vector<uint8_t> const& word) const {
  return add_(root_, word);
}

/*!
 * Remove a word.
 *
 * \param word Word.
 */
template <uint8_t alphabetSize, class T>
void Trie<alphabetSize, T>::remove(vector<uint8_t> const& word) const {
  remove_(root_, word, 0);
}

/*!
 * Find a word.
 *
 * \param word Word.
 *
 * \return `node` if found, `nullptr` otherwise.
 */
template <uint8_t alphabetSize, class T>
Node<alphabetSize, T>* Trie<alphabetSize, T>::find(
    vector<uint8_t> const& word) const {
  return find_(root_, word);
}

/*!
 * Traverse.
 *
 * \return Traversal results.
 */
template <uint8_t alphabetSize, class T>
generator<Result<T>> Trie<alphabetSize, T>::walk() const {
  vector<uint8_t> path;
  co_yield walk_(root_, path);
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
    vector<uint8_t> const& word, int const distance) const {
  vector<uint8_t> path;
  co_yield hamming_<alphabetSize, T, true>(root_, word, 0, distance, path);
}

/*!
 * Asymmetric Hamming.
 *
 * \param word Word.
 * \param distance Maximum distance.
 *
 * \return Traversal results.
 */
template <uint8_t alphabetSize, class T>
generator<Result<T>> Trie<alphabetSize, T>::asymmetricHamming(
    vector<uint8_t> const& word, int const distance) const {
  vector<uint8_t> path;
  co_yield hamming_<alphabetSize, T, false>(root_, word, 0, distance, path);
}

/*!
 * Levenshtein.
 *
 * \param word Word.
 * \param distance Maximum distance.
 *
 * \return Traversal results.
 */
template <uint8_t alphabetSize, class T>
generator<Result<T>> Trie<alphabetSize, T>::levenshtein(
    vector<uint8_t> const& word, int const distance) const {
  vector<uint8_t> path;
  co_yield levenshtein_<alphabetSize, T, true>(root_, word, 0, distance, path);
}

/*!
 * Asymmetric Levenshtein.
 *
 * \param word Word.
 * \param distance Maximum distance.
 *
 * \return Traversal results.
 */
template <uint8_t alphabetSize, class T>
generator<Result<T>> Trie<alphabetSize, T>::asymmetricLevenshtein(
    vector<uint8_t> const& word, int const distance) const {
  vector<uint8_t> path;
  co_yield levenshtein_<alphabetSize, T, false>(root_, word, 0, distance, path);
}
