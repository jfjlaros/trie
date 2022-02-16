Usage
=====

Include the header file to use the trie library.

.. code-block:: cpp

    #include "trie.tcc"

The library provides the ``Trie`` class, which takes two template arguments,
the first of which determines the alphabet size, the second determines the
type of the leaf.

.. code-block:: cpp

    Trie<4, Leaf> trie;

.. code-block:: cpp

    vector<uint8_t> word = {0, 1, 2, 3};
    trie.add(word);

.. code-block:: cpp

    Node<4, Leaf>* node = trie.find(word);

.. code-block:: cpp

    trie.remove(word);

.. code-block:: cpp

    for (Result<Leaf> result: trie.walk()) {
      // result.leaf : Leaf node.
      // result.path : Word leading up to the leaf.
    }

.. code-block:: cpp

    for (Result<Leaf> result: trie.hamming(word, 1)) {
      // result.leaf : Leaf node.
      // result.path : Word leading up to the leaf.
    }

.. code-block:: cpp

    struct MyLeaf : Leaf {
      vector<size_t> lines;
    }

.. code-block:: cpp

    size_t line = 0;
    for (vector<uint8_t> word: words) {
      MyLeaf* leaf = trie.add(word);
      leaf->lines.push_back(line++);
    }
