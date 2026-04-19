#include <bits/stdc++.h>
using namespace std;

int W, H, L;
vector<string> grid;
vector<tuple<int,int,int,int>> lasers; // r, c, dr, dc
vector<pair<int,int>> targets;

static inline int dirIndex(char c) {
    switch (c) {
        case 'A': return 0; // north
        case 'V': return 1; // south
        case '<': return 2; // west
        case '>': return 3; // east
    }
    return -1;
}
static const int DR[4] = {-1, 1, 0, 0};
static const int DC[4] = { 0, 0,-1, 1};

static void trace(vector<pair<int,int>>& empty, vector<vector<char>>& lit,
                  vector<vector<char>>& seenEmpty) {
    for (auto& [lr, lc, dr0, dc0] : lasers) {
        int dr = dr0, dc = dc0;
        int r = lr + dr, c = lc + dc;
        vector<uint8_t> seen((size_t)H * W * 4, 0);
        while (r >= 0 && r < H && c >= 0 && c < W) {
            int kier = 0;
            if (dr == -1) kier = 0;
            else if (dr == 1) kier = 1;
            else if (dc == -1) kier = 2;
            else kier = 3;
            size_t idx = ((size_t)r * W + c) * 4 + kier;
            if (seen[idx]) break;
            seen[idx] = 1;

            char ch = grid[r][c];
            if (ch == '#') break;
            if (ch == 'O') {
                lit[r][c] = 1;
            } else if (ch == '/') {
                int ndr = -dc, ndc = -dr;
                dr = ndr; dc = ndc;
            } else if (ch == '\\') {
                int ndr = dc, ndc = dr;
                dr = ndr; dc = ndc;
            } else if (ch == '.') {
                if (!seenEmpty[r][c]) {
                    seenEmpty[r][c] = 1;
                    empty.push_back({r, c});
                }
            }
            r += dr;
            c += dc;
        }
    }
}

static bool backtrack(int left) {
    vector<vector<char>> lit(H, vector<char>(W, 0));
    vector<vector<char>> seenEmpty(H, vector<char>(W, 0));
    vector<pair<int,int>> empty;
    trace(empty, lit, seenEmpty);

    bool ok = true;
    for (auto [r, c] : targets) if (!lit[r][c]) { ok = false; break; }
    if (ok) return true;
    if (left == 0) return false;

    for (auto [r, c] : empty) {
        if (grid[r][c] != '.') continue;
        for (char m : {'/', '\\'}) {
            grid[r][c] = m;
            if (backtrack(left - 1)) return true;
            grid[r][c] = '.';
        }
    }
    return false;
}

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);

    if (!(cin >> W >> H >> L)) return 0;
    cin.ignore();

    grid.assign(H, string(W, '.'));
    for (int i = 0; i < H; ++i) {
        string line;
        if (!getline(cin, line)) line = "";
        if ((int)line.size() < W) line.append(W - line.size(), '.');
        for (int j = 0; j < W; ++j) grid[i][j] = line[j];
    }

    for (int r = 0; r < H; ++r) {
        for (int c = 0; c < W; ++c) {
            int di = dirIndex(grid[r][c]);
            if (di >= 0) lasers.push_back({r, c, DR[di], DC[di]});
            else if (grid[r][c] == 'O') targets.push_back({r, c});
        }
    }

    backtrack(L);

    cout << W << ' ' << H << ' ' << L << '\n';
    for (auto& row : grid) cout << row << '\n';
    return 0;
}
