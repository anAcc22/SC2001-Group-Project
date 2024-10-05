#include <bits/stdc++.h>

using namespace std;
namespace rgs = ranges;

using ll  = long long;
using ill = pair<int, ll>;
using lli = pair<ll, int>;

mt19937 gen(chrono::steady_clock::now().time_since_epoch().count());
uniform_int_distribution w_dist(1, 1'000'000'000);

int n, m;
constexpr ll INF = 1'000'000'000'000'000'000LL;

vector<vector<ll>> adm;
vector<vector<ill>> adj;

vector<ll> d, s;

struct MiniTimer {
  using Clock        = chrono::steady_clock;
  using Milliseconds = chrono::milliseconds;
  void reset() { t = Clock::now(); }
  void displayElapsed() { cout << "Elapsed (ms): " << getElapsed() << "\n"; }
  double getElapsed() {
    return chrono::duration_cast<Milliseconds>(Clock::now() - t).count();
  }

private:
  chrono::time_point<Clock> t{ Clock::now() };
};

class SegTree {
  int n;
  vector<lli> a, tree;
  void build(int i, int l, int r) {
    if (l == r) tree[i] = a[l];
    else {
      int m = (l + r) / 2;
      build(2 * i, l, m);
      build(2 * i + 1, m + 1, r);
      tree[i] = conquer(tree[2 * i], tree[2 * i + 1]);
    }
  }
  void update(int i, int tl, int tr, int pos, lli val) {
    if (tl == tr) tree[i] = val;
    else {
      int tm = (tl + tr) / 2;
      if (pos <= tm) update(2 * i, tl, tm, pos, val);
      else update(2 * i + 1, tm + 1, tr, pos, val);
      tree[i] = conquer(tree[2 * i], tree[2 * i + 1]);
    }
  }
  lli query(int i, int tl, int tr, int l, int r) {
    if (l > r) return { INF, -1 };
    if (l == tl && r == tr) return tree[i];
    int tm = (tl + tr) / 2;
    return conquer(
        query(2 * i, tl, tm, l, min(r, tm)),
        query(2 * i + 1, tm + 1, tr, max(l, tm + 1), r));
  }

public:
  SegTree(vector<lli> &a)
      : a(a) {
    n = ssize(a);
    tree.assign(4 * n, {});
    build(1, 0, n - 1);
  }
  lli conquer(lli x, lli y) { return min(x, y); }
  void update(int pos, lli val) { update(1, 0, n - 1, pos, val); }
  lli query(int l, int r) { return query(1, 0, n - 1, l, r); }
};

int main() {
  cin.tie(nullptr)->sync_with_stdio(false);

  MiniTimer timer;

  auto gen_graph = [&](int n, int m, bool gen_adm, bool gen_adj) -> void {
    assert(m <= n * n);
    ::n = n, ::m = m;

    if (gen_adm) adm.assign(n, vector<ll>(n, INF));
    if (gen_adj) adj.assign(n, vector<ill>());

    vector<int> ed(m), ws(m), rg(n * n);

    rgs::iota(rg, 0);
    rgs::sample(rg, begin(ed), m, gen);

    for (int i = 0; i < m; i++) {
      int u = ed[i] / n, v = ed[i] % n;
      ws[i] = w_dist(gen);
      if (gen_adm) adm[u][v] = ws[i];
      if (gen_adj) adj[u].emplace_back(v, ws[i]);
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

  /* NOTE: <<- CSES (Shortest Routes I) ->>
   * Passes 6/23 testcases, produces RTE on the rest.
   */

  auto imp_a = [&]() -> ll {
    ll ops = 2 * n;

    d.assign(n, INF);
    s.assign(n, false);

    d[0] = 0;

    auto get_min = [&]() -> int {
      int idx = -1;
      ll cur  = INF;
      for (int i = 0; i < n; i++, ops++) {
        if (!s[i] && d[i] < cur) {
          idx = i, cur = d[i];
          ops += 2;
        }
      }
      return idx;
    };

    auto is_empty = [&]() -> int { return get_min() == -1; };

    while (!is_empty()) {
      int i = get_min();
      s[i]  = true;
      for (int j = 0; j < n; j++, ops++) {
        if (i == j || adm[i][j] == INF) continue;
        d[j] = min(d[j], d[i] + adm[i][j]);
      }
    }

    return ops;
  };

  /* NOTE: <<- CSES (Shortest Routes I) ->>
   * Passes 23/23 testcases (AC).
   */

  auto imp_b_ori = [&]() -> void {
    d.assign(n, INF);
    set<lli> pq;

    d[0] = 0;
    pq.emplace(d[0], 0);

    for (int u = 1; u < n; u++) {
      pq.emplace(INF, u);
    }

    while (!empty(pq)) {
      auto [cur, u] = *begin(pq);
      pq.erase(begin(pq));
      for (auto &[v, w] : adj[u]) {
        if (w == INF || d[u] + w >= d[v]) continue;
        pq.erase({ d[v], v });
        d[v] = d[u] + w;
        pq.emplace(d[v], v);
      }
    }
  };

  /* NOTE: <<- CSES (Shortest Routes I) ->>
   * Passes 15/23 testcases, produces TLE on the rest.
   */

  auto imp_b_bad = [&]() -> void {
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

  auto imp_b_opt = [&]() -> ll {
    ll ops = n;
    d.assign(n, INF);
    priority_queue<lli, vector<lli>, greater<>> pq;
    d[0] = 0;
    pq.emplace(0, 0);
    while (!empty(pq)) {
      auto [cur, u] = pq.top();
      ops += log2(ssize(pq));
      pq.pop();
      if (cur != d[u]) continue;
      ops++;
      for (auto &[v, w] : adj[u]) {
        ops += 3;
        if (w == INF || d[u] + w >= d[v]) continue;
        d[v] = d[u] + w;
        ops++;
        pq.emplace(d[v], v);
        ops += log2(ssize(pq));
      }
    }
    return ops;
  };

  /* NOTE: <<- CSES (Shortest Routes I) ->>
   * Passes 23/23 testcases (AC).
   */

  auto imp_b_seg = [&]() -> void {
    d.assign(n, INF);

    vector<lli> _(n);
    for (int i = 1; i < n; i++) _[i] = { INF, i };
    SegTree pq(_);

    d[0] = 0;

    while (pq.query(0, n - 1).first != INF) {
      auto [cur, u] = pq.query(0, n - 1);
      pq.update(u, { INF, u });
      if (cur != d[u]) continue;
      for (auto &[v, w] : adj[u]) {
        if (w == INF || d[u] + w >= d[v]) continue;
        d[v] = d[u] + w;
        pq.update(v, { d[v], v });
      }
    }
  };

  auto gen_break_b_bad = [&](int n) -> void {
    ::n = n, ::m = (n - 1) * n / 2;
    adm.assign(n, vector<ll>(n, INF));
    adj.assign(n, vector<ill>());

    // 0: I   n       2*n     3*n     4*n     5*n ...
    // 1: n   I     (n-1) 2*(n-1) 3*(n-1) 4*(n-1) ...
    // 2: 2*n (n-1)     I   (n-2) 2*(n-1) 3*(n-1) ...
    // ...

    for (int i = 0; i < n; i++) {
      for (int j = i + 1; j < n; j++) {
        ll w      = (ll)(n - i) * (j - i);
        adm[i][j] = adm[j][i] = w;
        adj[i].emplace_back(j, w), adj[j].emplace_back(i, w);
      }
    }
  };

  // NOTE: <<- Part (a) (vary n, m = n²) ->>

  /* vector<string> labels{ "V" };
  vector<string> empirical{ "Empirical" }, theoretical{ "Theoretical (3V²)" };

  for (int i = 100; i <= 500; i += 20) {
    ll avg = 0, trial_cnt = 5;
    for (int trial = 0; trial < trial_cnt; trial++) {
      gen_graph(i, i*i, true, false);
      avg += imp_a();
    }
    labels.push_back(to_string(i));
    theoretical.push_back(to_string((ll)3 * i * i));
    empirical.push_back(to_string(avg / trial_cnt));
  }

  ofstream output("part_i_n.txt");

  auto store = [&](vector<string> &v) -> void {
    for (int i = 0; i < ssize(v); i++) {
      output << v[i];
      if (i != ssize(v) - 1) {
        output << ", ";
      }
    }
  };

  store(labels);
  output << '\n';
  store(theoretical);
  output << '\n';
  store(empirical); */

  // NOTE: <<- Part (a) (vary n, m = 500) ->>

  /* vector<string> labels{ "V" };
  vector<string> empirical{ "Empirical" }, theoretical{ "Theoretical (3V²)" };

  for (int i = 100; i <= 500; i += 20) {
    ll avg = 0, trial_cnt = 5;
    for (int trial = 0; trial < trial_cnt; trial++) {
      gen_graph(i, 500, true, false);
      avg += imp_a();
    }
    labels.push_back(to_string(i));
    theoretical.push_back(to_string((ll)3 * i * i));
    empirical.push_back(to_string(avg / trial_cnt));
  }

  ofstream output("part_i_n_m_500.txt");

  auto store = [&](vector<string> &v) -> void {
    for (int i = 0; i < ssize(v); i++) {
      output << v[i];
      if (i != ssize(v) - 1) {
        output << ", ";
      }
    }
  };

  store(labels);
  output << '\n';
  store(theoretical);
  output << '\n';
  store(empirical); */

  // NOTE: <<- Part (a) (vary m, n = 500) ->>

  /* vector<string> labels{ "M" };
  vector<string> empirical{ "Empirical" }, theoretical{ "Theoretical (3V²)" };

  for (int i = 0; i <= 10'000; i += 100) {
    ll avg = 0, trial_cnt = 5;
    for (int trial = 0; trial < trial_cnt; trial++) {
      gen_graph(500, i, true, false);
      avg += imp_a();
    }
    labels.push_back(to_string(i));
    theoretical.push_back(to_string((ll)3 * 500 * 500));
    empirical.push_back(to_string(avg / trial_cnt));
  }

  ofstream output("part_i_m.txt");

  auto store = [&](vector<string> &v) -> void {
    for (int i = 0; i < ssize(v); i++) {
      output << v[i];
      if (i != ssize(v) - 1) {
        output << ", ";
      }
    }
  };

  store(labels);
  output << '\n';
  store(theoretical);
  output << '\n';
  store(empirical); */

  // NOTE: <<- Part (b) (vary n, m = 2'500) ->>

  /* vector<string> labels{ "N" };
  vector<string> empirical{ "Empirical" },
      theoretical{ "Theoretical (V + E·log(E))" };

  for (int i = 100; i <= 500; i += 10) {
    ll avg = 0, trial_cnt = 5;
    for (int trial = 0; trial < trial_cnt; trial++) {
      gen_graph(i, 2'500, false, true);
      avg += imp_b_opt();
    }
    labels.push_back(to_string(i));
    theoretical.push_back(to_string(i + (2'500 * log2(2'500))));
    empirical.push_back(to_string(avg / trial_cnt));
  }

  ofstream output("part_ii_n.txt");

  auto store = [&](vector<string> &v) -> void {
    for (int i = 0; i < ssize(v); i++) {
      output << v[i];
      if (i != ssize(v) - 1) {
        output << ", ";
      }
    }
  };

  store(labels);
  output << '\n';
  store(theoretical);
  output << '\n';
  store(empirical); */

  // NOTE: <<- Part (b) (vary m, n = ?) ->>

  /* vector<string> labels{ "M" };
  vector<string> empirical{ "Empirical" },
      theoretical{ "Theoretical (V + E·log(E))" };

  for (int i = 500; i <= 5000; i += 100) {
    ll avg = 0, trial_cnt = 5;
    for (int trial = 0; trial < trial_cnt; trial++) {
      gen_graph(1'000, i, false, true);
      avg += imp_b_opt();
    }
    labels.push_back(to_string(i));
    theoretical.push_back(to_string(1'000 + (i * log2(i))));
    empirical.push_back(to_string(avg / trial_cnt));
  }

  ofstream output("part_ii_m.txt");

  auto store = [&](vector<string> &v) -> void {
    for (int i = 0; i < ssize(v); i++) {
      output << v[i];
      if (i != ssize(v) - 1) {
        output << ", ";
      }
    }
  };

  store(labels);
  output << '\n';
  store(theoretical);
  output << '\n';
  store(empirical); */

  // NOTE: <<- Part (b) (edge case) ->>

  /* vector<string> labels{ "N" };
  vector<string> empirical{ "Empirical" },
      theoretical{ "Theoretical (V + E·log(E))" };

  for (int i = 100; i <= 200; i += 5) {
    ll avg = 0, trial_cnt = 5;
    for (int trial = 0; trial < trial_cnt; trial++) {
      gen_break_b_bad(i);
      avg += imp_b_opt();
    }
    labels.push_back(to_string(i));
    theoretical.push_back(to_string(i + (i*i * log2(i*i))));
    empirical.push_back(to_string(avg / trial_cnt));
  }

  ofstream output("part_ii_edge.txt");

  auto store = [&](vector<string> &v) -> void {
    for (int i = 0; i < ssize(v); i++) {
      output << v[i];
      if (i != ssize(v) - 1) {
        output << ", ";
      }
    }
  };

  store(labels);
  output << '\n';
  store(theoretical);
  output << '\n';
  store(empirical); */

  // NOTE: <<- Part (c) (vary n, m = 500) ->>

  /* vector<string> labels{ "N" };
  vector<string> part_a{ "Part (a)" }, part_b{ "Part (b)" };

  for (int i = 100; i <= 1'000; i += 20) {
    ll avg_a = 0, avg_b = 0, trial_cnt = 5;
    for (int trial = 0; trial < trial_cnt; trial++) {
      gen_graph(i, 5'000, true, true);

      timer.reset();
      imp_a();
      avg_a += timer.getElapsed();

      timer.reset();
      imp_b_opt();
      avg_b += timer.getElapsed();
    }
    labels.push_back(to_string(i));
    part_a.push_back(to_string(avg_a / trial_cnt));
    part_b.push_back(to_string(avg_b / trial_cnt));
  }

  ofstream output("part_iii_n.txt");

  auto store = [&](vector<string> &v) -> void {
    for (int i = 0; i < ssize(v); i++) {
      output << v[i];
      if (i != ssize(v) - 1) {
        output << ", ";
      }
    }
  };

  store(labels);
  output << '\n';
  store(part_a);
  output << '\n';
  store(part_b); */

  // NOTE: <<- Part (c) (complete graph) ->>

  /* vector<string> labels{ "N" };
  vector<string> part_a{ "Part (a)" }, part_b{ "Part (b)" };

  for (int i = 100; i <= 1'000; i += 50) {
    ll avg_a = 0, avg_b = 0, trial_cnt = 5;
    for (int trial = 0; trial < trial_cnt; trial++) {
      gen_graph(i, i*i, true, true);

      timer.reset();
      imp_a();
      avg_a += timer.getElapsed();

      timer.reset();
      imp_b_opt();
      avg_b += timer.getElapsed();
    }
    labels.push_back(to_string(i));
    part_a.push_back(to_string(avg_a / trial_cnt));
    part_b.push_back(to_string(avg_b / trial_cnt));
  }

  ofstream output("part_iii_complete.txt");

  auto store = [&](vector<string> &v) -> void {
    for (int i = 0; i < ssize(v); i++) {
      output << v[i];
      if (i != ssize(v) - 1) {
        output << ", ";
      }
    }
  };

  store(labels);
  output << '\n';
  store(part_a);
  output << '\n';
  store(part_b); */

  // NOTE: <<- Part (c) (segment tree vs binary heap, vary n, m = 500) ->>

  /* vector<string> labels{ "N" };
  vector<string> segtr{ "Segment Tree" }, bheap{ "Binary Heap" };

  for (int i = 100; i <= 1'000; i += 20) {
    ll avg_s = 0, avg_b = 0, trial_cnt = 5;
    for (int trial = 0; trial < trial_cnt; trial++) {
      gen_graph(i, 5'000, false, true);

      timer.reset();
      imp_b_opt();
      avg_b += timer.getElapsed();

      timer.reset();
      imp_b_seg();
      avg_s += timer.getElapsed();
    }
    labels.push_back(to_string(i));
    segtr.push_back(to_string(avg_s / trial_cnt));
    bheap.push_back(to_string(avg_b / trial_cnt));
  }

  ofstream output("part_iii_n_seg.txt");

  auto store = [&](vector<string> &v) -> void {
    for (int i = 0; i < ssize(v); i++) {
      output << v[i];
      if (i != ssize(v) - 1) {
        output << ", ";
      }
    }
  };

  store(labels);
  output << '\n';
  store(segtr);
  output << '\n';
  store(bheap); */

  // NOTE: <<- Part (c) (segment tree vs binary heap, complete graph) ->>

  vector<string> labels{ "N" };
  vector<string> segtr{ "Segment Tree" }, bheap{ "Binary Heap" };

  for (int i = 100; i <= 1'000; i += 100) {
    ll avg_s = 0, avg_b = 0, trial_cnt = 5;
    for (int trial = 0; trial < trial_cnt; trial++) {
      gen_graph(i, i*i, true, true);

      timer.reset();
      imp_b_opt();
      avg_b += timer.getElapsed();

      timer.reset();
      imp_b_seg();
      avg_s += timer.getElapsed();
    }
    labels.push_back(to_string(i));
    segtr.push_back(to_string(avg_s / trial_cnt));
    bheap.push_back(to_string(avg_b / trial_cnt));
  }

  ofstream output("part_iii_complete_seg.txt");

  auto store = [&](vector<string> &v) -> void {
    for (int i = 0; i < ssize(v); i++) {
      output << v[i];
      if (i != ssize(v) - 1) {
        output << ", ";
      }
    }
  };

  store(labels);
  output << '\n';
  store(segtr);
  output << '\n';
  store(bheap);

  return 0;
}
