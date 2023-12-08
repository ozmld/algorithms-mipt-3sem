#include <iostream>
#include <random>
#include <vector>

std::mt19937 rnd;

int FastPow(int num, int st, int mod) {
  if (st == 0) {
    return 1;
  }
  if (st % 2 == 0) {
    int hh = FastPow(num, st / 2, mod);
    return (int)((long long)hh * (long long)hh % (long long)mod);
  }
  return (int)((long long)num * (long long)FastPow(num, st - 1, mod) %
               (long long)mod);
}

int Sqrt(int xx) {
  int ii = 1;
  while (ii * ii <= xx) {
    ++ii;
  }
  return ii - 1;
}

bool IsResidue(int num, int mod) {
  return FastPow(num, (mod - 1) / 2, mod) == 1;
}

std::pair<int, int> FindMaxOdd(int mod) {
  int cur = mod - 1;
  int ord = 0;
  while (cur % 2 == 0) {
    cur /= 2;
    ++ord;
  }
  return {cur, ord};
}

const int kAdd = 15;

int ChooseNotResidue(int mod) {
  int num = rand() % mod;
  while (IsResidue(num, mod)) {
    num = (rand() + kAdd) % mod;
  }
  return num;
}

int FindOrd(int num, int mod, int ord) {
  int cur = num;
  for (int i = 0; i < ord; ++i) {
    if (cur == 1) {
      return i;
    }
    cur = (int)(((long long)cur * (long long)cur) % (long long)mod);
  }
  return ord;
}

int FindSqrtMod(int num, int mod) {
  if (num == 0 || mod == 2) {
    return num;
  }
  if (!IsResidue(num, mod)) {
    return -1;
  }
  // p = 1 + m * 2^s
  std::pair<int, int> coef_pair = FindMaxOdd(mod);
  int odd_num = coef_pair.first;
  int pow = coef_pair.second;
  int second_num = FastPow(num, odd_num, mod);
  int first_num = FastPow(num, (odd_num + 1) / 2, mod);
  int not_residue = ChooseNotResidue(mod);
  int num_in_pow = FastPow(not_residue, odd_num, mod);
  std::vector<int> powers(pow);
  for (int i = 0; i < pow; ++i) {
    powers[i] = (int)FastPow(num_in_pow, FastPow(2, i, mod), mod);
  }
  while (second_num != 1) {
    int remain = FindOrd(second_num, mod, pow);
    second_num = (int)((long long)second_num * (long long)powers[pow - remain] %
                       (long long)mod);
    first_num = (int)((long long)first_num *
                      (long long)powers[pow - remain - 1] % (long long)mod);
  }
  return first_num;
}

int main() {
  int tests_num;
  std::cin >> tests_num;
  for (int i = 0; i < tests_num; ++i) {
    int num;
    int mod;
    std::cin >> num >> mod;
    int ans = FindSqrtMod(num, mod);
    if (ans == -1) {
      std::cout << "IMPOSSIBLE";
    } else {
      std::cout << ans;
    }
    std::cout << "\n";
  }
}
