#pragma once

#include <array>

#include <stdlib.h>
#include <stdint.h>

using std::array;


/*! Leaf. */
struct Leaf {
  size_t count {0};  //!< Counter.
};

/*! Node.
 *
 * \tparam alphabetSize Size of the alphabet.
 * \tparam T Leaf type.
 */
template <uint8_t alphabetSize, class T>
class Node {
public:
  bool isEmpty() const;

  array<Node*, alphabetSize> child {};  //!< Children.
  T* leaf {nullptr};                    //!< Leaf.
};


/*! Check whether a node neither has any children, nor a leaf.
 *
 * \return True is the node is empty, false otherwise.
 */
template <uint8_t alphabetSize, class T>
bool Node<alphabetSize, T>::isEmpty() const {
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
