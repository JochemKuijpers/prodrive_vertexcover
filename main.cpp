#include <iostream>
#include <fstream>
#include <vector>
#include <random>
#include <ctime>
#include <algorithm>

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

void report(bool cover[1993], int* bestCover) {
    int currentCover = 0;
    for (int i = 0; i < 1993; i++) if (cover[i]) currentCover++;

    if (currentCover < *bestCover) {
        std::cout << "Found an improved cover:\n";
        std::cout << "Previous: " << *bestCover << "\n";
        std::cout << "Current:  " << currentCover << "\n";
        for (int i =0; i < 1993; i++) {
            std::cout << (cover[i] ? '1' : '0') << (i < 1992 ? "," : "\n\n");
        }
        *bestCover = currentCover;
    }
}

int main() {
    auto readonly_graph = std::vector<std::pair<int, int>>();
    load_graph(&readonly_graph);

    int bestCover = 1993;
    bool cover[1993];
    std::default_random_engine generator(static_cast<unsigned int>(std::time(0)));

    long n = 0;
    while (true) {
        n++;
        if (n % 1000 == 0) { std::cout << n << " iterations... \n"; }
        for (bool &i : cover) i = false;
        auto graph = copy_graph(&readonly_graph);

        while (!graph.empty()) {
            std::uniform_int_distribution<int> distribution(0, graph.size() - 1);
            int edge_idx = distribution(generator);
            int u = graph[edge_idx].first;
            int v = graph[edge_idx].second;

            auto it = graph.begin();

            cover[u] = true;
            cover[v] = true;

            while (it != graph.end()) {
                if (it->first == u || it->first == v || it->second == u || it->second == v) {
                    it = graph.erase(it);
                } else {
                    it++;
                }
            }
        }

        report(cover, &bestCover);

        // we can remove a vertex from the cover if all of its neighbors are in the cover
        // however, since we're approximating the vertex cover, we want to do this in a randomized order
        int rnd_idx[1993];
        for (int i = 0; i < 1993; i++) rnd_idx[i] = i;
        std::shuffle(rnd_idx, rnd_idx + 1993, generator);

        for (int i = 0; i < 1993; i++) {
            int v = rnd_idx[i];
            // assume all of it's edges are covered by it's neighbors, if we find a counter_example, set to false
            bool fully_covered = true;

            for (auto const &edge : readonly_graph) {
                if (edge.first == v && !cover[edge.second] || edge.second == v && !cover[edge.first]) {
                    fully_covered = false;
                }
            }

            if (fully_covered) {
                cover[v] = false;
            }
        }

        report(cover, &bestCover);
    }

    return 0;
}