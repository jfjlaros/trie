#include <catch.hpp>

#include "../src/node.tcc"


TEST_CASE("Empty node", "[node]") {
  Node<4, Leaf> node;

  REQUIRE(node.isEmpty());
}

TEST_CASE("Leaf node", "[node]") {
  Node<4, Leaf> node;
  Leaf leaf;
  node.leaf = &leaf;

  REQUIRE(!node.isEmpty());
}

TEST_CASE("Internal node", "[node]") {
  Node<4, Leaf> node;
  Node<4, Leaf> child;
  node.child[3] = &child;

  REQUIRE(!node.isEmpty());
}
