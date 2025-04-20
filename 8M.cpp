#include <iostream>
#include <queue>
#include <vector>

struct Edge {
  int to;
  int weight;
};
using Graph = std::vector<std::vector<Edge>>;
const int cInf = 1e9;
long long Prim(const Graph& graph, int start) {
  int n = graph.size();
  std::vector<int> dist(n, cInf);
  std::vector<int> parent(n, -1);
  std::vector<bool> in_mst(n, false);
  long long total_weight = 0;
  dist[start] = 0;
  using PII = std::pair<int, int>;
  std::priority_queue<PII, std::vector<PII>, std::greater<PII>> pq;
  pq.push({0, start});
  while (!pq.empty()) {
    int v = pq.top().second;
    pq.pop();
    if (in_mst[v]) {
      continue;
    }
    in_mst[v] = true;
    if (parent[v] != -1) {
      total_weight += dist[v];
    }
    for (const Edge& edge : graph[v]) {
      int u = edge.to;
      int weight = edge.weight;
      if (!in_mst[u] && weight < dist[u]) {
        dist[u] = weight;
        parent[u] = v;
        pq.push({dist[u], u});
      }
    }
  }
  return total_weight;
}

int main() {
  std::ios_base::sync_with_stdio(false);
  std::cin.tie(0);
  std::cout.tie(0);
  int n;
  int m;
  std::cin >> n >> m;
  Graph graph(n);
  for (int i = 0; i < m; ++i) {
    int u;
    int v;
    int c;
    std::cin >> u >> v >> c;
    --u;
    --v;
    graph[u].push_back({v, c});
    graph[v].push_back({u, c});
  }
  long long mst_weight = Prim(graph, 0);
  std::cout << mst_weight << '\n';
  return 0;
}
