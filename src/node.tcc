#ifndef TRIE_NODE_TCC_
#define TRIE_NODE_TCC_

#include <array>
#include <forward_list>

#include <stdlib.h>
#include <stdint.h>

#include "leaf.tcc"

using std::array;


/*!
 * Node.
 */
template <uint8_t alphabetSize, class T>
class Node {
  public:
    array<Node*, alphabetSize> child;
    T* leaf = NULL;

    Node(void);
    bool isEmpty(void);
};


/*!
 * Constructor.
 */
template <uint8_t alphabetSize, class T>
Node<alphabetSize, T>::Node(void) {
  for (uint8_t i = 0; i < alphabetSize; i++) {
    child[i] = NULL;
  }
}

/*!
 * Check whether the node is empty.
 *
 * \return `true` if node is empty, `false` otherwise.
 */
template <uint8_t alphabetSize, class T>
bool Node<alphabetSize, T>::isEmpty(void) {
  if (leaf) {
    return false;
  }
  for (uint8_t i = 0; i < alphabetSize; i++) {
    if (child[i]) {
      return false;
    }
  }
  return true;
}

#endif
