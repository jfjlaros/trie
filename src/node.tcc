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
struct Node {
  array<Node*, alphabetSize> child = {};
  T* leaf = NULL;
};
