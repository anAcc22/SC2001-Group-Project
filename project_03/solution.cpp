#include <bits/stdc++.h>

using namespace std;

using ll = long long;

int main() {
  cin.tie(nullptr)->sync_with_stdio(false);

  cout << setfill(' ') << setw(2);

  int n = 3;

  // NOTE: <<- Part (2) Input ->>
  /* vector<int> w{ 4, 6, 8 };
  vector<ll> p{ 7, 6, 9 }; */

  // NOTE: <<- Part (4) Input ->>
  vector<int> w{ 5, 6, 8 };
  vector<ll> p{ 7, 6, 9 };

  auto display = [](const vector<ll> &a) -> void {
    int n = ssize(a);
    for (int i = 0; i < n; i++) {
      cout << setfill(' ') << setw(2) << i << " \n"[i == n - 1];
    }
    for (int i = 0; i < n; i++) {
      cout << setfill(' ') << setw(2) << a[i] << " \n"[i == n - 1];
    }
  };

  // NOTE: <<- O(nc) memory ->>
  /* auto solve = [&](int c) -> ll {
    vector dp(n, vector(c + 1, 0LL));
    for (int i = 0; i < n; i++) {
      for (int j = 0; j <= c; j++) {
        dp[i][j] = i ? dp[i - 1][j] : 0;
        if (j - w[i] >= 0) dp[i][j] = max(dp[i][j], dp[i][j - w[i]] + p[i]);
      }
    }
    return dp[n - 1][c];
  }; */

  auto solve = [&](int c) -> ll {
    vector dp(c + 1, 0LL);
    for (int i = 0; i < n; i++) {
      for (int j = 0; j <= c; j++) {
        if (j - w[i] >= 0) dp[j] = max(dp[j], dp[j - w[i]] + p[i]);
      }
    }
    display(dp);
    return dp[c];
  };

  solve(14);

  return 0;
}
