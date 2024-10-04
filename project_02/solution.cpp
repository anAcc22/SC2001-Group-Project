#include <bits/stdc++.h>

template <typename T, typename U>
std::ostream &operator<<(std::ostream &out, const std::pair<T, U> &p) {
  out << "[ " << p.first << ' ' << p.second << " ]";
  return out;
}

template <typename... Ts>
std::ostream &operator<<(std::ostream &out, const std::tuple<Ts...> &t) {
  out << "[ ";
  std::apply([&out](auto &&...args) { ((out << args << " "), ...); }, t);
  out << ']';
  return out;
}

template <typename ValueType, size_t SIZE>
std::ostream &
operator<<(std::ostream &out, const std::array<ValueType, SIZE> &c) {
  out << "[ ";
  for (const auto &v : c) out << v << ' ';
  out << ']';
  return out;
}

template <
    template <typename, typename...> typename ContainerType, typename ValueType,
    typename... Args>
void display(const ContainerType<ValueType, Args...> &c) {
  std::cout << "❯ { ";
  for (const auto &v : c) std::cout << v << " ";
  std::cout << "}\n";
}

using namespace std;
namespace rgs = ranges;

using ll  = long long;
using ill = pair<int, ll>;
using lli = pair<ll, int>;

int n, m;
constexpr ll INF = 1'000'000'000'000'000'000LL;

mt19937 gen(chrono::steady_clock::now().time_since_epoch().count());
uniform_int_distribution w_dist(1, 1'000'000'000);

vector<vector<ll>> adm;
vector<vector<ill>> adj;

int main() {
  cin.tie(nullptr)->sync_with_stdio(false);

  vector<ll> d, s;

  auto gen_graph = [&](int n, int m) -> void {
    assert(m <= n * n);

    adm.assign(n, vector<ll>(n, INF));
    adj.assign(n, vector<ill>());

    vector<int> ed(m), ws(m), rg(n * n);

    rgs::iota(rg, 0);
    rgs::sample(rg, begin(ed), m, gen);

    for (int i = 0; i < m; i++) {
      int u = ed[i] / n, v = ed[i] % n;
      ws[i]     = w_dist(gen);
      adm[u][v] = ws[i];
      adj[u].emplace_back(v, ws[i]);
    }
  };

  auto display_adm = [&]() -> void {
    int n = ssize(adm);
    for (int i = 0; i < n; i++) {
      for (int j = 0; j < n; j++) {
        if (adm[i][j] == INF) cout << 'I' << " \n"[j == n - 1];
        else cout << adm[i][j] << " \n"[j == n - 1];
      }
    }
  };

  auto display_adj = [&]() -> void {
    int n = ssize(adj);
    for (int i = 0; i < n; i++) {
      for (auto j = 0; j < ssize(adj[i]); j++) {
        string s = "[" + to_string(adj[i][j].first) + ", "
                   + to_string(adj[i][j].second) + "]";
        cout << s << " \n"[j == ssize(adj[i]) - 1];
      }
    }
  };

  gen_graph(5, 5);

  display_adm();
  cout << '\n';
  display_adj();

  /* NOTE: <<- CSES (Shortest Routes I) ->>
   * Passes 6/23 testcases, produces RTE on the rest.
   */
  auto imp_a = [&]() -> void {
    d.assign(n, INF);
    s.assign(n, false);

    d[0] = 0;

    auto get_min = [&]() -> int {
      int idx = -1;
      ll cur  = INF;
      for (int i = 0; i < n; i++) {
        if (!s[i] && d[i] < cur) {
          idx = i, cur = d[i];
        }
      }
      return idx;
    };

    auto is_empty = [&]() -> int { return get_min() == -1; };

    while (!is_empty()) {
      int i = get_min();
      s[i]  = true;
      for (int j = 0; j < n; j++) {
        if (i == j || adm[i][j] == INF) continue;
        d[j] = min(d[j], d[i] + adm[i][j]);
      }
    }
  };

  /* NOTE: <<- CSES (Shortest Routes I) ->>
   * Passes 15/23 testcases, produces TLE on the rest.
   */
  auto imp_b_ori = [&]() -> void {
    d.assign(n, INF);
    priority_queue<lli, vector<lli>, greater<>> pq;

    d[0] = 0;
    pq.emplace(0, 0);

    for (int u = 1; u < n; u++) {
      pq.emplace(INF, u);
    }

    while (!empty(pq)) {
      auto [cur, u] = pq.top();
      pq.pop();
      for (auto &[v, w] : adj[u]) {
        if (w == INF || d[u] + w >= d[v]) continue;
        d[v] = d[u] + w;
        pq.emplace(d[v], v);
      }
    }
  };

  /* NOTE: <<- CSES (Shortest Routes I) ->>
   * Passes 23/23 testcases (AC).
   */
  auto imp_b_opt = [&]() -> void {
    d.assign(n, INF);
    priority_queue<lli, vector<lli>, greater<>> pq;

    d[0] = 0;
    pq.emplace(0, 0);

    while (!empty(pq)) {
      auto [cur, u] = pq.top();
      pq.pop();
      if (cur != d[u]) continue;
      for (auto &[v, w] : adj[u]) {
        if (w == INF || d[u] + w >= d[v]) continue;
        d[v] = d[u] + w;
        pq.emplace(d[v], v);
      }
    }
  };

  return 0;
}
