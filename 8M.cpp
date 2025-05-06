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
  int vertex_count = graph.size();
  std::vector<int> dist(vertex_count, cInf);
  std::vector<int> parent(vertex_count, -1);
  std::vector<bool> in_mst(vertex_count, false);
  long long total_weight = 0;
  dist[start] = 0;
  
  using PII = std::pair<int, int>;
  std::priority_queue<PII, std::vector<PII>, std::greater<>> pq;
  pq.push({0, start});
  
  while (!pq.empty()) {
    int current = pq.top().second;
    pq.pop();
    
    if (in_mst[current]) {
      continue;
    }
    
    in_mst[current] = true;
    if (parent[current] != -1) {
      total_weight += dist[current];
    }
    
    for (const Edge& edge : graph[current]) {
      int neighbor = edge.to;
      int cost = edge.weight;
      
      if (!in_mst[neighbor] && cost < dist[neighbor]) {
        dist[neighbor] = cost;
        parent[neighbor] = current;
        pq.push({dist[neighbor], neighbor});
      }
    }
  }
  return total_weight;
}

int main() {
  std::ios_base::sync_with_stdio(false);
  std::cin.tie(0);
  std::cout.tie(0);
  
  int vertex_count;
  int edge_count;
  std::cin >> vertex_count >> edge_count;
  
  Graph graph(vertex_count);
  for (int i = 0; i < edge_count; ++i) {
    int from;
    int to;
    int weight;
    std::cin >> from >> to >> weight;
    
    --from;
    --to;
    
    graph[from].push_back({to, weight});
    graph[to].push_back({from, weight});
  }
  
  long long mst_weight = Prim(graph, 0);
  std::cout << mst_weight << '\n';
  
  return 0;
}
