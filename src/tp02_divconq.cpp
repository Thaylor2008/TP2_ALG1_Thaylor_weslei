#include <cstdint>
#include <iostream>
#include <vector>
#include <queue>
#include <tuple>
#include <algorithm>

using namespace std;

// Valor de infinito para inicialização de custos (64 bits)
const long long INF = 1e18;

// Deslocamentos para células vizinhas (Cima, Baixo, Esquerda, Direita)
const int dr[] = {-1, 1, 0, 0};
const int dc[] = {0, 0, -1, 1};

int H, W;
vector<vector<long long>> grid_cost;

// Estrutura para armazenar uma consulta
struct Query {
    int r1, c1, r2, c2;
    int id;
};

// Mapeamento 2D para índice 1D no subgrafo das colunas [L, R]
inline int get_sub_id(int r, int c, int L, int width) {
    return r * width + (c - L);
}

// Respostas finais para as Q consultas
vector<long long> ans;

// Executa o Dijkstra no subgrafo restrito às colunas [L, R] a partir de (src_r, src_c)
vector<long long> dijkstra_subgrid(int L, int R, int src_r, int src_c) {
    int width = R - L + 1;
    int total_nodes = H * width;
    vector<long long> dist(total_nodes, INF);

    int start_id = get_sub_id(src_r, src_c, L, width);
    dist[start_id] = grid_cost[src_r][src_c];

    // Priority queue minima: <distancia_acumulada, r, c>
    priority_queue<tuple<long long, int, int>,
                   vector<tuple<long long, int, int>>,
                   greater<tuple<long long, int, int>>> pq;

    pq.push({dist[start_id], src_r, src_c});

    while (!pq.empty()) {
        auto [d, r, c] = pq.top();
        pq.pop();

        int u_id = get_sub_id(r, c, L, width);
        if (d > dist[u_id]) continue;

        for (int i = 0; i < 4; ++i) {
            int nr = r + dr[i];
            int nc = c + dc[i];

            // Restrição: a busca não pode sair do intervalo de colunas [L, R]
            if (nr >= 0 && nr < H && nc >= L && nc <= R) {
                int v_id = get_sub_id(nr, nc, L, width);
                long long weight = grid_cost[nr][nc];

                if (d + weight < dist[v_id]) {
                    dist[v_id] = d + weight;
                    pq.push({dist[v_id], nr, nc});
                }
            }
        }
    }

    return dist;
}

// Procedimento de Divisão e Conquista
void divide_and_conquer(int L, int R, const vector<Query>& active_queries) {
    if (active_queries.empty()) return;

    int mid = (L + R) / 2;
    int width = R - L + 1;

    // Executa H Dijkstras a partir de cada célula da coluna mid
    vector<vector<long long>> dist_from_mid(H);
    for (int k = 0; k < H; ++k) {
        dist_from_mid[k] = dijkstra_subgrid(L, R, k, mid);
    }

    vector<Query> left_queries, right_queries;

    for (const auto& q : active_queries) {
        int min_c = min(q.c1, q.c2);
        int max_c = max(q.c1, q.c2);

        // Se o caminho entre c1 e c2 atravessa a coluna mid
        if (min_c <= mid && mid <= max_c) {
            int u1 = get_sub_id(q.r1, q.c1, L, width);
            int u2 = get_sub_id(q.r2, q.c2, L, width);

            long long best = INF;
            for (int k = 0; k < H; ++k) {
                long long d1 = dist_from_mid[k][u1];
                long long d2 = dist_from_mid[k][u2];
                long long total_cost = d1 + d2 - grid_cost[k][mid];
                if (total_cost < best) {
                    best = total_cost;
                }
            }
            ans[q.id] = best;
        } else if (max_c < mid) {
            left_queries.push_back(q);
        } else { // min_c > mid
            right_queries.push_back(q);
        }
    }

    if (L == R) return;

    // Chamadas recursivas para as metades esquerda e direita
    if (mid - 1 >= L) divide_and_conquer(L, mid - 1, left_queries);
    if (mid + 1 <= R) divide_and_conquer(mid + 1, R, right_queries);
}

int main() {
    // Otimização de Entrada e Saída
    ios::sync_with_stdio(false);
    cin.tie(NULL);

    if (!(cin >> H >> W)) return 0;

    grid_cost.assign(H, vector<long long>(W));
    for (int i = 0; i < H; ++i) {
        for (int j = 0; j < W; ++j) {
            cin >> grid_cost[i][j];
        }
    }

    int Q;
    if (!(cin >> Q)) return 0;

    ans.resize(Q);
    vector<Query> queries(Q);

    for (int i = 0; i < Q; ++i) {
        cin >> queries[i].r1 >> queries[i].c1 >> queries[i].r2 >> queries[i].c2;
        // Converte coordenadas 1-indexadas para 0-indexadas
        queries[i].r1--; queries[i].c1--;
        queries[i].r2--; queries[i].c2--;
        queries[i].id = i;
    }

    // Inicia Divisão e Conquista no intervalo completo de colunas [0, W - 1]
    divide_and_conquer(0, W - 1, queries);

    // Imprime as respostas na ordem original das consultas
    for (int i = 0; i < Q; ++i) {
        cout << ans[i] << "\n";
    }

    return 0;
}
