#include <iostream>
#include <set>
#include <utility>
#include <vector>

const int cMaxVertexCount = 100000;
const long long cInfinity = 2009000999LL;

std::vector<std::vector<std::pair<int, int>>> graph(cMaxVertexCount);

std::vector<long long> Dijkstra(int start_vertex, int vertex_count) {
  std::vector<long long> distances(vertex_count, cInfinity);
  distances[start_vertex] = 0;
  std::set<std::pair<long long, int>> priority_queue;
  priority_queue.insert({0, start_vertex});
  while (!priority_queue.empty()) {
    long long current_distance = priority_queue.begin()->first;
    int current_vertex = priority_queue.begin()->second;
    priority_queue.erase(priority_queue.begin());

    if (current_distance > distances[current_vertex]) {
      continue;
    }
    for (const auto& edge : graph[current_vertex]) {
      int next_vertex = edge.first;
      int weight = edge.second;
      long long new_distance = distances[current_vertex] + weight;
      if (new_distance < distances[next_vertex]) {
        if (distances[next_vertex] != cInfinity) {
          priority_queue.erase({distances[next_vertex], next_vertex});
        }
        distances[next_vertex] = new_distance;
        priority_queue.insert({new_distance, next_vertex});
      }
    }
  }
  return distances;
}

int main() {
  std::ios_base::sync_with_stdio(false);
  std::cin.tie(0);
  std::cout.tie(0);
  int test_count;
  std::cin >> test_count;

  for (int test = 0; test < test_count; test++) {
    int vertex_count;
    std::cin >> vertex_count;
    int edge_count;
    std::cin >> edge_count;
    for (int i = 0; i < vertex_count; i++) {
      graph[i].clear();
    }
    for (int i = 0; i < edge_count; i++) {
      int from_vertex;
      std::cin >> from_vertex;
      int to_vertex;
      std::cin >> to_vertex;
      int weight;
      std::cin >> weight;
      graph[from_vertex].push_back({to_vertex, weight});
      graph[to_vertex].push_back({from_vertex, weight});
    }
    int start_vertex;
    std::cin >> start_vertex;
    std::vector<long long> distances = Dijkstra(start_vertex, vertex_count);
    for (int v = 0; v < vertex_count; v++) {
      std::cout << distances[v] << " ";
    }
    std::cout << "\n";
  }
  return 0;
}
