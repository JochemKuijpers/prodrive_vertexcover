#include <iostream>
#include <fstream>
#include <vector>
#include <random>

void load_graph(std::vector<std::pair<int, int>>* pGraph) {
    std::ifstream input("../graph");
    std::string line;

    while (std::getline(input, line, ';')) {
        int u, v;
        size_t commaPosition = line.find(',');
        u = std::stoi(line.substr(0, commaPosition));
        v = std::stoi(line.substr(commaPosition + 1, line.length() - commaPosition - 1));
        pGraph->push_back(std::make_pair(u, v));
        pGraph->push_back(std::make_pair(v, u));
    }
}

std::vector<std::pair<int, int>> copy_graph(std::vector<std::pair<int, int>>* pGraph) {
    auto newGraph = *pGraph;
    return newGraph;
}

int main() {
    auto loaded_graph = std::vector<std::pair<int, int>>();
    load_graph(&loaded_graph);

    int bestCover = 1993;
    bool cover[1993];
    std::default_random_engine generator;

    while (true) {
        for (int i = 0; i < 1993; i++) cover[i] = false;
        auto graph = copy_graph(&loaded_graph);

        while (!graph.empty()) {
            std::uniform_int_distribution<int> distribution(0, graph.size());
            int edge_idx = distribution(generator);
            int u = graph[edge_idx].first;
            int v = graph[edge_idx].second;

            auto it = graph.begin();

            cover[u] = 1;
            cover[v] = 1;

            while (it != graph.end()) {
                if (it->first == u || it->first == v || it->second == u || it->second == v) {
                    it = graph.erase(it);
                } else {
                    it++;
                }
            }
        }

        int currentCover = 0;
        for (int i = 0; i < 1993; i++) {
            if (cover[i]) currentCover++;
        }
        if (currentCover < bestCover) {
            std::cout << "Found an improved cover:\n";
            std::cout << "Previous: " << bestCover << "\n";
            std::cout << "Current:  " << currentCover << "\n";
            for (int i =0; i < 1993; i++) {
                std::cout << (cover[i] ? '1' : '0') << (i < 1992 ? "," : "\n\n");
            }
            bestCover = currentCover;
        }
    }

    return 0;
}