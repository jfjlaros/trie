#pragma once

#include "../lib/CPP20Coroutines/include/generator.hpp"

#include "trieFunctions.tcc"

/*! Trie.
 *
 * \tparam alphabetSize Size of the alphabet.
 * \tparam T Leaf type.
 */
template <uint8_t alphabetSize, class T>
class Trie {
public:
  /*! Add a word.
   *
   * \param word Word.
   *
   * \return Leaf.
   */
  T* add(vector<uint8_t> const&) const;

  /*! Remove a word.
   *
   * \param word Word.
   */
  void remove(vector<uint8_t> const&) const;

  /*! Find a word.
   *
   * \param word Word.
   *
   * \return `node` if found, `nullptr` otherwise.
   */
  Node<alphabetSize, T>* find(vector<uint8_t> const&) const;

  /*! Traverse.
   *
   * \return Traversal results.
   */
  generator<Result<T>> walk() const;

  /*! Hamming.
   *
   * \param word Word.
   * \param distance Maximum distance.
   *
   * \return Traversal results.
   */
  generator<Result<T>> hamming(vector<uint8_t> const&, int const) const;

  /*! Asymmetric Hamming.
   *
   * \param word Word.
   * \param distance Maximum distance.
   *
   * \return Traversal results.
   */
  generator<Result<T>> asymmetricHamming(
    vector<uint8_t> const&, int const) const;

  /*! Levenshtein.
   *
   * \param word Word.
   * \param distance Maximum distance.
   *
   * \return Traversal results.
   */
  generator<Result<T>> levenshtein(vector<uint8_t> const&, int const) const;

  /*! Asymmetric Levenshtein.
   *
   * \param word Word.
   * \param distance Maximum distance.
   *
   * \return Traversal results.
   */
  generator<Result<T>> asymmetricLevenshtein(
    vector<uint8_t> const&, int const) const;

private:
  unique_ptr<Node<alphabetSize, T>> root_ {make_unique<Node<alphabetSize, T>>()};
};


template <uint8_t alphabetSize, class T>
T* Trie<alphabetSize, T>::add(vector<uint8_t> const& word) const {
  return add_(root_.get(), word);
}

template <uint8_t alphabetSize, class T>
void Trie<alphabetSize, T>::remove(vector<uint8_t> const& word) const {
  remove_(root_.get(), word, 0);
}

template <uint8_t alphabetSize, class T>
Node<alphabetSize, T>* Trie<alphabetSize, T>::find(
    vector<uint8_t> const& word) const {
  return find_(root_.get(), word);
}

template <uint8_t alphabetSize, class T>
generator<Result<T>> Trie<alphabetSize, T>::walk() const {
  vector<uint8_t> path;
  co_yield walk_(root_.get(), path);
}

template <uint8_t alphabetSize, class T>
generator<Result<T>> Trie<alphabetSize, T>::hamming(
    vector<uint8_t> const& word, int const distance) const {
  vector<uint8_t> path;
  co_yield hamming_<alphabetSize, T, true>(root_.get(), word, 0, distance, path);
}

template <uint8_t alphabetSize, class T>
generator<Result<T>> Trie<alphabetSize, T>::asymmetricHamming(
    vector<uint8_t> const& word, int const distance) const {
  vector<uint8_t> path;
  co_yield hamming_<alphabetSize, T, false>(root_.get(), word, 0, distance, path);
}

template <uint8_t alphabetSize, class T>
generator<Result<T>> Trie<alphabetSize, T>::levenshtein(
    vector<uint8_t> const& word, int const distance) const {
  vector<uint8_t> path;
  co_yield levenshtein_<alphabetSize, T, true>(root_.get(), word, 0, distance, path);
}

template <uint8_t alphabetSize, class T>
generator<Result<T>> Trie<alphabetSize, T>::asymmetricLevenshtein(
    vector<uint8_t> const& word, int const distance) const {
  vector<uint8_t> path;
  co_yield levenshtein_<alphabetSize, T, false>(root_.get(), word, 0, distance, path);
}
