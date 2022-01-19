#pragma once

#include <array>

#include <stdlib.h>
#include <stdint.h>

using std::array;


/*!
 * Leaf.
 */
struct Leaf {
  size_t count = 0;
};

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
  for (Node<alphabetSize, T>* c: child) {
    if (c) {
      return false;
    }
  }
  return true;
}
