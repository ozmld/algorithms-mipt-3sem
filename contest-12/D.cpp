#include <iostream>
#include <unordered_map>
#include <vector>

long long FastPow(long long num, long long st, long long mod) {
  if (st == 0) {
    return 1;
  }
  if (st % 2 == 0) {
    long long temp = FastPow(num, st / 2, mod);
    return temp * temp % mod;
  }
  return num * FastPow(num, st - 1, mod) % mod;
}

long long Sqrt(long long num) {
  long long sqrt = 1;
  while (sqrt * sqrt <= num) {
    ++sqrt;
  }
  return sqrt - 1;
}

long long FindLog(long long prime, long long base, long long num) {
  prime = std::max(prime, 2LL);
  long long bs = Sqrt(prime);
  std::unordered_map<int, int> map;
  map.reserve(bs + 1);
  long long aa = FastPow(base, bs, prime);
  long long cur = 1;
  for (long long i = 0; i < prime; ++i) {
    map.insert({(int)cur, i});
    cur *= aa;
    cur %= prime;
    if (i * bs > prime) {
      break;
    }
  }
  cur = num;
  long long ans = prime;
  for (long long i = 0; i <= bs; ++i) {
    if (map.find(cur) != map.end()) {
      // return (prime + map.at(cur) * bs - i - 1) % (prime - 1);
      ans = std::min(ans, (prime + map.at(cur) * bs - i - 1) % (prime - 1));
    }
    cur *= base;
    cur %= prime;
  }
  cur = 1;
  for (long long i = 0; i < bs; ++i) {
    if (cur == num) {
      ans = std::min(ans, i);
    }
    cur *= base;
    cur %= prime;
  }
  return ans;
}

int main() {
  long long prime;
  long long base;
  long long num;
  while (std::cin >> prime >> base >> num) {
    long long log = FindLog(prime, base, num);
    if (log == prime) {
      std::cout << "no solution\n";
    } else {
      std::cout << log << "\n";
    }
  }
}
