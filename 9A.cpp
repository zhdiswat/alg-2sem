#include <iostream>
#include <vector>

bool Dfs(int vertex, const std::vector<std::vector<int>>& graph,
         std::vector<int>& matching, std::vector<bool>& used) {
  if (used[vertex]) {
    return false;
  }
  used[vertex] = true;
  for (int to : graph[vertex]) {
    if (matching[to] == -1 || Dfs(matching[to], graph, matching, used)) {
      matching[to] = vertex;
      return true;
    }
  }
  return false;
}

std::pair<int, std::vector<std::pair<int, int>>> FindMaxMatching(
    const std::vector<std::vector<int>>& graph, int left_size, int right_size) {
  std::vector<int> matching(right_size, -1);
  std::vector<bool> used(left_size, false);
  int match_count = 0;

  for (int i = 0; i < left_size; ++i) {
    std::fill(used.begin(), used.end(), false);
    if (Dfs(i, graph, matching, used)) {
      ++match_count;
    }
  }

  std::vector<std::pair<int, int>> matching_pairs;
  for (int right_vertex = 0; right_vertex < right_size; ++right_vertex) {
    if (matching[right_vertex] != -1) {
      matching_pairs.emplace_back(matching[right_vertex] + 1, right_vertex + 1);
    }
  }

  return {match_count, matching_pairs};
}

int main() {
  std::ios_base::sync_with_stdio(false);
  std::cin.tie(nullptr);
  std::cout.tie(nullptr);

  int left_size;
  int right_size;
  std::cin >> left_size >> right_size;

  std::vector<std::vector<int>> graph(left_size);
  for (int i = 0; i < left_size; ++i) {
    int right_vertex;
    while (std::cin >> right_vertex && right_vertex != 0) {
      graph[i].push_back(right_vertex - 1);
    }
  }

  auto [match_count, matching_pairs] =
      FindMaxMatching(graph, left_size, right_size);
  std::cout << match_count << "\n";
  for (const auto& pair : matching_pairs) {
    std::cout << pair.first << " " << pair.second << "\n";
  }

  return 0;
}
