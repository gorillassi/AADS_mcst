#include "graph.hpp"

int main() {
    Graph graph;
    std::string cmd;

    while(std::cin >> cmd){
        if (cmd == "NODE") {
            std::string id;
            std::cin >> id;
            graph.add_node(id);
        } else if (cmd == "EDGE") {
            std::string from, to;
            int w;
            std::cin >> from >> to >> w;
            graph.add_edge(from, to, w);
        } else if (cmd == "REMOVE") {
            std::string type;
            std::cin >> type;
            if (type == "NODE") {
                std::string id;
                std::cin >> id;
                graph.remove_node(id);
            } else if (type == "EDGE") {
                std::string from, to;
                std::cin >> from >> to;
                graph.remove_edge(from, to);
            }
        } else if (cmd == "RPO_NUMBERING") {
            std::string start;
            std::cin >> start;
            graph.rpo_numbering(start);
        }
    }
    return 0;
}
