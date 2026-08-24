#include "test_framework.hpp"
#include "bloombank/RelationshipGraph.hpp"

using bloombank::RelationshipGraph;

TEST(AddRelationshipIsBidirectional) {
    RelationshipGraph graph;
    graph.addRelationship(1, 2, "friend");
    EXPECT_TRUE(graph.areConnected(1, 2));
    EXPECT_TRUE(graph.areConnected(2, 1));
}

TEST(RemoveRelationshipDisconnects) {
    RelationshipGraph graph;
    graph.addRelationship(1, 2, "friend");
    EXPECT_TRUE(graph.removeRelationship(1, 2));
    EXPECT_FALSE(graph.areConnected(1, 2));
}

TEST(RemoveMissingRelationshipReturnsFalse) {
    RelationshipGraph graph;
    EXPECT_FALSE(graph.removeRelationship(1, 2));
}

TEST(FindAccountsByRelationTypeFiltersCorrectly) {
    RelationshipGraph graph;
    graph.addRelationship(1, 2, "friend");
    graph.addRelationship(1, 3, "family");

    auto friends = graph.findAccountsByRelationType(1, "friend");
    EXPECT_EQ(friends.size(), static_cast<size_t>(1));
    EXPECT_EQ(friends[0], 2);
}

TEST(TotalRelationshipsCountsEachPairOnce) {
    RelationshipGraph graph;
    graph.addRelationship(1, 2, "friend");
    graph.addRelationship(1, 3, "family");
    EXPECT_EQ(graph.getTotalRelationships(), 2);
}
