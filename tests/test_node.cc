#include <catch.hpp>

#include "../src/node.tcc"


TEST_CASE("Node", "[node]") {
  Node<4, Leaf> node;

  SECTION("Empty node") {
    REQUIRE(node.isEmpty());
  }

  SECTION("Leaf node") {
    Leaf leaf;
    node.leaf = &leaf;

    REQUIRE(not node.isEmpty());
  }

  SECTION("Internal node") {
    Node<4, Leaf> child;
    node.child[3] = &child;
  
    REQUIRE(not node.isEmpty());
  }
}
