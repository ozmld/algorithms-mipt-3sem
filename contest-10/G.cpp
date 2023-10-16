#include <algorithm>
#include <cstring>
#include <iostream>
#include <queue>
#include <set>

const size_t kAlphSize = 26;

struct Node {
  int to[kAlphSize];
  bool term;
  int ind;
  Node() {
    memset(to, -1, sizeof(to));
    term = false;
  }
};

class Trie {
  std::vector<Node> trie_;

 public:
  void AddString(std::string& str, int ind) {
    if (trie_.empty()) {
      trie_.push_back(Node());
    }
    int vertex = 0;
    for (size_t i = 0; i < str.length(); ++i) {
      if (trie_[vertex].to[str[i] - 'a'] == -1) {
        trie_[vertex].to[str[i] - 'a'] = trie_.size();
        trie_.push_back(Node());
      }
      vertex = trie_[vertex].to[str[i] - 'a'];
    }
    trie_[vertex].term = true;
    trie_[vertex].ind = ind;
  }

  int IsTerm(std::string& str, size_t finish_pos) {
    int vertex = 0;
    for (size_t i = 0; i < finish_pos; ++i) {
      vertex = trie_[vertex].to[str[i] - 'a'];
      if (vertex == -1) {
        return -1;
      }
    }
    if (!trie_[vertex].term) {
      return -1;
    }
    return trie_[vertex].ind;
  }
};

bool IsPalindrom(std::string& str, size_t start_pos) {
  for (size_t i = start_pos; i < str.length(); ++i) {
    if (str[i] != str[str.length() - (i - start_pos) - 1]) {
      return false;
    }
  }
  return true;
}

std::vector<std::pair<size_t, size_t>> FindPalindromePairs(
    std::vector<std::string> strings) {
  std::vector<std::pair<size_t, size_t>> matches;
  Trie normal;
  Trie reversed;
  for (size_t i = 0; i < strings.size(); ++i) {
    reversed.AddString(strings[i], i);
    std::reverse(strings[i].begin(), strings[i].end());
    normal.AddString(strings[i], i);
  }
  for (size_t i = 0; i < strings.size(); ++i) {
    for (size_t jj = 0; jj <= strings[i].length(); ++jj) {
      if (reversed.IsTerm(strings[i], jj) != -1 &&
          IsPalindrom(strings[i], jj) &&
          reversed.IsTerm(strings[i], jj) != (int)i) {
        matches.push_back({reversed.IsTerm(strings[i], jj), i});
      }
      if (jj == strings[i].length()) {
        continue;
      }
      std::reverse(strings[i].begin(), strings[i].end());
      if (normal.IsTerm(strings[i], jj) != -1 && IsPalindrom(strings[i], jj) &&
          normal.IsTerm(strings[i], jj) != (int)i) {
        matches.push_back({i, normal.IsTerm(strings[i], jj)});
      }
      std::reverse(strings[i].begin(), strings[i].end());
    }
  }
  return matches;
}
int main() {
  size_t str_num;
  std::cin >> str_num;
  std::vector<std::string> strings;
  for (size_t i = 0; i < str_num; ++i) {
    std::string str;
    std::cin >> str;
    strings.push_back(str);
  }
  std::vector<std::pair<size_t, size_t>> matches;
  matches = FindPalindromePairs(strings);

  std::cout << matches.size() << "\n";
  for (size_t i = 0; i < matches.size(); ++i) {
    std::cout << matches[i].first + 1 << " " << matches[i].second + 1 << "\n";
  }
}
