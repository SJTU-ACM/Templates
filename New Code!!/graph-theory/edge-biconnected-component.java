/* 
 * Edge Bi-connected Component
 * Check: hihoCoder 1184
 */

static int n, m;
static int[] head = new int[MAXN], nxt = new int[MAXM << 1], to = new int[MAXM << 1];
static int ed;
// Opposite edge exists, set head[] to -1.

static void AddEdge(int u, int v) {
    nxt[ed] = head[u]; head[u] = ed; to[ed++] = v;
    nxt[ed] = head[v]; head[v] = ed; to[ed++] = u;
}

static class EBCC {
    static int[] dfn = new int[MAXN], low = new int[MAXN], bccIdx = new int[MAXN];
    static int bccCnt, stamp;
    static boolean[] isBridge = new boolean[MAXM << 1], vis = new boolean[MAXM << 1];

    static void Tarjan(int now) {
        dfn[now] = low[now] = ++stamp;
        for (int i = head[now]; i != -1; i = nxt[i]) {
            if (dfn[to[i]] == 0) {
                vis[i] = vis[i ^ 1] = true;
                Tarjan(to[i]);
                low[now] = Math.min(low[now], low[to[i]]);
                if (low[to[i]] > dfn[now])
                    isBridge[i] = isBridge[i ^ 1] = true;
            } else if (dfn[to[i]] < dfn[now] && !vis[i]) {
                vis[i] = vis[i ^ 1] = true;
                low[now] = Math.min(low[now], dfn[to[i]]);
            }
        }
    }

    static void DFS(int now) {
        bccIdx[now] = bccCnt;
        for (int i = head[now]; i != -1; i = nxt[i]) {
            if (isBridge[i]) continue;
            if (bccIdx[to[i]] == 0) DFS(to[i]);
        }
    }

    static void Solve() {
        Arrays.fill(dfn, 0);
        Arrays.fill(low, 0);
        Arrays.fill(isBridge, false);
        Arrays.fill(bccIdx, 0);
        bccCnt = stamp = 0;

        for (int i = 1; i <= n; ++i)
            if (dfn[i] == 0) Tarjan(i);

        for (int i = 1; i <= n; ++i)
            if (bccIdx[i] == 0) {
                ++bccCnt;
                DFS(i);
            }
    }
}