#include <catch.hpp>
#include <memory>

#include "../src/node.tcc"

using std::make_unique;


TEST_CASE("Node", "[node]") {
  Node<4, Leaf> node;

  SECTION("Empty node") {
    REQUIRE(node.isEmpty());
  }

  SECTION("Leaf node") {
    node.leaf = make_unique<Leaf>();

    REQUIRE(not node.isEmpty());
  }

  SECTION("Internal node") {
    node.child[3] = make_unique<Node<4, Leaf>>();
  
    REQUIRE(not node.isEmpty());
  }
}
