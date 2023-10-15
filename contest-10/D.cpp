// Нужно узнать, сколько существует потенциальных позиций МФТИ на
// карте Москвы (прямоугольник размера n×m) и области, то есть таких
// прямоугольников размера  a×b, что при наложении на него карты МФТИ,
// совпадут условные обозначения во всех клетках, кроме может быть
// одной.

#include <cstring>
#include <iostream>
#include <queue>
#include <set>

template <typename T>
void PrintVector(std::vector<T>& vec) {
  for (size_t i = 0; i < vec.size(); ++i) {
    std::cout << vec[i] << " ";
  }
  std::cout << "\n";
}

const size_t kAlphSize = 26;
const int kAlphabet[kAlphSize] = {0,  1,  2,  3,  4,  5,  6,  7,  8,
                                  9,  10, 11, 12, 13, 14, 15, 16, 17,
                                  18, 19, 20, 21, 22, 23, 24, 25};
std::vector<std::vector<int>> possible_match;
int threshold = 0;
int match_num = 0;

struct Node {
  int to[kAlphSize];
  bool term;
  int sufflink;
  std::vector<int> term_ind;
  int link;
  int go[kAlphSize];
  int cnt = 0;
  std::vector<size_t> cnt_indexes = {};
  Node() {
    memset(to, -1, sizeof(to));
    memset(go, -1, sizeof(go));
    term = false;
    cnt = 0;
  }
};
struct Trie {
  std::vector<Node> bor;
  size_t patterns_num = 0;
  void Add(std::string& str, int ind) {
    ++patterns_num;
    if (bor.empty()) {
      bor.push_back(Node());
    }
    int vertex = 0;
    for (size_t i = 0; i < str.length(); ++i) {
      if (bor[vertex].to[str[i] - 'a'] == -1) {
        bor[vertex].to[str[i] - 'a'] = bor.size();
        bor.push_back(Node());
      }
      vertex = bor[vertex].to[str[i] - 'a'];
    }
    bor[vertex].term = true;
    bor[vertex].term_ind.push_back(ind);
  }
  void CntIndexesUpdate(int vertex_v) {
    if (bor[vertex_v].term) {
      for (auto elem : bor[vertex_v].term_ind) {
        bor[vertex_v].cnt_indexes.push_back(elem);
      }
    }
    if (bor[vertex_v].link != -1) {
      for (auto elem : bor[bor[vertex_v].link].cnt_indexes) {
        bor[vertex_v].cnt_indexes.push_back(elem);
      }
    }
  }

  void Build() {
    bor[0].link = -1;
    for (auto character : kAlphabet) {
      if (bor[0].to[character] != -1) {
        bor[0].go[character] = bor[0].to[character];
      } else {
        bor[0].go[character] = 0;
      }
    }
    std::queue<int> vertices_queue;
    vertices_queue.push(0);
    while (!vertices_queue.empty()) {
      int vertex_v = vertices_queue.front();
      vertices_queue.pop();
      CntIndexesUpdate(vertex_v);
      bor[vertex_v].cnt =
          (bor[vertex_v].term ? 1 : 0) +
          (bor[vertex_v].link == -1 ? 0 : bor[bor[vertex_v].link].cnt);
      for (auto character1 : kAlphabet) {
        int vertex_u = bor[vertex_v].to[character1];
        if (vertex_u == -1) {
          continue;
        }
        bor[vertex_u].link =
            (vertex_v == 0 ? 0 : bor[bor[vertex_v].link].go[character1]);
        for (auto character2 : kAlphabet) {
          if (bor[vertex_u].to[character2] != -1) {
            bor[vertex_u].go[character2] = bor[vertex_u].to[character2];
          } else {
            bor[vertex_u].go[character2] =
                bor[bor[vertex_u].link].go[character2];
          }
        }
        vertices_queue.push(vertex_u);
      }
    }
  }

