#include <algorithm>
#include <climits>
#include <iostream>
#include <queue>
#include <vector>

const long long cInf = LLONG_MAX;

struct Edge {
  int to;
  long long capacity;
  long long cost;
  int rev;
};

class FlowNetwork {
 public:
  explicit FlowNetwork(int vertex_count)
      : vertex_count_(vertex_count), graph_(vertex_count) {}

  void AddEdge(int from, int to, long long capacity, long long cost) {
    Edge forward = {to, capacity, cost, static_cast<int>(graph_[to].size())};
    Edge reverse = {from, 0, -cost, static_cast<int>(graph_[from].size())};
    graph_[from].push_back(forward);
    graph_[to].push_back(reverse);
  }

  long long ComputeMinCostMaxFlow(int source, int sink) {
    long long total_cost = 0;
    while (true) {
      std::vector<long long> dist(vertex_count_, cInf);
      std::vector<int> prev_vertex(vertex_count_, -1);
      std::vector<int> prev_edge(vertex_count_, -1);
      if (!BellmanFord(source, dist, prev_vertex, prev_edge)) {
        break;
      }
      long long flow = cInf;
      for (int v = sink; v != source; v = prev_vertex[v]) {
        int u = prev_vertex[v];
        int edge_idx = prev_edge[v];
        flow = std::min(flow, graph_[u][edge_idx].capacity);
      }
      for (int v = sink; v != source; v = prev_vertex[v]) {
        int u = prev_vertex[v];
        int edge_idx = prev_edge[v];
        graph_[u][edge_idx].capacity -= flow;
        graph_[v][graph_[u][edge_idx].rev].capacity += flow;
        total_cost += flow * graph_[u][edge_idx].cost;
      }
    }
    return total_cost;
  }

 private:
  bool BellmanFord(int source, std::vector<long long>& dist,
                   std::vector<int>& prev_vertex, std::vector<int>& prev_edge) {
    dist[source] = 0;
    for (int i = 0; i < vertex_count_ - 1; ++i) {
      bool updated = false;
      for (int u = 0; u < vertex_count_; ++u) {
        if (dist[u] == cInf) {
          continue;
        }
        for (int j = 0; j < int(graph_[u].size()); ++j) {
          const Edge& e = graph_[u][j];
          if (e.capacity > 0 && dist[u] + e.cost < dist[e.to]) {
            dist[e.to] = dist[u] + e.cost;
            prev_vertex[e.to] = u;
            prev_edge[e.to] = j;
            updated = true;
          }
        }
      }
      if (!updated) {
        break;
      }
    }
    return dist[vertex_count_ - 1] != cInf;
  }

  int vertex_count_;
  std::vector<std::vector<Edge>> graph_;
};

int main() {
  std::ios_base::sync_with_stdio(false);
  std::cin.tie(nullptr);
  std::cout.tie(nullptr);

  int vertex_count;
  int edge_count;
  std::cin >> vertex_count >> edge_count;

  FlowNetwork network(vertex_count);

  for (int i = 0; i < edge_count; ++i) {
    int u;
    int v;
    long long capacity;
    long long cost;
    std::cin >> u >> v >> capacity >> cost;
    --u;
    --v;
    network.AddEdge(u, v, capacity, cost);
  }

  int source = 0;
  int sink = vertex_count - 1;
  long long min_cost = network.ComputeMinCostMaxFlow(source, sink);

  std::cout << min_cost << "\n";

  return 0;
}
