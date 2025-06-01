#include <iostream>
#include <fstream>
#include <random>
#include <algorithm>
#include <cmath>
#include <iomanip>
using namespace std;

const int MAX_NODES = 100;
const int MAX_EDGES = 2000;
const int POP_SIZE = 30;
const int MAX_GEN = 100;
const int MAX_RUNNERS = 5;
const double PENALTY = 1000.0;

struct Graph {
    int n;
    pair<int,int> edges[MAX_EDGES];
    int m;
    Graph() : n(0), m(0) {}
};

struct Plant {
    bool nodes[MAX_NODES];
    double fit, norm_fit, map_fit;
    int cover, uncovered;
    Plant() : fit(0), norm_fit(0), map_fit(0), cover(0), uncovered(0) {
        fill(nodes, nodes+MAX_NODES, false);
    }
};

mt19937 gen(random_device{}());
double randd(double a, double b) {
    uniform_real_distribution<> d(a, b);
    return d(gen);
}

Graph read_graph(const string& file) {
    Graph g;
    ifstream in(file);
    if (!in) return g;
    in >> g.n;
    int u, v;
    while (g.m < MAX_EDGES && in >> u >> v)
        if (u >= 0 && u < g.n && v >= 0 && v < g.n)
            g.edges[g.m++] = {u, v};
    return g;
}

void calc_fit(Plant& p, const Graph& g) {
    p.cover = 0;
    for (int i = 0; i < g.n; ++i) p.cover += p.nodes[i];
    p.uncovered = 0;
    for (int i = 0; i < g.m; ++i)
        if (!p.nodes[g.edges[i].first] && !p.nodes[g.edges[i].second])
            p.uncovered++;
    p.fit = p.uncovered > 0 ? p.uncovered * PENALTY + p.cover : p.cover;
}

void init_pop(Plant pop[], int size, int n) {
    for (int i = 0; i < size; ++i)
        for (int j = 0; j < n; ++j)
            pop[i].nodes[j] = (randd(0,1) < 0.5);
}

bool cmp(const Plant& a, const Plant& b) {
    return a.fit < b.fit;
}

Plant ppa(const Graph& g) {
    Plant pop[POP_SIZE], best;
    best.fit = 1e18;
    init_pop(pop, POP_SIZE, g.n);

    const int MAX_COMB = POP_SIZE + POP_SIZE * MAX_RUNNERS;
    Plant comb[MAX_COMB];

    for (int gen = 0; gen < MAX_GEN; ++gen) {
        for (int i = 0; i < POP_SIZE; ++i) calc_fit(pop[i], g);
        sort(pop, pop + POP_SIZE, cmp);
        if (pop[0].fit < best.fit) best = pop[0];

        double minf = pop[0].fit, maxf = pop[POP_SIZE-1].fit;
        for (int i = 0; i < POP_SIZE; ++i) {
            auto& p = pop[i];
            p.norm_fit = (fabs(maxf-minf) < 1e-9) ? 0.5 : (maxf-p.fit)/(maxf-minf);
            p.map_fit = 0.5 * (tanh(4*p.norm_fit-2) + 1.0);
        }

        int cidx = 0;
        for (int i = 0; i < POP_SIZE; ++i) comb[cidx++] = pop[i];

        for (int i = 0; i < POP_SIZE; ++i) {
            const auto& p = pop[i];
            int num_runners = max(1, (int)ceil(MAX_RUNNERS * p.map_fit * randd(0,1)));
            for (int r = 0; r < num_runners && cidx < MAX_COMB; ++r) {
                Plant runner = p;
                for (int d = 0; d < g.n; ++d) {
                    double dval = 2.0 * (1-p.map_fit) * (randd(0,1)-0.5);
                    double cont = (p.nodes[d] ? 1.0 : 0.0) + dval;
                    double sgm = 1.0/(1.0+exp(-cont));
                    runner.nodes[d] = (randd(0,1) < sgm);
                }
                calc_fit(runner, g);
                comb[cidx++] = runner;
            }
            if (cidx >= MAX_COMB) break;
        }
        sort(comb, comb+cidx, cmp);
        for (int i = 0; i < POP_SIZE; ++i) pop[i] = comb[i];
    }
    if (best.fit == 1e18 && g.n > 0) {
        calc_fit(pop[0], g);
        if (pop[0].fit < best.fit) best = pop[0];
        else calc_fit(best, g);
    }
    return best;
}

int main() {
    for (int n = 10; n <= 200; n += 10) {
        string fname = "graph_" + to_string(n) + ".dat";
        cout << "=== Menguji file: " << fname << " ===\n";
        Graph g = read_graph(fname);
        if (g.n == 0) {
            cout << "Graph error or empty.\n\n";
            continue;
        }
        Plant best = ppa(g);
        cout << "Best cover size: ";
        if (best.uncovered == 0) {
            cout << best.cover << "\nVertices: ";
            for (int i = 0, f = 1; i < g.n; ++i)
                if (best.nodes[i]) cout << (f ? "" : " ") << i, f = 0;
            if (best.cover == 0) cout << "(No vertex needed)";
            cout << '\n';
        } else {
            cout << "No valid cover found.\n";
            cout << "Best fitness: " << fixed << setprecision(2) << best.fit << "\n";
            cout << "Uncovered edges: " << best.uncovered << "\n";
            cout << "Partial cover size: " << best.cover << "\n";
        }
        cout << "Raw fitness: " << fixed << setprecision(2) << best.fit << "\n";
        cout << "-----------------------------\n\n";
    }
    return 0;
}
