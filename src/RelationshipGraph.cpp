#include "bloombank/RelationshipGraph.hpp"

#include <algorithm>
#include <iostream>
#include <set>

namespace bloombank {

void RelationshipGraph::addRelationship(int accountId1, int accountId2, const std::string& relationType) {
    adjacencyList_[accountId1].push_back({accountId2, relationType});
    adjacencyList_[accountId2].push_back({accountId1, relationType});
}

bool RelationshipGraph::removeRelationship(int accountId1, int accountId2) {
    bool removed = false;

    auto it1 = adjacencyList_.find(accountId1);
    if (it1 != adjacencyList_.end()) {
        auto& connections = it1->second;
        for (auto it = connections.begin(); it != connections.end();) {
            if (it->first == accountId2) {
                it = connections.erase(it);
                removed = true;
            } else {
                ++it;
            }
        }
    }

    auto it2 = adjacencyList_.find(accountId2);
    if (it2 != adjacencyList_.end()) {
        auto& connections = it2->second;
        for (auto it = connections.begin(); it != connections.end();) {
            if (it->first == accountId1) {
                it = connections.erase(it);
                removed = true;
            } else {
                ++it;
            }
        }
    }

    return removed;
}

std::vector<std::pair<int, std::string>> RelationshipGraph::getRelationships(int accountId) const {
    auto it = adjacencyList_.find(accountId);
    return it != adjacencyList_.end() ? it->second : std::vector<std::pair<int, std::string>>{};
}

void RelationshipGraph::displayRelationships(int accountId) const {
    std::cout << "\n===== Relationships for Account #" << accountId << " =====" << std::endl;
    auto it = adjacencyList_.find(accountId);
    if (it == adjacencyList_.end() || it->second.empty()) {
        std::cout << "No relationships found." << std::endl;
        return;
    }
    for (const auto& relation : it->second) {
        std::cout << "Connected to Account #" << relation.first << " (" << relation.second << ")" << std::endl;
    }
    std::cout << "=======================" << std::endl;
}

std::vector<int> RelationshipGraph::findAccountsByRelationType(int accountId, const std::string& relationType) const {
    std::vector<int> result;
    auto it = adjacencyList_.find(accountId);
    if (it != adjacencyList_.end()) {
        for (const auto& relation : it->second) {
            if (relation.second == relationType) {
                result.push_back(relation.first);
            }
        }
    }
    return result;
}

bool RelationshipGraph::areConnected(int accountId1, int accountId2) const {
    auto it = adjacencyList_.find(accountId1);
    if (it == adjacencyList_.end()) {
        return false;
    }
    return std::any_of(it->second.begin(), it->second.end(),
                        [accountId2](const std::pair<int, std::string>& relation) { return relation.first == accountId2; });
}

int RelationshipGraph::getTotalRelationships() const {
    int total = 0;
    for (const auto& entry : adjacencyList_) {
        total += static_cast<int>(entry.second.size());
    }
    return total / 2;
}

void RelationshipGraph::displayAllRelationships() const {
    std::cout << "\n===== All Relationships =====" << std::endl;
    if (adjacencyList_.empty()) {
        std::cout << "No relationships found." << std::endl;
        return;
    }
    std::set<std::pair<int, int>> processedPairs;
    for (const auto& entry : adjacencyList_) {
        int accountId1 = entry.first;
        for (const auto& relation : entry.second) {
            int accountId2 = relation.first;
            std::pair<int, int> relationPair = {std::min(accountId1, accountId2), std::max(accountId1, accountId2)};
            if (processedPairs.insert(relationPair).second) {
                std::cout << "Account #" << accountId1 << " and Account #" << accountId2
                           << " (" << relation.second << ")" << std::endl;
            }
        }
    }
    std::cout << "=======================" << std::endl;
}

} // namespace bloombank
