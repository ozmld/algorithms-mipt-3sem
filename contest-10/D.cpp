// Нужно узнать, сколько существует потенциальных позиций МФТИ на
// карте Москвы (прямоугольник размера n×m) и области, то есть таких
// прямоугольников размера  a×b, что при наложении на него карты МФТИ,
// совпадут условные обозначения во всех клетках, кроме может быть
// одной.

#include <cstring>
#include <iostream>
#include <queue>
#include <set>

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
class Trie {
  std::vector<Node> trie_;
  size_t patterns_num_ = 0;
  void CntIndexesUpdate(int vertex_v) {
    if (trie_[vertex_v].term) {
      for (auto elem : trie_[vertex_v].term_ind) {
        trie_[vertex_v].cnt_indexes.push_back(elem);
      }
    }
    if (trie_[vertex_v].link != -1) {
      for (auto elem : trie_[trie_[vertex_v].link].cnt_indexes) {
        trie_[vertex_v].cnt_indexes.push_back(elem);
      }
    }
  }

 public:
  Trie() { trie_ = {Node()}; }
  void AddString(std::string& str, int ind) {
    ++patterns_num_;
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
    trie_[vertex].term_ind.push_back(ind);
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
  void FindAllMatches(std::string& text, size_t pos, size_t bound, size_t shift,
                      std::string& type) {
    int vertex = 0;
    for (size_t i = 0; i < text.length(); ++i) {
      vertex = trie_[vertex].go[text[i] - 'a'];
      for (auto ind : trie_[vertex].cnt_indexes) {
        if (pos + patterns_num_ > bound + ind ||
            ind >= std::min(pos + 1, patterns_num_)) {
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

void CountMapMatches(std::vector<std::string>& moscow_rows,
                     std::vector<std::string>& moscow_columns,
                     std::vector<std::string>& mipt_rows,
                     std::vector<std::string>& mipt_columns) {
  threshold = mipt_rows.size() + mipt_columns.size() - 2;
  if (threshold == 0) {
    match_num = moscow_rows.size() * moscow_columns.size();
    return;
  }
  Trie trie_rows;
  Trie trie_columns;
  for (size_t i = 0; i < mipt_rows.size(); ++i) {
    trie_rows.AddString(mipt_rows[i], i);
  }
  for (size_t i = 0; i < mipt_columns.size(); ++i) {
    trie_columns.AddString(mipt_columns[i], i);
  }
  trie_rows.BuildTrie();
  trie_columns.BuildTrie();
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
  CountMapMatches(moscow_rows, moscow_columns, mipt_rows, mipt_columns);
  std::cout << match_num;
}
