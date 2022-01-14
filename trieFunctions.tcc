#ifndef TRIE_TRIE_FUNCTIONS_TCC_
#define TRIE_TRIE_FUNCTIONS_TCC_

#include <vector>

#include "node.tcc"

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
 * \param data Data.
 */
template <uint8_t alphabetSize, class T, class U>
void _add(Node<alphabetSize, T>* node, vector<uint8_t>& word, U data) {
  for (uint8_t letter: word) {
    if (!node->child[letter]) {
      node->child[letter] = new Node<alphabetSize, T>;
    }
    node = node->child[letter];
  }
  if (!node->leaf) {
    node->leaf = new T;
  }
  node->leaf->push(data);
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
      node->leaf->pop();
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

#endif
