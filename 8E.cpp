#include <algorithm>
#include <iostream>
#include <vector>

struct Edge {
  int from;
  int to;
  int weight;
};
bool Findnegative(int n, const std::vector<Edge>& edges,
                  std::vector<int>& cycle) {
  const int cInf = 1e9;
  std::vector<int> distances(n + 1, cInf);
  std::vector<int> parent(n + 1, -1);
  distances[0] = 0;
  for (int i = 0; i < n; ++i) {
    bool updated = false;
    for (const Edge& edge : edges) {
      if (distances[edge.from] < cInf &&
          distances[edge.to] > distances[edge.from] + edge.weight) {
        distances[edge.to] = distances[edge.from] + edge.weight;
        parent[edge.to] = edge.from;
        updated = true;
      }
    }
    if (!updated) {
      break;
    }
  }
  for (const Edge& edge : edges) {
    if (distances[edge.from] < cInf &&
        distances[edge.to] > distances[edge.from] + edge.weight) {
      int v = edge.to;
      for (int i = 0; i < n; ++i) {
        v = parent[v];
      }
      std::vector<int> local_cycle;
      int u = v;
      do {
        local_cycle.push_back(u);
        u = parent[u];
      } while (u != v);
      local_cycle.push_back(v);
      std::reverse(local_cycle.begin(), local_cycle.end());
      cycle = local_cycle;
      return true;
    }
  }

  return false;
}

int main() {
  std::ios_base::sync_with_stdio(false);
  std::cin.tie(nullptr);
  std::cout.tie(nullptr);
  int n;
  std::cin >> n;
  const int cConst = 100000;
  std::vector<Edge> edges;
  for (int i = 1; i <= n; ++i) {
    edges.push_back({0, i, 0});
  }
  for (int i = 1; i <= n; ++i) {
    for (int j = 1; j <= n; ++j) {
      int weight;
      std::cin >> weight;
      if (weight != cConst) {
        edges.push_back({i, j, weight});
      }
    }
  }
  std::vector<int> cycle;
  bool has_cycle = Findnegative(n, edges, cycle);
  if (static_cast<int>(has_cycle) == 0) {
    std::cout << "NO\n";
    return 0;
  }
  std::cout << "YES\n";
  std::cout << cycle.size() << "\n";
  for (size_t i = 0; i < cycle.size(); ++i) {
    if (i > 0) {
      std::cout << " ";
    }
    std::cout << cycle[i];
  }
  std::cout << "\n";
  return 0;
}
