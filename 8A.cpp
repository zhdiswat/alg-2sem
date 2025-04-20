#include <algorithm>
#include <climits>
#include <iostream>
#include <map>
#include <queue>
#include <set>
#include <vector>

struct GE {
  int to;
  long long c;
};

struct P {
  int n;
  int u;
  int d;
  int i;
  int o;
  int k;
  std::vector<std::vector<int>> tp;
};

void RdIn(P& p) {
  std::cin >> p.n >> p.u >> p.d >> p.i >> p.o >> p.k;
  p.tp.resize(p.k);
  for (int j = 0; j < p.k; ++j) {
    int t;
    std::cin >> t;
    p.tp[j].resize(t);
    for (int l = 0; l < t; ++l) {
      std::cin >> p.tp[j][l];
    }
  }
}

std::set<int> GetSf(const P& p) {
  std::set<int> sf;
  sf.insert(1);
  sf.insert(p.n);
  for (const auto& t : p.tp) {
    for (int f : t) {
      sf.insert(f);
    }
  }
  return sf;
}

std::vector<int> MkFl(const std::set<int>& sf) {
  std::vector<int> fl(sf.begin(), sf.end());
  return fl;
}

std::map<int, int> MkF2i(const std::vector<int>& fl) {
  std::map<int, int> f2i;
  for (int i = 0; i < fl.size(); ++i) {
    f2i[fl[i]] = i;
  }
  return f2i;
}

void AddEl(const std::vector<int>& fl, int u, int d,
           std::vector<std::vector<GE>>& g) {
  int nv = fl.size();
  for (int i = 0; i < nv; ++i) {
    for (int j = 0; j < nv; ++j) {
      if (i != j) {
        int ff = fl[i];
        int tf = fl[j];
        long long c = (tf > ff) ? (long long)u * (tf - ff)
                                : (long long)d * (ff - tf);
        g[i].push_back({j, c});
      }
    }
  }
}

void AddTp(const P& p, const std::map<int, int>& f2i,
           std::vector<std::vector<GE>>& g) {
  for (const auto& t : p.tp) {
    for (int f : t) {
      for (int to : t) {
        if (f != to) {
          int fi = f2i.at(f);
          int ti = f2i.at(to);
          g[fi].push_back({ti, p.i + p.o});
        }
      }
    }
  }
}

std::vector<long long> Djk(const std::vector<std::vector<GE>>& g, int si) {
  int nv = g.size();
  std::vector<long long> dist(nv, LLONG_MAX);
  dist[si] = 0;
  std::priority_queue<std::pair<long long, int>,
                      std::vector<std::pair<long long, int>>,
                      std::greater<>> pq;
  pq.push({0, si});
  while (!pq.empty()) {
    long long c = pq.top().first;
    int v = pq.top().second;
    pq.pop();
    if (c > dist[v]) {
      continue;
    }
    for (const auto& e : g[v]) {
      int n = e.to;
      long long nc = e.c;
      if (dist[v] + nc < dist[n]) {
        dist[n] = dist[v] + nc;
        pq.push({dist[n], n});
      }
    }
  }
  return dist;
}

long long FSp(const P& p) {
  std::set<int> sf = GetSf(p);
  std::vector<int> fl = MkFl(sf);
  std::map<int, int> f2i = MkF2i(fl);
  int nv = fl.size();
  std::vector<std::vector<GE>> g(nv);
  AddEl(fl, p.u, p.d, g);
  AddTp(p, f2i, g);
  int si = f2i[1];
  int ei = f2i[p.n];
  std::vector<long long> dist = Djk(g, si);
  return dist[ei];
}

int main() {
  std::ios_base::sync_with_stdio(false);
  std::cin.tie(nullptr);
  std::cout.tie(nullptr);
  P p;
  RdIn(p);
  long long r = FSp(p);
  std::cout << r << "\n";
  return 0;
}
