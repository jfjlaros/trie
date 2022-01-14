#ifndef TRIE_LEAF_TCC_
#define TRIE_LEAF_TCC_

/*!
 * Leaf.
 */
template <class T>
class Leaf {
  public:
    size_t count = 0;

    void push(T);
    T pop(void);
};


/*!
 * Add data.
 *
 * This function is meant to be overridden.
 */
template <class T>
void Leaf<T>::push(T) {
  count++;
}

/*!
 * Remove data.
 *
 * This function is meant to be overridden.
 */
template <class T>
T Leaf<T>::pop(void) {
  count--;
  return 0;
}

#endif
