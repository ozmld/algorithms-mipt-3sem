#include <cstring>
#include <iostream>
#include <queue>
#include <set>

const size_t kAlphSize = 26;
const int kAlphabet[kAlphSize] = {0,  1,  2,  3,  4,  5,  6,  7,  8,
                                  9,  10, 11, 12, 13, 14, 15, 16, 17,
                                  18, 19, 20, 21, 22, 23, 24, 25};

struct Node {
  int to[kAlphSize];
  bool term;
  std::set<int> term_ind;
  int link;
  int go[kAlphSize];
  int cnt = 0;
  std::set<int> cnt_indexes = {};
  Node() {
    memset(to, -1, sizeof(to));
    memset(go, -1, sizeof(go));
    term = false;
    cnt = 0;
  }
};

class Trie {
  std::vector<Node> trie_;

  void CntIndexesUpdate(int vertex_v) {
    if (trie_[vertex_v].term) {
      trie_[vertex_v].cnt_indexes.insert(trie_[vertex_v].term_ind.begin(),
                                         trie_[vertex_v].term_ind.end());
    }
    if (trie_[vertex_v].link != -1) {
      trie_[vertex_v].cnt_indexes.insert(
          trie_[trie_[vertex_v].link].cnt_indexes.begin(),
          trie_[trie_[vertex_v].link].cnt_indexes.end());
    }
  }

 public:
  Trie() { trie_ = {Node()}; }
  void AddString(std::string& str, int ind) {
    if (trie_.empty()) {
      Trie();
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
    trie_[vertex].term_ind.insert(ind);
  }
  void BuildTrie() {
    trie_[0].link = -1;
    for (auto character : kAlphabet) {
      if (trie_[0].to[character] != -1) {
        trie_[0].go[character] = trie_[0].to[character];
      } else {
        trie_[0].go[character] = 0;
      }
    }
    std::queue<int> vertices_queue;
    vertices_queue.push(0);
    while (!vertices_queue.empty()) {
      int vertex_v = vertices_queue.front();
      vertices_queue.pop();
      CntIndexesUpdate(vertex_v);
      trie_[vertex_v].cnt =
          (trie_[vertex_v].term ? 1 : 0) +
          (trie_[vertex_v].link == -1 ? 0 : trie_[trie_[vertex_v].link].cnt);
      for (auto character1 : kAlphabet) {
        int vertex_u = trie_[vertex_v].to[character1];
        if (vertex_u == -1) {
          continue;
        }
        trie_[vertex_u].link =
            (vertex_v == 0 ? 0 : trie_[trie_[vertex_v].link].go[character1]);
        for (auto character2 : kAlphabet) {
          if (trie_[vertex_u].to[character2] != -1) {
            trie_[vertex_u].go[character2] = trie_[vertex_u].to[character2];
          } else {
            trie_[vertex_u].go[character2] =
                trie_[trie_[vertex_u].link].go[character2];
          }
        }
        vertices_queue.push(vertex_u);
      }
    }
  }
  int GetNext(int vertex, char symb) { return trie_[vertex].go[symb - 'a']; }
  std::set<int> GetMatches(int vertex) { return trie_[vertex].cnt_indexes; }
};

std::vector<std::vector<int>> FindAllMatches(
    std::string& text, std::vector<std::string>& patterns) {
  std::vector<std::vector<int>> matches(patterns.size());
  Trie trie;
  for (size_t i = 0; i < patterns.size(); ++i) {
    trie.AddString(patterns[i], i);
  }
  trie.BuildTrie();
  int vertex = 0;
  for (size_t i = 0; i < text.length(); ++i) {
    vertex = trie.GetNext(vertex, text[i]);
    for (auto ind : trie.GetMatches(vertex)) {
      matches[ind].push_back(i + 1);
    }
  }
  return matches;
}

int main() {
  std::string text;
  std::cin >> text;

  size_t pattern_num;
  std::cin >> pattern_num;
  std::vector<std::string> patterns;
  for (size_t i = 0; i < pattern_num; ++i) {
    std::string pattern;
    std::cin >> pattern;
    patterns.push_back(pattern);
  }
  std::vector<std::vector<int>> matches = FindAllMatches(text, patterns);
  for (size_t i = 0; i < matches.size(); ++i) {
    std::cout << matches[i].size() << " ";
    for (auto match_ind : matches[i]) {
      std::cout << match_ind + 1 - patterns[i].length() << " ";
    }
    std::cout << "\n";
  }
}
