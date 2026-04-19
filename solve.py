import sys

sys.setrecursionlimit(10000)

DIR = {'A': (-1, 0), 'V': (1, 0), '<': (0, -1), '>': (0, 1)}
def trace(grid, H, W, lasers):
    lit = set()
    empty = []
    seen_empty = set()
    for lr, lc, ld in lasers:
        dr, dc = DIR[ld]
        r, c = lr + dr, lc + dc
        states = set()
        while 0 <= r < H and 0 <= c < W:
            st = (r, c, dr, dc)
            if st in states: break
            states.add(st)
            ch = grid[r][c]
            if ch == '#': break
            if ch == 'O': lit.add((r, c))
            elif ch == '/': dr, dc = -dc, -dr
            elif ch == '\\': dr, dc = dc, dr
            elif ch == '.':
                if (r, c) not in seen_empty:
                    seen_empty.add((r, c))
                    empty.append((r, c))
            r += dr
            c += dc
    return lit, empty


def solve(rows: int, cols: int, mirrors: int, grid, lasers, targets):
    H, W = rows, cols
    def backtrack(left):
        lit, empty = trace(grid, H, W, lasers)
        if targets.issubset(lit): return True
        if left == 0: return False
        for (r, c) in empty:
            if grid[r][c] == '.':
                for m in ('/', '\\'):
                    grid[r][c] = m
                    if backtrack(left - 1): return True
                    grid[r][c] = '.'
        return False
    backtrack(mirrors)
    return grid


def runner():
    if len(sys.argv) >= 3:
        with open(sys.argv[1], 'r') as f: data = f.read()
        out = open(sys.argv[2], 'w')
    else:
        data = sys.stdin.read()
        out = sys.stdout

    lines = data.splitlines()
    W, H, L = (int(x) for x in lines[0].split())
    grid = []
    for i in range(H):
        row = lines[i + 1]
        if len(row) < W: row = row + '.' * (W - len(row))
        grid.append(list(row[:W]))
    lasers = []
    targets = set()
    for r in range(H):
        for c in range(W):
            ch = grid[r][c]
            if ch in DIR: lasers.append((r, c, ch))
            elif ch == 'O': targets.add((r, c))
    solve(H, W, L, grid, lasers, targets)
    out.write(f"{W} {H} {L}\n")
    for row in grid: out.write(''.join(row) + '\n')
    if out is not sys.stdout: out.close()

if __name__ == '__main__': runner()