#include "graph.hpp"

void Graph::add_node(const std::string& id) {
    if (!nodes.count(id)) {
        nodes[id] = new Node(id);
    }
}

void Graph::add_edge(const std::string& from, const std::string& to, int weight) {
    if (!nodes.count(from) && !nodes.count(to)) {
        std::cout << "Unknown nodes " << from << " " << to << std::endl;
        return;
    }
    if (!nodes.count(from)) {
        std::cout << "Unknown node " << from << std::endl;
        return;
    }
    if (!nodes.count(to)) {
        std::cout << "Unknown node " << to << std::endl;
        return;
    }
    Edge* e = new Edge(nodes[from], nodes[to], weight);
    nodes[from]->outgoing.push_back(e);
    nodes[to]->incoming.push_back(e);
    edges.push_back(e);
}

void Graph::remove_node(const std::string& id) {
    if (!nodes.count(id)) {
        std::cout << "Unknown node " << id << std::endl;
        return;
    }

    Node* node = nodes[id];

    for (Edge* e : node->incoming) {
        auto& out = e->from->outgoing;
        out.erase(std::remove(out.begin(), out.end(), e), out.end());
        edges.erase(std::remove(edges.begin(), edges.end(), e), edges.end());
        delete e;
    }

    for (Edge* e : node->outgoing) {
        auto& in = e->to->incoming;
        in.erase(std::remove(in.begin(), in.end(), e), in.end());
        edges.erase(std::remove(edges.begin(), edges.end(), e), edges.end());
        delete e;
    }

    delete node;
    nodes.erase(id);
}

void Graph::remove_edge(const std::string& from, const std::string& to) {
    if (!nodes.count(from) && !nodes.count(to)) {
        std::cout << "Unknown nodes " << from << " " << to << std::endl;
        return;
    }
    if (!nodes.count(from)) {
        std::cout << "Unknown node " << from << std::endl;
        return;
    }
    if (!nodes.count(to)) {
        std::cout << "Unknown node " << to << std::endl;
        return;
    }

    Node* n1 = nodes[from];
    Node* n2 = nodes[to];

    auto it = std::find_if(edges.begin(), edges.end(), [&](Edge* e) {
        return e->from == n1 && e->to == n2;
    });

    if (it != edges.end()) {
        Edge* e = *it;
        n1->outgoing.erase(std::remove(n1->outgoing.begin(), n1->outgoing.end(), e), n1->outgoing.end());
        n2->incoming.erase(std::remove(n2->incoming.begin(), n2->incoming.end(), e), n2->incoming.end());
        edges.erase(it);
        delete e;
    }
}

void Graph::rpo_numbering(const std::string& startId) {
    if (!nodes.count(startId)) {
        std::cout << "Unknown node " << startId << std::endl;
        return;
    }

    std::set<std::string> visited, recStack;
    std::vector<std::string> order;
    std::string loopFrom, loopTo;

    if (!dfs(nodes[startId], visited, recStack, order, loopFrom, loopTo)) {
        std::cout << "Found loop " << loopFrom << "->" << loopTo << std::endl;
    }

    std::reverse(order.begin(), order.end());
    for (const std::string& id : order) {
        std::cout << id << " ";
    }
    std::cout << std::endl;
}
