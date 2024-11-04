#include <bits/stdc++.h>

using namespace std;

using ll = long long;

int n, c;
constexpr int WMAX = 1'000, LMAX = 20;

int main() {
  cin.tie(nullptr)->sync_with_stdio(false);

  cin >> n >> c;

  vector<int> w(n);
  vector<ll> p(n);

  for (int i = 0; i < n; i++) cin >> w[i] >> p[i];

  vector lvls(LMAX + 1, vector<ll>(2 * WMAX + 1));

  for (int i = 0; i < n; i++) {
    for (int j = 0; j <= 2 * WMAX; j++) {
      if (j - w[i] >= 0) {
        lvls[0][j] = max(lvls[0][j], lvls[0][j - w[i]] + p[i]);
      }
    }
  }

  auto convolute = [&](const vector<ll> &a, const vector<ll> &b) {
    int n = ssize(a), m = ssize(b);
    vector<ll> ans(n + m - 1);
    for (int i = 0; i < n; i++) {
      for (int j = 0; j < m; j++) {
        ans[i + j] = max(ans[i + j], a[i] + b[j]);
      }
    }
    return ans;
  };

  auto merge = [&](const vector<ll> &a, const vector<ll> &b) {
    vector<ll> _ = convolute(a, b), ans;
    for (int i = 0; i < WMAX + 1; i++) ans.push_back(_[i + WMAX]);
    vector<ll> __ = convolute(lvls[0], ans), bak;
    for (int i = 0; i < WMAX; i++) bak.push_back(__[i + WMAX + 1]);
    for (auto &i : bak) ans.push_back(i);
    return ans;
  };

  for (int lv = 1; lv <= LMAX; lv++) {
    lvls[lv] = merge(lvls[lv - 1], lvls[lv - 1]);
  }

  int cr = 0;
  vector<ll> base;

  for (int lv = LMAX; lv >= 0; lv--) {
    if ((cr | (1 << lv)) * WMAX - WMAX >= c + 1) continue;
    base = empty(base) ? lvls[lv] : merge(base, lvls[lv]);
    cr |= (1 << lv);
  }

  cout << base[c - (cr * WMAX - WMAX)] << '\n';

  return 0;
}
