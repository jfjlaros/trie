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
    array<Node*, alphabetSize> child = {};
    T* leaf = NULL;

    bool isEmpty();
};


/*!
 * Check whether a node neither has any children, nor a leaf.
 *
 * \return True is the node is empty, false otherwise.
 */
template <uint8_t alphabetSize, class T>
bool Node<alphabetSize, T>::isEmpty() {
  if (leaf) {
    return false;
  }
  for (Node* node: child) {
    if (node) {
      return false;
    }
  }
  return true;
}
