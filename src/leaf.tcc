#ifndef TRIE_LEAF_TCC_
#define TRIE_LEAF_TCC_

/*!
 * Leaf.
 */
template <class T>
class Leaf {
  public:
    size_t count = 0;

    virtual void add(T) {}
    virtual void remove(void) {}
};

template <>
class Leaf<void> {
  public:
    size_t count = 0;

    virtual void add(void) {}
    virtual void remove(void) {}
};

#endif
