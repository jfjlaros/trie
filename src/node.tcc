#pragma once

#include <array>
#include <memory>

#include <stdlib.h>
#include <stdint.h>

using std::array;
using std::unique_ptr;


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

  array<unique_ptr<Node>, alphabetSize> child {};  //!< Children.
  unique_ptr<T> leaf {nullptr};                    //!< Leaf.
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
  for (auto const& node : child) {
    if (node) {
      return false;
    }
  }
  return true;
}
