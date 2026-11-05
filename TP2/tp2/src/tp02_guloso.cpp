#include <iostream>
#include <vector>
#include <queue>
#include <utility>

using namespace std;

const long long INF = 1e18;

// Movimentos possiveis: cima, baixo, esquerda e direita
int dr[] = {-1, 1, 0, 0};
int dc[] = {0, 0, -1, 1};

int H, W;

// Transforma coordenadas da matriz em um indice linear do vetor
inline int get_id(int r, int c) {
    return r * W + c;
}

// Algoritmo de Dijkstra direto na grade para calcular o menor caminho entre duas celulas
long long dijkstra(int r1, int c1, int r2, int c2, const vector<long long>& mapa) {

    // Caso de origem e destino na mesma celula
    if (r1 == r2 && c1 == c2) {
        return mapa[get_id(r1, c1)];
    }

    int inicio = get_id(r1, c1);
    int destino = get_id(r2, c2);
    int total_celulas = H * W;

    vector<long long> dist(total_celulas, INF);

    // Fila de prioridade: menor distancia acumulada primeiro
    priority_queue<
        pair<long long, int>,
        vector<pair<long long, int>>,
        greater<pair<long long, int>>
    > fila;

    // O custo inicial inclui o peso da celula de origem
    dist[inicio] = mapa[inicio];
    fila.push({dist[inicio], inicio});

    while (!fila.empty()) {

        auto top = fila.top();
        fila.pop();

        long long d = top.first;
        int u = top.second;

        // Ignora caminhos ja atualizados
        if (d > dist[u]) continue;

        // Parada antecipada ao atingir o objetivo
        if (u == destino) return d;

        int r = u / W;
        int c = u % W;

        // Explora os quatro vizinhos
        for (int i = 0; i < 4; i++) {

            int nr = r + dr[i];
            int nc = c + dc[i];

            // Garante que o vizinho esta dentro dos limites do mapa
            if (nr >= 0 && nr < H && nc >= 0 && nc < W) {

                int v = get_id(nr, nc);
                long long w = mapa[v];

                // Atualiza o caminho mais curto
                if (dist[u] + w < dist[v]) {
                    dist[v] = dist[u] + w;
                    fila.push({dist[v], v});
                }
            }
        }
    }

    return dist[destino];
}

int main() {

    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    if (!(cin >> H >> W)) return 0;

    // Leitura dos custos da grade em vetor 1D
    vector<long long> mapa(H * W);

    for (int i = 0; i < H; i++) {
        for (int j = 0; j < W; j++) {
            cin >> mapa[get_id(i, j)];
        }
    }

    int Q;
    cin >> Q;

    // Leitura e processamento de cada consulta
    while (Q--) {
        int r1, c1, r2, c2;
        cin >> r1 >> c1 >> r2 >> c2;

        // Converte coordenadas 1-indexadas da entrada para 0-indexadas
        r1--;
        c1--;
        r2--;
        c2--;

        cout << dijkstra(r1, c1, r2, c2, mapa) << "\n";
    }

    return 0;
}