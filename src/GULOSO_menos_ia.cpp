#include <iostream>
#include <vector>
#include <queue>

using namespace std;

const long long INF = 1e18;
int dr[] = {-1, 1, 0, 0};
int dc[] = {0, 0, -1, 1};

int H, W;

inline int to_id(int r, int c) {
    return r * W + c;
}

long long dijkstra(int r1, int c1, int r2, int c2, const vector<long long>& g) {
    if (r1 == r2 && c1 == c2) {
        return g[to_id(r1, c1)];
    }

    int start_id = to_id(r1, c1);
    int target_id = to_id(r2, c2);
    int total_nodes = H * W;

    vector<long long> dist(total_nodes, INF);
    using Item = pair<long long, int>;
    priority_queue<Item, vector<Item>, greater<Item>> pq;

    dist[start_id] = g[start_id];
    pq.push({dist[start_id], start_id});

    while (!pq.empty()) {
        auto top = pq.top();
        pq.pop();

        long long d = top.first;
        int u = top.second;

        if (d > dist[u]) continue;
        if (u == target_id) return d;

        int r = u / W;
        int c = u % W;

        for (int i = 0; i < 4; i++) {
            int nr = r + dr[i];
            int nc = c + dc[i];

            if (nr >= 0 && nr < H && nc >= 0 && nc < W) {
                int v = to_id(nr, nc);
                long long w = g[v];

                if (dist[u] + w < dist[v]) {
                    dist[v] = dist[u] + w;
                    pq.push({dist[v], v});
                }
            }
        }
    }

    return dist[target_id];
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(NULL);

    if (!(cin >> H >> W)) return 0;

    vector<long long> g(H * W);
    for (int i = 0; i < H; i++) {
        for (int j = 0; j < W; j++) {
            cin >> g[to_id(i, j)];
        }
    }

    int Q;
    cin >> Q;

    while (Q--) {
        int r1, c1, r2, c2;
        cin >> r1 >> c1 >> r2 >> c2;
        r1--; c1--; r2--; c2--;

        cout << dijkstra(r1, c1, r2, c2, g) << "\n";
    }

    return 0;
}
