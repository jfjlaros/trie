#pragma once

#include <vector>

#include "node.tcc"

using std::vector;

/*!
 * Result.
 */
template <class T>
struct Result {
  vector<uint8_t> path;
  T* leaf;
};


/*
 * Delete a (sub)trie.
 *
 * \param node Root.
 */
template <uint8_t alphabetSize, class T>
void _delete(Node<alphabetSize, T>* node) {
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

/*
 * Add a word to a (sub)trie.
 *
 * \param node Root.
 * \param word Word.
 *
 * \return Leaf.
 */
template <uint8_t alphabetSize, class T>
T* _add(Node<alphabetSize, T>* node, vector<uint8_t>& word) {
  for (uint8_t letter: word) {
    if (!node->child[letter]) {
      node->child[letter] = new Node<alphabetSize, T>;
    }
    node = node->child[letter];
  }
  if (!node->leaf) {
    node->leaf = new T;
  }
  node->leaf->count++;

  return node->leaf;
}

/*
 * Remove a word from a (sub)trie.
 *
 * \param node Root.
 * \param word Word.
 * \param position Position in `word`.
 *
 * \return `true` if the last occurrence of `word` is removed.
 */
template <uint8_t alphabetSize, class T>
bool _remove(
    Node<alphabetSize, T>* node, vector<uint8_t>& word, size_t position) {
  if (position == word.size()) {
    if (node->leaf) {
      node->leaf->count--;
      if (!node->leaf->count) {
        delete node->leaf;
        node->leaf = NULL;
        return true;
      }
    }
    return false;
  }

  if (!node->child[word[position]]) {
    return false;
  }

  bool result = _remove(node->child[word[position]], word, position + 1);
  if (result) {
    if (node->child[word[position]]->isEmpty()) {
      delete node->child[word[position]];
      node->child[word[position]] = NULL;
    }
  }

  return result;
}

/*
 * Find a word in a (sub)trie.
 *
 * \param node Root.
 * \param word Word.
 *
 * \return `node` if found, `NULL` otherwise.
 */
template <uint8_t alphabetSize, class T>
Node<alphabetSize, T>* _find(
    Node<alphabetSize, T>* node, vector<uint8_t>& word) {
  for (uint8_t letter: word) {
    if (!node->child[letter]) {
      return NULL;
    }
    node = node->child[letter];
  }
  return node;
}

/*
 * Traverse a (sub)trie.
 *
 * \param node Root.
 * \param path Path.
 *
 * \return Traversal results.
 */
template <uint8_t alphabetSize, class T>
generator<Result<T>> _walk(
    Node<alphabetSize, T>* node, vector<uint8_t>& path) {
  if (node->leaf) {
    Result<T> result = {path, node->leaf};
    co_yield result;
  }
  for (size_t i = 0; i < alphabetSize; i++) {
    if (node->child[i]) {
      path.push_back(i);
      co_yield _walk(node->child[i], path);
      path.pop_back();
    }
  }
}

/*
 * Find all words within Hamming distance `distance` of `word`.
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
generator<Result<T>> _hamming(
    Node<alphabetSize, T>* node, vector<uint8_t>& word, size_t position,
    int distance, vector<uint8_t>& path) {
  if (distance >= 0) {
    if (position < word.size()) {
      uint8_t start = 0;
      if (!full) {
        start = word[position];
      }
      for (uint8_t i = start; i < alphabetSize; i++) {
        if (node->child[i]) {
          path.push_back(i);
          co_yield _hamming<alphabetSize, T, full>(
            node->child[i], word, position + 1,
            distance - (uint8_t)(i != word[position]), path);
          path.pop_back();
        }
      }
    }
    else {
      Result<T> result = {path, node->leaf};
      co_yield result;
    }
  }
}

/*
 * Find all words within Levenshtein distance `distance` of `word`.
 *
 * \tparam full
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
generator<Result<T>> _levenshtein(
    Node<alphabetSize, T>* node, vector<uint8_t>& word, size_t position,
    int distance, vector<uint8_t>& path) {
  if (distance >= 0) {
    // Deletion.
    co_yield _levenshtein<alphabetSize, T, full>(
      node, word, position + 1, distance - 1, path);

    uint8_t start = 0;
    if (!full && position < word.size()) {
      start = word[position];
    }
    for (uint8_t i = start; i < alphabetSize; i++) {
      if (node->child[i]) {
        path.push_back(i);

        // Substitution.
        if (position < word.size()) {
          co_yield _levenshtein<alphabetSize, T, full>(
            node->child[i], word, position + 1,
            distance - (uint8_t)(i != word[position]), path);
        }

        // Insertion.
        co_yield _levenshtein<alphabetSize, T, full>(
          node->child[i], word, position, distance - 1, path);

        path.pop_back();
      }
    }
    
    if (position >= word.size() && node->leaf) {
      Result<T> result = {path, node->leaf};
      co_yield result;
    }
  }
}
