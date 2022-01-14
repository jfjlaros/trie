#ifndef TRIE_RECORD_TCC
#define TRIE_RECORD_TCC

template <class T>
class Record {
  public:
    T* data = NULL;
    Record<T>* next = NULL;
};

#endif
