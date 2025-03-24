#pragma once

#include <iostream>
#include <map>
#include <set>
#include <vector>
#include <algorithm>
#include <string>

struct Node;
struct Edge;

struct Node {
    std::string id;
    std::vector<Edge*> outgoing;
    std::vector<Edge*> incoming;

    Node(const std::string& id) : id(id) {}
};

struct Edge {
    Node* from;
    Node* to;
    int weight;

    Edge(Node* f, Node* t, int w) : from(f), to(t), weight(w) {}
};

class Graph {
private:
    std::map<std::string, Node*> nodes;
    std::vector<Edge*> edges;

    bool dfs(Node* node, std::set<std::string>& visited, std::set<std::string>& recStack,
             std::vector<std::string>& order, std::string& loopFrom, std::string& loopTo) {
        visited.insert(node->id);
        recStack.insert(node->id);

        for (Edge* e : node->outgoing) {
            Node* neighbor = e->to;
            if (!visited.count(neighbor->id)) {
                if (!dfs(neighbor, visited, recStack, order, loopFrom, loopTo)) return false;
            } else if (recStack.count(neighbor->id)) {
                loopFrom = node->id;
                loopTo = neighbor->id;
                return false;
            }
        }

        recStack.erase(node->id);
        order.push_back(node->id);
        return true;
    }

public:
    void add_node(const std::string& id);

    void add_edge(const std::string& from, const std::string& to, int weight);

    void remove_node(const std::string& id);

    void remove_edge(const std::string& from, const std::string& to);

    void rpo_numbering(const std::string& startId);
};
