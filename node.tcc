#ifndef TRIE_NODE_TCC_
#define TRIE_NODE_TCC_

#include <array>

#include <stdlib.h>
#include <stdint.h>

using std::array;

template <uint8_t alphabetSize>
class Node {
  public:
    array<Node*, alphabetSize> child;
    bool isEmpty(void);
    size_t size;
    Node(void);
};


/*!
 * Constructor.
 */
template <uint8_t alphabetSize>
Node<alphabetSize>::Node(void) {
  for (uint8_t i = 0; i < alphabetSize; i++) {
    child[i] = NULL;
  }
  size = 0;
}

/*!
 * Check whether the node is empty.
 *
 * return `true` if node is empty, `false` otherwise.
 */
template <uint8_t alphabetSize>
bool Node<alphabetSize>::isEmpty(void) {
  if (size) {
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
