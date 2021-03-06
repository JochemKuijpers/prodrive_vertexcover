#include <iostream>
#include <fstream>
#include <vector>
#include <random>
#include <ctime>
#include <algorithm>

// loads graph from one directory up: "../graph".
void load_graph(std::vector<std::pair<int, int>>* pGraph) {
    std::ifstream input("../graph");
    std::string line;

    while (std::getline(input, line, ';')) {
        int u, v;
        size_t commaPosition = line.find(',');
        u = std::stoi(line.substr(0, commaPosition));
        v = std::stoi(line.substr(commaPosition + 1, line.length() - commaPosition - 1));
        pGraph->push_back(std::make_pair(u, v));
    }
}

// check if the cover is better than we already found, and if so, output the cover.
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

    while (true) {
        // initialize all cover booleans to true: everyone is in the cover at first.
        for (bool &i : cover) i = true;

        report(cover, &bestCover);

        // The following transformations are applied in an arbitrary order to the vertices of the graph.
        // To achieve this arbitrary ordering, we shuffle a list of vertex indices.
        int rnd_idx[1993];
        for (int i = 0; i < 1993; i++) rnd_idx[i] = i;

        std::cout << "--\n";

        for (int trans = 0; trans < 10000; trans++) {
            // If all neighbors are in the vertex cover, we can remove the center vertex from the cover.
            // And if only one of the neighbors is not in the vertex cover, we can switch with that neighbor.
            std::shuffle(rnd_idx, rnd_idx + 1993, generator);
            for (int i = 0; i < 1993; i++) {
                int v = rnd_idx[i];
                if (!cover[v]) { continue; }

                int non_cover_neighbor = -1; // the index of the neighbor not in the cover
                bool ignore = false; // set to true if more than one neighbors was not in the cover
                for (auto const &edge : readonly_graph) {
                    if (edge.first == v && !cover[edge.second]) {
                        if (non_cover_neighbor < 0) {
                            non_cover_neighbor = edge.second;
                        } else {
                            ignore = true; break;
                        }
                    } else if (edge.second == v && !cover[edge.first]) {
                        if (non_cover_neighbor < 0) {
                            non_cover_neighbor = edge.first;
                        } else {
                            ignore = true; break;
                        }
                    }
                }

                if (non_cover_neighbor >= 0 && !ignore) {
                    cover[v] = false;
                    cover[non_cover_neighbor] = true;
                } else if (non_cover_neighbor < 0) {
                    cover[v] = false;
                }
            }

            report(cover, &bestCover);
        }
    }

    return 0;
}