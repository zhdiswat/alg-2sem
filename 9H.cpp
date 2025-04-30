#include <algorithm>
#include <iostream>
#include <queue>
#include <vector>

const long long cInf = 1e18;

struct Edge {
  int to;
  long long capacity;
  long long flow;
  int reverse_index;
};

std::vector<Edge> all_edges;
std::vector<std::vector<int>> graph;
std::vector<int> distances;
std::vector<int> edge_pointer;

bool RunBfs(int source, int sink) {
  std::fill(distances.begin(), distances.end(), -1);
  distances[source] = 0;

  std::queue<int> vertex_queue;
  vertex_queue.push(source);

  while (!vertex_queue.empty()) {
    int current_vertex = vertex_queue.front();
    vertex_queue.pop();

    for (int edge_index : graph[current_vertex]) {
      const Edge& edge = all_edges[edge_index];
      if (edge.flow < edge.capacity && distances[edge.to] == -1) {
        distances[edge.to] = distances[current_vertex] + 1;
        vertex_queue.push(edge.to);
      }
    }
  }
  return distances[sink] != -1;
}

long long RunDfs(int current_vertex, int sink, long long min_capacity) {
  if (current_vertex == sink || min_capacity == 0) {
    return min_capacity;
  }

  for (int& i = edge_pointer[current_vertex];
       i < int(graph[current_vertex].size()); ++i) {
    int edge_index = graph[current_vertex][i];
    Edge& edge = all_edges[edge_index];

    if (distances[edge.to] != distances[current_vertex] + 1) {
      continue;
    }
    if (edge.flow >= edge.capacity) {
      continue;
    }

    long long flow_delta = std::min(min_capacity, edge.capacity - edge.flow);
    long long pushed_flow = RunDfs(edge.to, sink, flow_delta);

    if (pushed_flow > 0) {
      edge.flow += pushed_flow;
      all_edges[edge.reverse_index].flow -= pushed_flow;
      return pushed_flow;
    }
  }
  return 0;
}

long long ComputeMaxFlow(int source, int sink) {
  long long max_flow = 0;

  while (RunBfs(source, sink)) {
    std::fill(edge_pointer.begin(), edge_pointer.end(), 0);

    long long flow = RunDfs(source, sink, cInf);
    while (flow > 0) {
      max_flow += flow;
      flow = RunDfs(source, sink, cInf);
    }
  }
  return max_flow;
}

void InitializeGraph(int vertex_count) {
  graph.resize(vertex_count + 1);
  distances.resize(vertex_count + 1);
  edge_pointer.resize(vertex_count + 1);
}

void AddEdge(int from, int to, long long capacity,
             std::vector<int>& edge_indices) {
  int forward_index = all_edges.size();
  all_edges.push_back({to, capacity, 0, 0});
  graph[from].push_back(forward_index);

  int reverse_index = all_edges.size();
  all_edges.push_back({from, 0, 0, forward_index});
  graph[to].push_back(reverse_index);

  all_edges[forward_index].reverse_index = reverse_index;
  edge_indices.push_back(forward_index);
}

int main() {
  std::ios_base::sync_with_stdio(false);
  std::cin.tie(0);
  std::cout.tie(0);

  int vertex_count;
  int edge_count;
  std::cin >> vertex_count >> edge_count;

  InitializeGraph(vertex_count);

  std::vector<int> original_edge_indices;
  for (int i = 0; i < edge_count; ++i) {
    int from_vertex;
    int to_vertex;
    long long edge_capacity;
    std::cin >> from_vertex >> to_vertex >> edge_capacity;
    AddEdge(from_vertex, to_vertex, edge_capacity, original_edge_indices);
  }

  const int cSource = 1;
  const int cSink = vertex_count;
  long long max_flow = ComputeMaxFlow(cSource, cSink);

  std::cout << max_flow << '\n';
  for (int edge_index : original_edge_indices) {
    std::cout << all_edges[edge_index].flow << '\n';
  }

  return 0;
}
