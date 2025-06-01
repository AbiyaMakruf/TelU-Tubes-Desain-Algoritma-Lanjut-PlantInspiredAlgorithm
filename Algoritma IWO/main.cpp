#include <iostream>
#include <fstream>
#include <random>
#include <algorithm>
#include <cmath>
#include <iomanip>
using namespace std;

const int MAX_NODES = 200;
const int MAX_EDGES = 2000;
const int INIT_POP = 10;
const int MAX_POP = 25;
const int MAX_IT = 200;
const int MIN_SEEDS = 0, MAX_SEEDS = 5;
const double SIGMA_INIT = 0.5, SIGMA_FINAL = 0.001, N_MOD = 2.0, PENALTY = 1000.0;

struct Graph {
    int n, m;
    pair<int,int> edges[MAX_EDGES];
    Graph() : n(0), m(0) {}
};

struct Weed {
    bool nodes[MAX_NODES];
    double fit;
    int cover, uncovered;
    Weed() : fit(0), cover(0), uncovered(0) {
        fill(nodes, nodes+MAX_NODES, false);
    }
    bool operator<(const Weed& o) const { return fit < o.fit; }
};

mt19937 gen(random_device{}());
double randd(double a, double b) {
    uniform_real_distribution<> d(a, b); return d(gen);
}
double randn(double mean, double std) {
    normal_distribution<> d(mean, std); return d(gen);
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

void calc_fit(Weed& w, const Graph& g) {
    w.cover = 0;
    for (int i = 0; i < g.n; ++i) w.cover += w.nodes[i];
    w.uncovered = 0;
    for (int i = 0; i < g.m; ++i)
        if (!w.nodes[g.edges[i].first] && !w.nodes[g.edges[i].second])
            w.uncovered++;
    w.fit = w.uncovered > 0 ? w.uncovered * PENALTY + w.cover : w.cover;
}

void init_pop(Weed pop[], int size, int n) {
    for (int i = 0; i < size; ++i)
        for (int j = 0; j < n; ++j)
            pop[i].nodes[j] = (randd(0,1) < 0.5);
}

Weed iwo(const Graph& g) {
    Weed pop[MAX_POP];
    int cur_pop = INIT_POP;
    init_pop(pop, cur_pop, g.n);

    Weed best; best.fit = 1e18;
    const int MAX_COMB = MAX_POP + MAX_POP * MAX_SEEDS;
    Weed comb[MAX_COMB];

    for (int it = 0; it < MAX_IT; ++it) {
        for (int i = 0; i < cur_pop; ++i) calc_fit(pop[i], g);

        double minf = pop[0].fit, maxf = pop[0].fit;
        for (int i = 1; i < cur_pop; ++i) {
            minf = min(minf, pop[i].fit);
            maxf = max(maxf, pop[i].fit);
        }

        int cidx = 0;
        for (int i = 0; i < cur_pop; ++i) comb[cidx++] = pop[i];

        for (int i = 0; i < cur_pop; ++i) {
            int num_seeds = (fabs(maxf-minf) < 1e-9)
                ? (MIN_SEEDS + MAX_SEEDS)/2
                : MIN_SEEDS + (int)round((maxf-pop[i].fit)/(maxf-minf)*(MAX_SEEDS-MIN_SEEDS));
            num_seeds = max(MIN_SEEDS, min(MAX_SEEDS, num_seeds));
            double sigma = pow((double)(MAX_IT-it-1)/MAX_IT, N_MOD) * (SIGMA_INIT-SIGMA_FINAL) + SIGMA_FINAL;
            sigma = max(sigma, SIGMA_FINAL);

            for (int s = 0; s < num_seeds && cidx < MAX_COMB; ++s) {
                Weed seed = pop[i];
                for (int d = 0; d < g.n; ++d) {
                    double pert = randn(0, sigma);
                    double cont = (pop[i].nodes[d] ? 1.0 : 0.0) + pert;
                    double sigm = 1.0/(1.0+exp(-cont));
                    seed.nodes[d] = (randd(0,1) < sigm);
                }
                calc_fit(seed, g);
                comb[cidx++] = seed;
            }
            if (cidx >= MAX_COMB) break;
        }

        sort(comb, comb + cidx);
        cur_pop = min(cidx, MAX_POP);
        for (int i = 0; i < cur_pop; ++i) pop[i] = comb[i];
        if (cur_pop > 0 && pop[0].fit < best.fit) best = pop[0];
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
        Weed best = iwo(g);
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
