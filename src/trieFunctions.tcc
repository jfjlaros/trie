#ifndef TRIE_TRIE_FUNCTIONS_TCC_
#define TRIE_TRIE_FUNCTIONS_TCC_

#include <map>
#include <vector>

#include "node.tcc"

using std::pair;
using std::vector;


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
 * Add a word to a (sub)trie.
 *
 * \param node Root.
 * \param word Word.
 * \param data Data.
 */
template <uint8_t alphabetSize, class T, class U>
void _add(Node<alphabetSize, T>* node, vector<uint8_t>& word, U& data) {
  T* leaf = _add(node, word);
  leaf->add(data);
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
      node->leaf->remove();
      if (!node->leaf->count) {
        delete node->leaf;
        node->leaf = NULL;
      }
      return true;
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
 * \param word Word.
 * \param visit Callback function.
 * \param result Traversal result.
 */
template <uint8_t alphabetSize, class T, class R>
void _traverse(
    Node<alphabetSize, T>* node, vector<uint8_t>& word,
    void (*visit)(vector<uint8_t>&, T&, R&), R& result) {
  if (node->leaf) {
    visit(word, *node->leaf, result);
  }
  for (size_t i = 0; i < alphabetSize; i++) {
    if (node->child[i]) {
      word.push_back(i);
      _traverse(node->child[i], word, visit, result);
      word.pop_back();
    }
  }
}

/*
 * Traverse a (sub)trie.
 *
 * \param node Root.
 * \param word Word.
 * \param visit Callback function.
 */
template <uint8_t alphabetSize, class T>
void _traverse(
    Node<alphabetSize, T>* node, vector<uint8_t>& word,
    void (*visit)(vector<uint8_t>&, T&)) {
  if (node->leaf) {
    visit(word, *node->leaf);
  }
  for (size_t i = 0; i < alphabetSize; i++) {
    if (node->child[i]) {
      word.push_back(i);
      _traverse(node->child[i], word, visit);
      word.pop_back();
    }
  }
}

/*
 * Find all words within Hamming distance `distance`.
 *
 * \param node Root.
 * \param word Word.
 * \param position Position in `word`.
 * \param distance Maximum distance.
 * \param path Path.
 * \param visit Callback function.
 * \param result Traversal result.
 */
template <uint8_t alphabetSize, class T, class R>
void _hamming(
    Node<alphabetSize, T>* node, vector<uint8_t>& word, size_t position,
    int distance, vector<uint8_t>& path,
    void (*visit)(vector<uint8_t>&, T&, R&), R& result) {
  if (distance < 0) {
    return;
  }
  if (position == word.size()) {
    if (!distance && node->leaf) {
      visit(path, *node->leaf, result);
    }
    return;
  }

  for (uint8_t i = word[position]; i < alphabetSize; i++) {
    if (node->child[i]) {
      path.push_back(i);
      _hamming(
        node->child[i], word, position + 1,
        distance - (uint8_t)(i != word[position]), path, visit, result);
      path.pop_back();
    }
  }
}

template <uint8_t alphabetSize, class T>
generator<pair<vector<uint8_t>, T>> _walk(
    Node<alphabetSize, T>* node, vector<uint8_t>& path) {
  if (!node->leaf) {
    for (size_t i = 0; i < alphabetSize; i++) {
      if (node->child[i]) {
        path.push_back(i);
        co_yield _walk(node->child[i], path);
        path.pop_back();
      }
    }
  }
  else {
    co_yield pair(path, *node->leaf);
  }
}

template <uint8_t alphabetSize, class T>
generator<pair<vector<uint8_t>, T>> _hamming(
    Node<alphabetSize, T>* node, vector<uint8_t>& word, size_t position,
    int distance, vector<uint8_t>& path) {
  if (distance >= 0) {
    if (position < word.size()) {
      for (uint8_t i = word[position]; i < alphabetSize; i++) {
        if (node->child[i]) {
          path.push_back(i);
          co_yield _hamming(
            node->child[i], word, position + 1,
            distance - (uint8_t)(i != word[position]), path);
          path.pop_back();
        }
      }
    }
    else {
      co_yield pair(path, *node->leaf);
    }
  }
}

#endif
