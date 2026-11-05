#include <iostream>
#include <vector>
#include <queue>
#include <algorithm>
#include <utility>
using namespace std;

const long long INF = 1e18;

// Movimentos possiveis: cima, baixo, esquerda e direita
int dr[] = {-1, 1, 0, 0};
int dc[] = {0, 0, -1, 1};

int H, W;
vector<vector<long long>> mapa;

// Guarda os pontos de origem e destino de cada consulta
struct Query {
    int r1, c1, r2, c2, id;
};

// Transforma uma posicao da matriz em um indice de vetor
int get_id(int r, int c, int L, int width) {
    return r * width + (c - L);
}

vector<long long> ans;

// Algoritmo de Dijkstra para calcular as menores distancias a partir de uma celula
// considerando somente as colunas entre L e R
vector<long long> dijkstra(int L, int R, int sr, int sc) {

    int width = R - L + 1;
    vector<long long> dist(H * width, INF);

    int start = get_id(sr, sc, L, width);

    // O custo inicial inclui a celula de origem
    dist[start] = mapa[sr][sc];

    // Fila de prioridade: menor distancia primeiro
    priority_queue<
        pair<long long, pair<int, int>>,
        vector<pair<long long, pair<int, int>>>,
        greater<pair<long long, pair<int, int>>>
    > fila;

    fila.push({dist[start], {sr, sc}});

    while (!fila.empty()) {

        auto top = fila.top();
        fila.pop();

        long long d = top.first;
        int r = top.second.first;
        int c = top.second.second;

        int u = get_id(r, c, L, width);

        // Ignora distancias que ja foram atualizadas
        if (d > dist[u]) continue;

        // Verifica os quatro vizinhos
        for (int i = 0; i < 4; i++) {

            int nr = r + dr[i];
            int nc = c + dc[i];

            // Verifica se a posicao esta dentro da regiao
            if (nr >= 0 && nr < H && nc >= L && nc <= R) {

                int v = get_id(nr, nc, L, width);
                long long w = mapa[nr][nc];

                // Atualiza caso encontre um caminho menor
                if (d + w < dist[v]) {
                    dist[v] = d + w;
                    fila.push({dist[v], {nr, nc}});
                }
            }
        }
    }

    return dist;
}

// Divide as consultas utilizando a coluna do meio
void solve(int L, int R, const vector<Query>& q_list) {

    if (q_list.empty()) return;

    int meio = (L + R) / 2;
    int width = R - L + 1;

    // Calcula Dijkstra a partir de cada linha da coluna central
    vector<vector<long long>> d_mid(H);

    for (int i = 0; i < H; i++) {
        d_mid[i] = dijkstra(L, R, i, meio);
    }

    vector<Query> consultasEsquerda;
    vector<Query> consultasDireita;

    for (const auto& q : q_list) {

        int u1 = get_id(q.r1, q.c1, L, width);
        int u2 = get_id(q.r2, q.c2, L, width);

        long long melhor = INF;

        // Testa qual celula da coluna central serve de melhor ponte na faixa [L, R]
        for (int k = 0; k < H; k++) {
            long long d1 = d_mid[k][u1];
            long long d2 = d_mid[k][u2];

            if (d1 != INF && d2 != INF) {
                long long custo = d1 + d2 - mapa[k][meio];
                melhor = min(melhor, custo);
            }
        }

        // Atualiza a resposta global da consulta com o menor caminho que cruza a coluna meio
        ans[q.id] = min(ans[q.id], melhor);

        // Se a consulta esta inteiramente à esquerda do meio, passa para o subproblema esquerdo
        if (q.c1 < meio && q.c2 < meio) {
            consultasEsquerda.push_back(q);
        } 
        // Se a consulta esta inteiramente à direita do meio, passa para o subproblema direito
        else if (q.c1 > meio && q.c2 > meio) {
            consultasDireita.push_back(q);
        }
    }

    // Caso-base: resta apenas uma coluna
    if (L == R) return;

    // Resolve os dois subproblemas recursivamente
    if (meio - 1 >= L) {
        solve(L, meio - 1, consultasEsquerda);
    }

    if (meio + 1 <= R) {
        solve(meio + 1, R, consultasDireita);
    }
}

int main() {

    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    if (!(cin >> H >> W)) return 0;

    // Leitura dos custos da grade
    mapa.assign(H, vector<long long>(W));

    for (int i = 0; i < H; i++) {
        for (int j = 0; j < W; j++) {
            cin >> mapa[i][j];
        }
    }

    int Q;
    cin >> Q;

    // Inicializa as respostas com INF
    ans.assign(Q, INF);

    vector<Query> queries(Q);

    // Leitura das consultas
    for (int i = 0; i < Q; i++) {

        cin >> queries[i].r1 >> queries[i].c1
            >> queries[i].r2 >> queries[i].c2;

        // Converte as coordenadas para indices iniciados em zero
        queries[i].r1--;
        queries[i].c1--;
        queries[i].r2--;
        queries[i].c2--;

        queries[i].id = i;
    }

    // Inicia a divisao e conquista
    solve(0, W - 1, queries);

    // Imprime as respostas na ordem original
    for (int i = 0; i < Q; i++) {
        cout << ans[i] << "\n";
    }

    return 0;
}