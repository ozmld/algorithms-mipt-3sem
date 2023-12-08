#include <iostream>
#include <string>
#include <vector>

const size_t kAlphSize = 256;

std::vector<int> GetSuffArray(std::string text) {
  std::vector<int> pp(text.size(), 0);
  std::vector<int> cc(text.size(), 0);
  std::vector<int> cnt(kAlphSize, 0);
  for (size_t i = 0; i < text.size(); ++i) {
    ++cnt[text[i]];
  }
  for (size_t i = 1; i < cnt.size(); ++i) {
    cnt[i] += cnt[i - 1];
  }
  for (int i = text.size() - 1; i >= 0; --i) {
    pp[--cnt[text[i]]] = i;
  }
  int x = 0;
  for (size_t i = 0; i < text.size(); ++i) {
    if (i == 0 || text[pp[i]] == text[pp[i - 1]]) {
      cc[pp[i]] = x;
    } else {
      cc[pp[i]] = ++x;
    }
  }
  int kk = 1;
  while (kk < (int)text.size()) {
    std::vector<int> sort_sec(text.size(), 0);
    for (size_t i = 0; i < text.size(); ++i) {
      sort_sec[i] = (pp[i] + text.size() - kk) % text.size();
    }
    cnt = std::vector<int>(text.size(), 0);
    for (size_t i = 0; i < text.size(); ++i) {
      ++cnt[cc[sort_sec[i]]];
    }
    for (size_t i = 1; i < cnt.size(); ++i) {
      cnt[i] += cnt[i - 1];
    }
    for (int i = text.size() - 1; i >= 0; --i) {
      pp[--cnt[cc[sort_sec[i]]]] = sort_sec[i];
    }

    std::vector<int> ccc(text.size(), 0);
    int x = 0;
    for (size_t i = 1; i < text.size(); ++i) {
      if (cc[pp[i]] != cc[pp[i - 1]] ||
          cc[(pp[i] + kk) % text.size()] !=
              cc[(pp[i - 1] + kk) % text.size()]) {
        ccc[pp[i]] = ++x;
      } else {
        ccc[pp[i]] = x;
      }
    }
    kk *= 2;
    cc = ccc;
  }
  return pp;
}

int main() {
  std::string text;
  std::cin >> text;
  std::vector<int> suffmas = GetSuffArray(text);
  for (size_t i = 0; i < suffmas.size(); ++i) {
    std::cout << text[(text.size() + suffmas[i] - 1) % text.size()];
  }
}
