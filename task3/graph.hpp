#pragma once

#include <iostream>
#include <map>
#include <set>
#include <vector>
#include <algorithm>
#include <string>
#include <queue>
#include <limits>

const int INF = std::numeric_limits<int>::max();

struct Node;
struct Edge;

struct Edge {
    Node* from;
    Node* to;
    int weight;
    int flow = 0;
    Edge* reverse = nullptr;
    bool isResidual = false;

    Edge(Node* f, Node* t, int w) : from(f), to(t), weight(w) {}
};

struct Node {
    std::string id;
    std::vector<Edge*> outgoing;
    std::vector<Edge*> incoming;

    Node(const std::string& id) : id(id) {}
};

class Graph {
private:
    std::map<std::string, Node*> nodes;
    std::vector<Edge*> edges;

    bool dfs(Node* node, std::set<std::string>& visited, std::set<std::string>& recStack,
             std::vector<std::string>& order, std::string& loopFrom, std::string& loopTo){

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
    bool bfs(std::map<Node*, Edge*>& parentMap, Node* source, Node* sink){
        std::set<Node*> visited;
        std::queue<Node*> q;
        q.push(source);
        visited.insert(source);

        while (!q.empty()) {
            Node* current = q.front(); q.pop();

            for (Edge* e : current->outgoing) {
                int capacity = e->weight - e->flow;
                if (capacity > 0 && !visited.count(e->to)) {
                    visited.insert(e->to);
                    parentMap[e->to] = e;
                    q.push(e->to);
                    if (e->to == sink) return true;
                }
            }
        }
        return false;
    }

public:
    ~Graph();

    void add_node(const std::string& id);

    void add_edge(const std::string& from, const std::string& to, int weight);

    void remove_edge(const std::string& from, const std::string& to);

    void remove_node(const std::string& id);

    void rpo_numbering(const std::string& startId);

    void dijkstra(const std::string& startId);

    void max_flow(const std::string& sourceId, const std::string& sinkId);
};

inline Graph::~Graph() {
    for (auto& [id, node] : nodes) {
        delete node;
    }
    nodes.clear();

    std::set<Edge*> uniqueEdges;
    for (Edge* e : edges) {
        uniqueEdges.insert(e);
    }
    for (Edge* e : uniqueEdges) {
        delete e;
    }
    edges.clear();
}
