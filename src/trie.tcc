#ifndef TRIE_TCC_
#define TRIE_TCC_

#include "trieFunctions.tcc"

/*!
 * Trie.
 */
template <uint8_t alphabetSize, class T>
class Trie {
  public:
    Trie(void);
    ~Trie(void);
    void add(vector<uint8_t>&);
    template <class U>
      void add(vector<uint8_t>&, U&);
    void remove(vector<uint8_t>&);
    Node<alphabetSize, T>* find(vector<uint8_t>&);
    template <class R>
      R traverse(void (*)(vector<uint8_t>&, T&, R&));
    void traverse(void (*)(vector<uint8_t>&, T&));
    template <class R>
      R hamming(vector<uint8_t>&, int, void (*)(vector<uint8_t>&, T&, R&));

  private:
    Node<alphabetSize, T>* _root = NULL;
};


/*!
 * Constructor.
 */
template <uint8_t alphabetSize, class T>
Trie<alphabetSize, T>::Trie(void) {
  _root = new Node<alphabetSize, T>;
}

/*!
 * Destructor.
 */
template <uint8_t alphabetSize, class T>
Trie<alphabetSize, T>::~Trie(void) {
  _delete(_root);
}

/*!
 * Add a word.
 *
 * \param word Word.
 *
 * \return Leaf.
 */
template <uint8_t alphabetSize, class T>
void Trie<alphabetSize, T>::add(vector<uint8_t>& word) {
  _add(_root, word);
}

/*!
 * Add a word.
 *
 * \param word Word.
 * \param data Data.
 *
 * \return Leaf.
 */
template <uint8_t alphabetSize, class T>
template <class U>
void Trie<alphabetSize, T>::add(vector<uint8_t>& word, U& data) {
  _add(_root, word, data);
}

/*!
 * Remove a word.
 *
 * \param word Word.
 */
template <uint8_t alphabetSize, class T>
void Trie<alphabetSize, T>::remove(vector<uint8_t>& word) {
  _remove(_root, word, 0);
}

/*!
 * Find a word.
 *
 * \param word Word.
 *
 * \return `node` if found, `NULL` otherwise.
 */
template <uint8_t alphabetSize, class T>
Node<alphabetSize, T>* Trie<alphabetSize, T>::find(vector<uint8_t>& word) {
  return _find(_root, word);
}

/*!
 * Traverse.
 *
 * \param visit Callback function.
 *
 * \return Traversal result.
 */
template <uint8_t alphabetSize, class T>
template <class R>
R Trie<alphabetSize, T>::traverse(void (*visit)(vector<uint8_t>&, T&, R&)) {
  vector<uint8_t> word;
  R result;
  _traverse(_root, word, visit, result);
  return result;
}

/*!
 * Traverse.
 *
 * \param visit Callback function.
 *
 * \return Traversal result.
 */
template <uint8_t alphabetSize, class T>
void Trie<alphabetSize, T>::traverse(void (*visit)(vector<uint8_t>&, T&)) {
  vector<uint8_t> word;
  _traverse(_root, word, visit);
}

/*!
 * Hamming.
 *
 * \param word Word.
 * \param distance Maximum distance.
 * \param visit Callback function.
 *
 * \return Traversal result.
 */
template <uint8_t alphabetSize, class T>
template <class R>
R Trie<alphabetSize, T>::hamming(
    vector<uint8_t>& word, int distance,
    void (*visit)(vector<uint8_t>&, T&, R&)) {
  vector<uint8_t> path;
  R result;
  _hamming(_root, word, 0, distance, path, visit, result);
  return result;
}

#endif
