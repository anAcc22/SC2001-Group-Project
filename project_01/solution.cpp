#include <bits/stdc++.h>

using namespace std;

using ll = long long;

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
    if (l + r - 1 <= s) return insertion_sort(l, r);
    int m   = midpoint(l, r);
    ll cmps = self(l, m, s) + self(m + 1, r, s);
    cmps += merge(l, m, r);
    return cmps;
  };

  auto build = [&](int size) -> void {
    a.resize(size);
    ranges::generate(a, [&]() { return dist(gen); });
  };

  build(100);

  display();

  auto ans = hybrid_sort(0, ssize(a) - 1);

  cout << ans << '\n';
  display();

  return 0;
}
