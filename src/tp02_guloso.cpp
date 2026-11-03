#include <cstdint>
#include <iostream>
#include <vector>
#include <queue>
#include <tuple>

using namespace std;

// Valor de infinito para inicialização de distâncias de 64 bits
const long long INF = 1e18;

// Deslocamentos para células vizinhas (Cima, Baixo, Esquerda, Direita)
const int dr[] = {-1, 1, 0, 0};
const int dc[] = {0, 0, -1, 1};

int H, W;

// Mapeia coordenada 2D (linha r, coluna c) para um índice 1D
inline int to_id(int r, int c) {
    return r * W + c;
}

// Algoritmo Guloso (Dijkstra) para responder a uma consulta
long long dijkstra_guloso(int r1, int c1, int r2, int c2, const vector<long long>& grid_cost) {
    // Caso de origem e destino iguais: o custo é o custo da própria célula
    if (r1 == r2 && c1 == c2) {
        return grid_cost[to_id(r1, c1)];
    }

    int start_id = to_id(r1, c1);
    int target_id = to_id(r2, c2);
    int total_nodes = H * W;

    vector<long long> dist(total_nodes, INF);

    // Fila de prioridades mínima (Min-Heap): armazena pares <distancia_acumulada, id_no>
    priority_queue<pair<long long, int>, 
                   vector<pair<long long, int>>, 
                   greater<pair<long long, int>>> pq;

    dist[start_id] = grid_cost[start_id];
    pq.push({dist[start_id], start_id});

    while (!pq.empty()) {
        auto [d, u] = pq.top();
        pq.pop();

        if (d > dist[u]) continue;
        if (u == target_id) return d; // Parada precoce ao atingir o destino

        int r = u / W;
        int c = u % W;

        for (int i = 0; i < 4; ++i) {
            int nr = r + dr[i];
            int nc = c + dc[i];

            // Verifica os limites da grade (0 <= nr < H e 0 <= nc < W)
            if (nr >= 0 && nr < H && nc >= 0 && nc < W) {
                int v = to_id(nr, nc);
                long long weight = grid_cost[v];

                if (dist[u] + weight < dist[v]) {
                    dist[v] = dist[u] + weight;
                    pq.push({dist[v], v});
                }
            }
        }
    }

    return dist[target_id];
}

int main() {
    // Otimização de E/S em C++ para suportar até 50.000 consultas rapidamente
    ios::sync_with_stdio(false);
    cin.tie(NULL);

    if (!(cin >> H >> W)) return 0;

    vector<long long> grid_cost(H * W);
    for (int i = 0; i < H; ++i) {
        for (int j = 0; j < W; ++j) {
            cin >> grid_cost[to_id(i, j)];
        }
    }

    int Q;
    if (!(cin >> Q)) return 0;

    while (Q--) {
        int r1, c1, r2, c2;
        cin >> r1 >> c1 >> r2 >> c2;
        
        // Converte coordenadas 1-indexadas da entrada para 0-indexadas
        r1--; c1--; r2--; c2--;

        cout << dijkstra_guloso(r1, c1, r2, c2, grid_cost) << "\n";
    }

    return 0;
}