#pragma once

#include <vector>

#include "node.tcc"

using std::make_unique;
using std::vector;

/*! Result.
 *
 * \tparam T Leaf type.
 */
template <class T>
struct Result {
  vector<uint8_t> path;
  T* leaf;
};


/* Add a word to a (sub)trie.
 *
 * \tparam alphabetSize Size of the alphabet.
 * \tparam T Leaf type.
 *
 * \param node Root.
 * \param word Word.
 *
 * \return Leaf.
 */
template <uint8_t alphabetSize, class T>
T* add_(Node<alphabetSize, T>* const node, vector<uint8_t> const& word) {
  auto node_ {node};
  for (auto const& letter: word) {
    if (not node_->child[letter]) {
      node_->child[letter] = make_unique<Node<alphabetSize, T>>();
    }
    node_ = node_->child[letter].get();
  }
  if (not node_->leaf) {
    node_->leaf = make_unique<T>();
  }
  node_->leaf->count++;

  return node_->leaf.get();
}

/* Remove a word from a (sub)trie.
 *
 * \tparam alphabetSize Size of the alphabet.
 * \tparam T Leaf type.
 *
 * \param node Root.
 * \param word Word.
 * \param position Position in `word`.
 *
 * \return `true` if the last occurrence of `word` is removed.
 */
template <uint8_t alphabetSize, class T>
bool remove_(
    Node<alphabetSize, T>* const node, vector<uint8_t> const& word,
    size_t const position) {
  if (position == word.size()) {
    if (node->leaf) {
      node->leaf->count--;
      if (not node->leaf->count) {
        node->leaf.reset();
        return true;
      }
    }
    return false;
  }

  if (not node->child[word[position]]) {
    return false;
  }

  auto result {remove_(node->child[word[position]].get(), word, position + 1)};
  if (result) {
    if (node->child[word[position]]->isEmpty()) {
      node->child[word[position]].reset();
    }
  }

  return result;
}

/* Find a word in a (sub)trie.
 *
 * \tparam alphabetSize Size of the alphabet.
 * \tparam T Leaf type.
 *
 * \param node Root.
 * \param word Word.
 *
 * \return `node` if found, `nullptr` otherwise.
 */
template <uint8_t alphabetSize, class T>
Node<alphabetSize, T>* find_(
    Node<alphabetSize, T>* const node, vector<uint8_t> const& word) {
  auto node_ {node};
  for (auto const& letter: word) {
    if (not node_->child[letter]) {
      return nullptr;
    }
    node_ = node_->child[letter].get();
  }
  return node_;
}

/* Traverse a (sub)trie.
 *
 * \tparam alphabetSize Size of the alphabet.
 * \tparam T Leaf type.
 *
 * \param node Root.
 * \param path Path.
 *
 * \return Traversal results.
 */
template <uint8_t alphabetSize, class T>
generator<Result<T>> walk_(
    Node<alphabetSize, T>* const node, vector<uint8_t>& path) {
  if (node->leaf) {
    Result<T> result {path, node->leaf.get()};
    co_yield result;
  }
  for (size_t i {0}; i < alphabetSize; i++) {
    if (node->child[i]) {
      path.push_back(i);
      co_yield walk_(node->child[i].get(), path);
      path.pop_back();
    }
  }
}

/* Find all words within Hamming distance `distance` of `word`.
 *
 * \tparam alphabetSize Size of the alphabet.
 * \tparam T Leaf type.
 * \tparam full Full traversal.
 *
 * \param node Root.
 * \param word Word.
 * \param position Position in `word`.
 * \param distance Maximum distance.
 * \param path Path.
 *
 * \return Traversal results.
 */
template <uint8_t alphabetSize, class T, bool full>
generator<Result<T>> hamming_(
    Node<alphabetSize, T> const* const node,
    vector<uint8_t> const& word, size_t const position, int const distance,
    vector<uint8_t>& path) {
  if (distance >= 0) {
    if (position < word.size()) {
      uint8_t start {0};
      if (not full) {
        start = word[position];
      }
      for (uint8_t i {start}; i < alphabetSize; i++) {
        if (node->child[i]) {
          path.push_back(i);
          co_yield hamming_<alphabetSize, T, full>(
            node->child[i].get(), word, position + 1,
            distance - (i != word[position]), path);
          path.pop_back();
        }
      }
    }
    else {
      Result<T> result {path, node->leaf.get()};
      co_yield result;
    }
  }
}

/* Find all words within Levenshtein distance `distance` of `word`.
 *
 * \tparam alphabetSize Size of the alphabet.
 * \tparam T Leaf type.
 * \tparam full Full traversal.
 *
 * \param node Root.
 * \param word Word.
 * \param position Position in `word`.
 * \param distance Maximum distance.
 * \param path Path.
 *
 * \return Traversal results.
 */
template <uint8_t alphabetSize, class T, bool full=true>
generator<Result<T>> levenshtein_(
    Node<alphabetSize, T> const* const node, vector<uint8_t> const& word,
    size_t const position, int const distance, vector<uint8_t>& path) {
  if (distance >= 0) {
    // Deletion.
    co_yield levenshtein_<alphabetSize, T, full>(
      node, word, position + 1, distance - 1, path);

    uint8_t start {0};
    if (not full and position < word.size()) {
      start = word[position];
    }
    for (uint8_t i {start}; i < alphabetSize; i++) {
      if (node->child[i]) {
        path.push_back(i);

        // Substitution.
        if (position < word.size()) {
          co_yield levenshtein_<alphabetSize, T, full>(
            node->child[i].get(), word, position + 1,
            distance - (i != word[position]), path);
        }

        // Insertion.
        co_yield levenshtein_<alphabetSize, T, full>(
          node->child[i].get(), word, position, distance - 1, path);

        path.pop_back();
      }
    }
    
    if (position >= word.size() and node->leaf) {
      Result<T> result {path, node->leaf.get()};
      co_yield result;
    }
  }
}
