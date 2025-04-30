#include <algorithm>
#include <iostream>
#include <queue>
#include <vector>

const long long cInf = 1e18;

struct Edge {
  int from;
  int to;
  long long capacity;
  long long flow;
  int index;
};

class MaxFlow {
 public:
  explicit MaxFlow(int vertex_count)
      : vertex_count_(vertex_count),
        adjacency_list_(vertex_count),
        level_(vertex_count),
        iter_(vertex_count) {}

  void AddEdge(int from, int to, long long capacity, int index) {
    Edge forward_edge = {from, to, capacity, 0, index};
    Edge reverse_edge = {to, from, 0, 0, index};
    adjacency_list_[from].push_back(edges_.size());
    edges_.push_back(forward_edge);
    adjacency_list_[to].push_back(edges_.size());
    edges_.push_back(reverse_edge);
  }

  long long ComputeMaxFlow(int source, int sink) {
    long long max_flow = 0;
    while (true) {
      long long flow = FindAugmentingPath(source, sink);
      if (flow == 0) {
        break;
      }
      max_flow += flow;
    }
    return max_flow;
  }

  std::vector<int> GetMinCutEdges(int source) {
    std::vector<bool> visited(vertex_count_, false);
    std::queue<int> queue;
    queue.push(source);
    visited[source] = true;
    while (!queue.empty()) {
      int current_vertex = queue.front();
      queue.pop();
      for (int edge_idx : adjacency_list_[current_vertex]) {
        const Edge& edge = edges_[edge_idx];
        if (edge.capacity - edge.flow > 0 && !visited[edge.to]) {
          visited[edge.to] = true;
          queue.push(edge.to);
        }
      }
    }
    std::vector<int> cut_edges;
    for (const Edge& edge : edges_) {
      if (visited[edge.from] && !visited[edge.to] && edge.capacity > 0) {
        cut_edges.push_back(edge.index);
      }
    }
    return cut_edges;
  }

 private:
  long long FindAugmentingPath(int source, int sink) {
    std::fill(level_.begin(), level_.end(), -1);
    level_[source] = 0;
    std::queue<int> queue;
    queue.push(source);
    while (!queue.empty()) {
      int current_vertex = queue.front();
      queue.pop();
      for (int edge_idx : adjacency_list_[current_vertex]) {
        const Edge& edge = edges_[edge_idx];
        if (edge.capacity - edge.flow > 0 && level_[edge.to] == -1) {
          level_[edge.to] = level_[current_vertex] + 1;
          queue.push(edge.to);
        }
      }
    }
    if (level_[sink] == -1) {
      return 0;
    }
    std::fill(iter_.begin(), iter_.end(), 0);
    long long flow = 0;
    while (true) {
      long long delta_flow = Dfs(source, sink, cInf);
      if (delta_flow == 0) {
        break;
      }
      flow += delta_flow;
    }
    return flow;
  }

  long long Dfs(int current_vertex, int sink, long long min_capacity) {
    if (current_vertex == sink) {
      return min_capacity;
    }
    for (int& i = iter_[current_vertex];
         i < int(adjacency_list_[current_vertex].size()); ++i) {
      int edge_idx = adjacency_list_[current_vertex][i];
      Edge& edge = edges_[edge_idx];
      if (edge.capacity - edge.flow > 0 &&
          level_[edge.to] == level_[current_vertex] + 1) {
        long long delta_flow = Dfs(
            edge.to, sink, std::min(min_capacity, edge.capacity - edge.flow));
        if (delta_flow > 0) {
          edge.flow += delta_flow;
          edges_[edge_idx ^ 1].flow -= delta_flow;
          return delta_flow;
        }
      }
    }
    return 0;
  }

  int vertex_count_;
  std::vector<std::vector<int>> adjacency_list_;
  std::vector<Edge> edges_;
  std::vector<int> level_;
  std::vector<int> iter_;
};

int main() {
  std::ios_base::sync_with_stdio(false);
  std::cin.tie(nullptr);
  std::cout.tie(nullptr);

  int vertex_count;
  int edge_count;
  std::cin >> vertex_count >> edge_count;

  MaxFlow flow(vertex_count);

  for (int i = 0; i < edge_count; ++i) {
    int u;
    int v;
    long long capacity;
    std::cin >> u >> v >> capacity;
    --u;
    --v;
    flow.AddEdge(u, v, capacity, i);
    flow.AddEdge(v, u, capacity, i);
  }

  int source = 0;
  int sink = vertex_count - 1;
  long long min_cut_weight = flow.ComputeMaxFlow(source, sink);
  std::vector<int> min_cut_edges = flow.GetMinCutEdges(source);

  std::sort(min_cut_edges.begin(), min_cut_edges.end());
  auto last = std::unique(min_cut_edges.begin(), min_cut_edges.end());
  min_cut_edges.erase(last, min_cut_edges.end());

  std::cout << min_cut_edges.size() << " " << min_cut_weight << "\n";
  for (int idx : min_cut_edges) {
    std::cout << idx + 1 << " ";
  }
  std::cout << "\n";

  return 0;
}