  void FindAllMatches(std::string& text, size_t pos, size_t treshold,
                      size_t shift, std::string& type) {
    int vertex = 0;
    for (size_t i = 0; i < text.length(); ++i) {
      vertex = bor[vertex].go[text[i] - 'a'];
      for (auto ind : bor[vertex].cnt_indexes) {
        if (pos + patterns_num > treshold + ind ||
            ind >= std::min(pos + 1, patterns_num)) {
          continue;
        }
        if (type == "rows") {
          ++possible_match[pos - ind][i + 1 - shift];
          if (possible_match[pos - ind][i + 1 - shift] == threshold) {
            ++match_num;
          }
        }
        if (type == "columns") {
          ++possible_match[i + 1 - shift][pos - ind];
          if (possible_match[i + 1 - shift][pos - ind] == threshold) {
            ++match_num;
          }
        }
      }
    }
  }
};

size_t CountMatchesNum(std::vector<std::vector<int>>& possible_match,
                       size_t treshold) {
  size_t match_num = 0;
  for (size_t i = 0; i < possible_match.size(); ++i) {
    for (size_t elem : possible_match[i]) {
      if (elem + 2 >= treshold) {
        ++match_num;
      }
    }
  }
  return match_num;
}

void Katya(std::vector<std::string>& moscow_rows,
           std::vector<std::string>& moscow_columns,
           std::vector<std::string>& mipt_rows,
           std::vector<std::string>& mipt_columns) {
  Trie trie_rows;
  Trie trie_columns;
  for (size_t i = 0; i < mipt_rows.size(); ++i) {
    trie_rows.Add(mipt_rows[i], i);
  }
  for (size_t i = 0; i < mipt_columns.size(); ++i) {
    trie_columns.Add(mipt_columns[i], i);
  }
  trie_rows.Build();
  trie_columns.Build();
  possible_match.resize(moscow_rows.size(),
                        std::vector<int>(moscow_columns.size(), 0));
  std::string type = "rows";
  for (size_t i = 0; i < moscow_rows.size(); ++i) {
    trie_rows.FindAllMatches(moscow_rows[i], i, moscow_rows.size(),
                             mipt_columns.size(), type);
  }
  type = "columns";
  for (size_t i = 0; i < moscow_columns.size(); ++i) {
    trie_columns.FindAllMatches(moscow_columns[i], i, moscow_columns.size(),
                                mipt_rows.size(), type);
  }
}

int main() {
  std::ios::sync_with_stdio(false);
  std::cin.tie(0);
  std::cout.tie(0);
  size_t moscow_map_height;
  size_t moscow_map_width;
  std::cin >> moscow_map_height >> moscow_map_width;
  std::vector<std::string> moscow_rows(moscow_map_height);
  std::vector<std::string> moscow_columns(moscow_map_width);
  for (size_t i = 0; i < moscow_map_height; ++i) {
    std::string row;
    std::cin >> row;
    moscow_rows[i] = row;
    for (size_t jj = 0; jj < moscow_map_width; ++jj) {
      moscow_columns[jj] += row[jj];
    }
  }

  size_t mipt_map_height;
  size_t mipt_map_width;
  std::cin >> mipt_map_height >> mipt_map_width;
  threshold = mipt_map_height + mipt_map_width - 2;
  if (threshold == 0) {
    std::cout << moscow_map_height * moscow_map_width;
    return 0;
  }
  std::vector<std::string> mipt_rows(mipt_map_height);
  std::vector<std::string> mipt_columns(mipt_map_width);
  for (size_t i = 0; i < mipt_map_height; ++i) {
    std::string row;
    std::cin >> row;
    mipt_rows[i] = row;
    for (size_t jj = 0; jj < mipt_map_width; ++jj) {
      mipt_columns[jj] += row[jj];
    }
  }
  Katya(moscow_rows, moscow_columns, mipt_rows, mipt_columns);
  std::cout << match_num;
}
