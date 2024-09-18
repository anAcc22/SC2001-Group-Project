#include <bits/stdc++.h>

using namespace std;

using ll = long long;

struct Timer {
  using Clock        = std::chrono::steady_clock;
  using Milliseconds = std::chrono::milliseconds;
  void reset() { t = Clock::now(); }
  double getElapsed() {
    auto elapsed = Clock::now() - t;
    return std::chrono::duration_cast<Milliseconds>(elapsed).count();
  }

private:
  std::chrono::time_point<Clock> t{ Clock::now() };
};

int main() {
  cin.tie(nullptr)->sync_with_stdio(false);

  vector<int> a;

  random_device dev;
  mt19937 gen(dev());

  uniform_int_distribution dist(1, INT_MAX);

  auto display = [&]() -> void {
    cout << "{ ";
    for (int i = 0; i < ssize(a); i++) {
      cout << a[i] << " ";
    }
    cout << "}\n";
  };

  auto merge = [&](int l, int m, int r) -> ll {
    ll cmps = 0LL;
    vector<int> merged;
    for (int i = l, j = m + 1; i <= m || j <= r;) {
      if (i == m + 1) merged.push_back(a[j++]);
      else if (j == r + 1) merged.push_back(a[i++]);
      else {
        cmps++;
        if (a[i] < a[j]) merged.push_back(a[i++]);
        else merged.push_back(a[j++]);
      }
    }
    ranges::copy(merged, begin(a) + l);
    return cmps;
  };

  auto insertion_sort = [&](int l, int r) -> ll {
    if (l >= r) return 0LL;
    ll cmps = 0LL;
    for (int i = l + 1; i <= r; i++) {
      for (int j = i; j >= l + 1; j--) {
        cmps++;
        if (a[j] < a[j - 1]) swap(a[j], a[j - 1]);
        else break;
      }
    }
    return cmps;
  };

  auto merge_sort = [&](this auto &&self, int l, int r) -> ll {
    if (l >= r) return 0LL;
    int m   = midpoint(l, r);
    ll cmps = self(l, m) + self(m + 1, r);
    cmps += merge(l, m, r);
    return cmps;
  };

  auto hybrid_sort = [&](this auto &&self, int l, int r, int s = 1) -> ll {
    if (l >= r) return 0LL;
    if (r - l + 1 <= s) return insertion_sort(l, r);
    int m   = midpoint(l, r);
    ll cmps = self(l, m, s) + self(m + 1, r, s);
    cmps += merge(l, m, r);
    return cmps;
  };

  auto build = [&](int size) -> void {
    a.resize(size);
    ranges::generate(a, [&]() { return dist(gen); });
  };

  auto ceil_div = [](int p, int q) { return (p + q - 1) / q; };

  // auto f = [](double n, double s) { // NOTE: naive variant
  //   return n * s + n * log2(n / s);
  // };

  auto f = [&](int n, int s) {
    int h    = __lg(ceil_div(n, s) - 1) + 1;
    double b = n >> h;
    return ((b - 1) * n / 4.0) + n - n * (log(b) / b) + 0.7355 * n * h;
  };

  // NOTE: <<- Part (i): fix `s` at 50 ->>

  /* vector<string> labels, comparisons, theoretical;

  for (int len = 1'000, inc = 250; len <= 1'000'000; len *= 10, inc = len / 4) {
    string start_size = to_string(len), end_size = to_string(10 * len);
    comparisons = { "Empirical" };
    labels      = { "Array Size" };
    theoretical = { "Theoretical" };

    for (int tmp = len, i = 0; i < 37; tmp += inc, i++) {
      cerr << "Array Size: " << tmp << '\n';
      double c = 0.0;
      for (int trial = 0; trial < 5; trial++) {
        build(tmp);
        c += hybrid_sort(0, ssize(a) - 1, 50);
      }
      labels.push_back(to_string(tmp));
      comparisons.push_back(to_string(c / 5.0));
      theoretical.push_back(to_string(f(tmp, 50)));
    }

    ofstream output("part_i_rev_" + start_size + "_" + end_size + ".txt");

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
    store(comparisons);
    output << '\n';
    store(theoretical);

    output.close();
  } */

  // NOTE: <<- Part (ii): fix `n` at 10,000 ->>

  /* vector<string> labels{ "Threshold" };
  vector<string> comparisons{ "Empirical" }, theoretical{ "Theoretical" };

  ofstream output("part_ii_rev.txt");

  for (int s = 10; s <= 1'000; s += 10) {
    double c = 0.0;
    for (int trail = 0; trail < 5; trail++) {
      build(10'000);
      c += hybrid_sort(0, ssize(a) - 1, s);
    }
    labels.push_back(to_string(s));
    comparisons.push_back(to_string(c / 5.0));
    theoretical.push_back(to_string(f(10'000, s)));
  }

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
  store(comparisons);
  output << '\n';
  store(theoretical);

  output.close(); */

  // NOTE: <<- Part (iii): vary both `s` and `n` ->>

  /* vector<string> labels{ "Threshold" };
  vector<int> ns{ 250'000, 500'000, 750'000, 1'000'000 };
  vector comparisons(ssize(ns), vector<string>());

  ofstream output("part_iii.txt");

  for (int s = 0; s <= 50; s++) {
    for (int i = 0; auto n : ns) {
      if (empty(comparisons[i])) comparisons[i] = { "n = " + to_string(n) };
      double c = 0.0;
      for (int trial = 0; trial < 5; trial++) {
        build(n);
        c += hybrid_sort(0, ssize(a) - 1, s);
      }
      comparisons[i++].push_back(to_string(c / 5.0));
    }
    labels.push_back(to_string(s));
  }

  auto store = [&](vector<string> &v) -> void {
    for (int i = 0; i < ssize(v); i++) {
      output << v[i];
      if (i != ssize(v) - 1) {
        output << ", ";
      }
    }
  };

  store(labels);
  for (const auto &v : comparisons) {
    output << '\n';
    vector<string> _ = v;
    store(_);
  } */

  // NOTE: <<- Part (iii): vary both `s` and `n` using RUNTIME ->>

  /* vector<string> labels{ "Threshold" };
  vector<int> ns{ 250'000, 500'000, 750'000, 1'000'000 };
  vector comparisons(ssize(ns), vector<string>());
  map<int, pair<int, double>> mp;

  Timer timer;

  ofstream output("part_iii_runtime.txt");

  for (int s = 0; s <= 250; s += 5) {
    for (int i = 0; auto n : ns) {
      if (empty(comparisons[i])) comparisons[i] = { "n = " + to_string(n) };
      double c = 0.0;
      for (int trial = 0; trial < 5; trial++) {
        timer.reset();
        build(n);
        hybrid_sort(0, ssize(a) - 1, s);
        c += timer.getElapsed();
      }
      c /= 5.0;
      comparisons[i++].push_back(to_string(c));
      if (!mp.contains(n) || c < mp[n].second) mp[n] = { s, c };
    }
    labels.push_back(to_string(s));
  }

  int best_s_sum = 0;

  for (auto &[k, v] : mp) best_s_sum += v.first;

  cerr << "Best `S`: " << best_s_sum / ssize(mp) << '\n';

  auto store = [&](vector<string> &v) -> void {
    for (int i = 0; i < ssize(v); i++) {
      output << v[i];
      if (i != ssize(v) - 1) {
        output << ", ";
      }
    }
  };

  store(labels);
  for (const auto &v : comparisons) {
    output << '\n';
    vector<string> _ = v;
    store(_);
  } */

  // NOTE: <<- Part (d): compare with Merge Sort (key comparisons) ->>

  vector<string> labels, cmp_hybrid, cmp_merge;

  Timer timer;

  for (int len = 1'000, inc = 250; len <= 100'000; len *= 10, inc = len / 4) {
    string start_size = to_string(len), end_size = to_string(10 * len);
    cmp_hybrid = { "Hybrid Sort" };
    cmp_merge  = { "Merge Sort" };
    labels     = { "Array Size" };

    for (int tmp = len, i = 0; i < 37; tmp += inc, i++) {
      cerr << "Array Size: " << tmp << '\n';
      int c_hybrid = 0, c_merge = 0;
      for (int trial = 0; trial < 5; trial++) {
        build(tmp);
        c_hybrid += hybrid_sort(0, ssize(a) - 1, 40);
        c_merge += merge_sort(0, ssize(a) - 1);
      }
      labels.push_back(to_string(tmp));
      cmp_hybrid.push_back(to_string(c_hybrid / 5));
      cmp_merge.push_back(to_string(c_merge / 5));
    }

    ofstream output("part_d_cmp_" + start_size + "_" + end_size + ".txt");

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
    store(cmp_hybrid);
    output << '\n';
    store(cmp_merge);

    output.close();
  }

  // NOTE: <<- Part (d): compare with Merge Sort (runtime) ->>

  /* vector<string> labels, cmp_hybrid, cmp_merge;

  Timer timer;

  for (int len = 1'000, inc = 250; len <= 100'000; len *= 10, inc = len / 4) {
    string start_size = to_string(len), end_size = to_string(10 * len);
    cmp_hybrid = { "Hybrid Sort" };
    cmp_merge  = { "Merge Sort" };
    labels     = { "Array Size" };

    for (int tmp = len, i = 0; i < 37; tmp += inc, i++) {
      cerr << "Array Size: " << tmp << '\n';
      double c_hybrid = 0.0, c_merge = 0.0;
      for (int trial = 0; trial < 5; trial++) {
        build(tmp);

        timer.reset();
        hybrid_sort(0, ssize(a) - 1, 40);
        c_hybrid += timer.getElapsed();

        timer.reset();
        merge_sort(0, ssize(a) - 1);
        c_merge += timer.getElapsed();
      }
      labels.push_back(to_string(tmp));
      cmp_hybrid.push_back(to_string(c_hybrid / 5.0));
      cmp_merge.push_back(to_string(c_merge / 5.0));
    }

    ofstream output("part_d_" + start_size + "_" + end_size + ".txt");

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
    store(cmp_hybrid);
    output << '\n';
    store(cmp_merge);

    output.close();
  } */

  return 0;
}
