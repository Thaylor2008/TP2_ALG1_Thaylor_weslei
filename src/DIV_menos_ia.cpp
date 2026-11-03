#include <bits/stdc++.h>
using namespace std;

const long long INF = 1e18;
int dr[] = {-1, 1, 0, 0};
int dc[] = {0, 0, -1, 1};

int H, W;
vector<vector<long long>> grid;

struct Query {
    int r1, c1, r2, c2, id;
};

inline int get_id(int r, int c, int L, int width) {
    return r * width + (c - L);
}

vector<long long> ans;

vector<long long> dijkstra(int L, int R, int sr, int sc) {
    int width = R - L + 1;
    vector<long long> dist(H * width, INF);

    int start = get_id(sr, sc, L, width);
    dist[start] = grid[sr][sc];

    using Item = pair<long long, pair<int, int>>;
    priority_queue<Item, vector<Item>, greater<Item>> pq;
    pq.push({dist[start], {sr, sc}});

    while (!pq.empty()) {
        auto top = pq.top();
        pq.pop();
        
        long long d = top.first;
        int r = top.second.first;
        int c = top.second.second;

        int u = get_id(r, c, L, width);
        if (d > dist[u]) continue;

        for (int i = 0; i < 4; i++) {
            int nr = r + dr[i];
            int nc = c + dc[i];

            if (nr >= 0 && nr < H && nc >= L && nc <= R) {
                int v = get_id(nr, nc, L, width);
                long long w = grid[nr][nc];

                if (d + w < dist[v]) {
                    dist[v] = d + w;
                    pq.push({dist[v], {nr, nc}});
                }
            }
        }
    }

    return dist;
}

void solve(int L, int R, const vector<Query>& q_list) {
    if (q_list.empty()) return;

    int mid = (L + R) / 2;
    int width = R - L + 1;

    vector<vector<long long>> d_mid(H);
    for (int i = 0; i < H; i++) {
        d_mid[i] = dijkstra(L, R, i, mid);
    }

    vector<Query> q_left, q_right;

    for (const auto& q : q_list) {
        int min_c = min(q.c1, q.c2);
        int max_c = max(q.c1, q.c2);

        if (min_c <= mid && mid <= max_c) {
            int u1 = get_id(q.r1, q.c1, L, width);
            int u2 = get_id(q.r2, q.c2, L, width);

            long long best = INF;
            for (int k = 0; k < H; k++) {
                long long cost = d_mid[k][u1] + d_mid[k][u2] - grid[k][mid];
                best = min(best, cost);
            }
            ans[q.id] = best;
        } else if (max_c < mid) {
            q_left.push_back(q);
        } else {
            q_right.push_back(q);
        }
    }

    if (L == R) return;

    if (mid - 1 >= L) solve(L, mid - 1, q_left);
    if (mid + 1 <= R) solve(mid + 1, R, q_right);
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    if (!(cin >> H >> W)) return 0;

    grid.assign(H, vector<long long>(W));
    for (int i = 0; i < H; i++) {
        for (int j = 0; j < W; j++) {
            cin >> grid[i][j];
        }
    }

    int Q;
    cin >> Q;

    ans.resize(Q);
    vector<Query> queries(Q);

    for (int i = 0; i < Q; i++) {
        cin >> queries[i].r1 >> queries[i].c1 >> queries[i].r2 >> queries[i].c2;
        queries[i].r1--; queries[i].c1--;
        queries[i].r2--; queries[i].c2--;
        queries[i].id = i;
    }

    solve(0, W - 1, queries);

    for (int i = 0; i < Q; i++) {
        cout << ans[i] << "\n";
    }

    return 0;
}
