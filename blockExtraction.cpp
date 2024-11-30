#include <bits/stdc++.h>
using namespace std;

int main() {
    int x, y;
    cin >> x >> y;
    vector<vector<int>> z(x, vector<int>(y));

    for (int i = 0; i < x; ++i) {
        for (int j = 0; j < y; ++j) {
            cin >> z[i][j];
        }
    }

    int w;
    cin >> w;

    map<int, vector<pair<int, int>>> a;
    set<int> b;
    for (int i = 0; i < x; ++i) {
        for (int j = 0; j < y; ++j) {
            int c = z[i][j];
            a[c].emplace_back(i, j);
            b.insert(c);
        }
    }

    set<int> d;
    for (int i = 0; i < x; ++i) {
        vector<int> e;
        for (int j = 0; j < y; ++j) {
            if (z[i][j] == w) {
                e.push_back(j);
            }
        }
        if (!e.empty()) {
            int f = *max_element(e.begin(), e.end());
            for (int j = f + 1; j < y; ++j) {
                int g = z[i][j];
                if (g != w) {
                    d.insert(g);
                }
            }
        }
    }

    set<int> h = b;
    d.erase(w);
    int i = 0;
    for (auto j = d.begin(); j != d.end(); ++j) {
        if (h.find(*j) != h.end()) {
            h.erase(*j);
            i++;
        }
    }

    auto k = [&](const set<int>& l) -> set<int> {
        set<int> m;
        for (auto& n : l) {
            for (auto& [o, p] : a[n]) {
                if (o == x - 1) {
                    m.insert(n);
                    break;
                }
            }
        }
        queue<int> q;
        for (auto& r : m) {
            q.push(r);
        }
        while (!q.empty()) {
            int s = q.front();
            q.pop();
            for (auto& t : l) {
                if (m.find(t) != m.end()) continue;
                bool u = false;
                for (auto& [v, w] : a[t]) {
                    if (v + 1 < x) {
                        int x = z[v + 1][w];
                        if (m.find(x) != m.end()) {
                            u = true;
                            break;
                        }
                    }
                }
                if (u) {
                    m.insert(t);
                    q.push(t);
                }
            }
        }
        return m;
    };

    while (true) {
        set<int> y = k(h);
        set<int> z;
        for (auto& aa : h) {
            if (y.find(aa) == y.end()) {
                z.insert(aa);
            }
        }
        if (z.empty()) break;
        for (auto& bb : z) {
            h.erase(bb);
            i++;
        }
    }

    cout << i;

    return 0;
}
