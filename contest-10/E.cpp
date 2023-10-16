#include <cstring>
#include <iostream>
#include <queue>
#include <set>
#include <vector>

const size_t kAlphSize = 10;
const int kAlphabet[kAlphSize] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9};

struct Node {
  int to[kAlphSize];
  bool term;
  int link;
  int go[kAlphSize];
  int cnt = 0;
  size_t height = 0;

  Node() {
    memset(to, -1, sizeof(to));
    memset(go, -1, sizeof(go));
    term = false;
    cnt = 0;
    height = 0;
  }
};
class Trie {
  std::vector<Node> trie_;
  void CntUpdate(int vertex) {
    for (auto character : kAlphabet) {
      if (trie_[vertex].to[character] == -1) {
        continue;
      }
      CntUpdate(trie_[vertex].to[character]);
      trie_[vertex].cnt += trie_[trie_[vertex].to[character]].cnt;
    }
  }

 public:
  void AddString(std::string str) {
    if (trie_.empty()) {
      trie_.push_back(Node());
    }
    int vertex = 0;
    for (size_t i = 0; i < str.length(); ++i) {
      if (trie_[vertex].to[str[i] - '0'] == -1) {
        trie_[vertex].to[str[i] - '0'] = trie_.size();
        trie_.push_back(Node());
        trie_[trie_.size() - 1].height = trie_[vertex].height + 1;
      }
      vertex = trie_[vertex].to[str[i] - '0'];
    }
    trie_[vertex].term = true;
    trie_[vertex].cnt = 1;
  }

  void Build() {
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
    CntUpdate(0);
  }
  std::vector<int> FindSumCnt(int group_size) {
    std::vector<int> groups = {};
    std::queue<int> vertices_queue;
    vertices_queue.push(0);
    while (!vertices_queue.empty()) {
      int vertex_v = vertices_queue.front();
      vertices_queue.pop();
      if (trie_[vertex_v].height % 2 == 0) {
        if (groups.size() != (trie_[vertex_v].height / 2 + 1)) {
          groups.push_back(0);
        }
        if (trie_[vertex_v].cnt >= group_size) {
          groups[groups.size() - 1] += 1;
        }
      }
      for (auto character1 : kAlphabet) {
        int vertex_u = trie_[vertex_v].to[character1];
        if (vertex_u == -1) {
          continue;
        }
        vertices_queue.push(vertex_u);
      }
    }
    return groups;
  }
};

std::vector<std::string> SnilsesRefactor(std::vector<std::string> snilses) {
  std::vector<std::string> new_snilses;
  for (std::string snils : snilses) {
    std::string new_snils;
    for (size_t i = 0; i < snils.length(); ++i) {
      new_snils += snils[i];
      new_snils += snils[snils.length() - i - 1];
    }
    new_snilses.push_back(new_snils);
  }
  return new_snilses;
}

std::vector<int> FindGroups(int group_size, std::vector<std::string> snilses) {
  Trie trie;

  std::vector<std::string> new_snilses = SnilsesRefactor(snilses);
  for (std::string new_snils : new_snilses) {
    trie.AddString(new_snils);
  }
  trie.Build();
  std::vector<int> groups = trie.FindSumCnt(group_size);
  return groups;
}

int main() {
  std::ios::sync_with_stdio(false);
  std::cin.tie(0);
  std::cout.tie(0);
  size_t num;
  int group_size;
  std::cin >> num >> group_size;
  std::vector<std::string> snilses;
  for (size_t i = 0; i < num; ++i) {
    std::string snils;
    std::cin >> snils;
    snilses.push_back(snils);
  }

  std::vector<int> groups = FindGroups(group_size, snilses);

  size_t query_num;
  std::cin >> query_num;
  for (size_t i = 0; i < query_num; ++i) {
    int length;
    std::cin >> length;
    std::cout << groups[length] << "\n";
  }
}
