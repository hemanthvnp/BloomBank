#pragma once

#include <string>
#include <unordered_map>
#include <utility>
#include <vector>

namespace bloombank {

// Undirected graph of account relationships, stored as an adjacency list.
// addRelationship / removeRelationship / areConnected: O(1) average,
// O(degree) worst case for a given account.
class RelationshipGraph {
public:
    void addRelationship(int accountId1, int accountId2, const std::string& relationType);
    bool removeRelationship(int accountId1, int accountId2);
    std::vector<std::pair<int, std::string>> getRelationships(int accountId) const;
    void displayRelationships(int accountId) const;
    std::vector<int> findAccountsByRelationType(int accountId, const std::string& relationType) const;
    bool areConnected(int accountId1, int accountId2) const;
    int getTotalRelationships() const;
    void displayAllRelationships() const;

private:
    std::unordered_map<int, std::vector<std::pair<int, std::string>>> adjacencyList_;
};

} // namespace bloombank
