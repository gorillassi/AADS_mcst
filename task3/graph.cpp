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

void Graph::dijkstra(const std::string& startId){
    if(!nodes.count(startId)) {
        std::cout << "Unknown node " << startId << std::endl;
        return;
    }

    std::map<std::string, int> dist;
    for (const auto& [id, node] : nodes) {
        dist[id] = INF;
    }
    dist[startId] = 0;

    std::priority_queue<
        std::pair<int, std::string>,
        std::vector<std::pair<int, std::string>>,
        std::greater<std::pair<int, std::string>>
    > queue;

    queue.push({0, startId});

    while(!queue.empty()) {
        std::pair<int, std::string> current = queue.top();
        queue.pop();

        int currentDist = current.first;
        std::string currentNode = current.second;

        if(currentDist > dist[currentNode]) {
            continue;
        }

        for(Edge* edge : nodes[currentNode]->outgoing){
            std::string neighbor = edge->to->id;
            int weight = edge->weight;

            int newDist = dist[currentNode] + weight;

            if(newDist < dist[neighbor]){
                dist[neighbor] = newDist;
                queue.push({newDist, neighbor});
            }
        }
    }

    for (const auto& entry : dist) {
        const std::string& id = entry.first;
        int distance = entry.second;
    
        if(id == startId){
            continue;
        }

        std::cout << id << " ";
        if (distance == INF) {
            std::cout << "unreachable"; 
        } else {
            std::cout << distance; 
        }
        std::cout << std::endl;
    }
}

void Graph::max_flow(const std::string& sourceId, const std::string& sinkId){
    if (!nodes.count(sourceId) || !nodes.count(sinkId)) {
        std::cout << "Unknown nodes " << sourceId << " " << sinkId << std::endl;
        return;
    }

    for (Edge* e : edges) {
        e->flow = 0;
        if (!e->reverse) {
            Edge* rev = new Edge(e->to, e->from, 0);
            rev->reverse = e;
            e->reverse = rev;
            edges.push_back(rev);
            e->to->outgoing.push_back(rev);
            e->from->incoming.push_back(rev);
        }
    }

    Node* source = nodes[sourceId];
    Node* sink = nodes[sinkId];

    int maxFlow = 0;
    std::map<Node*, Edge*> parentMap;

    while (bfs(parentMap, source, sink)) {
        int pathFlow = INF;
        for (Node* v = sink; v != source; ) {
            Edge* e = parentMap[v];
            pathFlow = std::min(pathFlow, e->weight - e->flow);
            v = e->from;
        }

        for (Node* v = sink; v != source; ) {
            Edge* e = parentMap[v];
            e->flow += pathFlow;
            e->reverse->flow -= pathFlow;
            v = e->from;
        }

        maxFlow += pathFlow;
        parentMap.clear();
    }

    std::cout << maxFlow << std::endl;
}
