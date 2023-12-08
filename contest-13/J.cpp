#include <algorithm>
#include <cmath>
#include <iostream>
#include <vector>
std::vector<std::pair<const double, const double>> coef = {
    {1.5, -2.3}, {444, 57}, {-239, 7}};  // NOLINT
struct Point {
  double x_pos;
  double y_pos;
  Point() : x_pos(0), y_pos(0) {}
  Point(double x_pos, double y_pos) : x_pos(x_pos), y_pos(y_pos) {}
  // Point(double x_pos, double y_pos) : x_pos(x_pos), y_pos(y_pos) {}
  double GetSquareOfDistance(const Point& p1) const {
    return (p1.x_pos - x_pos) * (p1.x_pos - x_pos) +
           (p1.y_pos - y_pos) * (p1.y_pos - y_pos);
  }
  Point operator-(const Point& p1) const {
    return Point(x_pos - p1.x_pos, y_pos - p1.y_pos);
  }
  bool operator<(const Point& p1) const {
    return x_pos < p1.x_pos || (x_pos == p1.x_pos && y_pos < p1.y_pos);
  }
  bool operator==(const Point& p1) const {
    return std::abs(x_pos - p1.x_pos) == 0 && std::abs(y_pos - p1.y_pos) == 0;
  }
};

std::ostream& operator<<(std::ostream& os, const std::vector<Point>& data) {
  os << "------\n";
  for (auto point : data) {
    os << "(" << point.x_pos << ", " << point.y_pos << ")\n";
  }
  os << "------\n";
  return os;
}

Point begin;

struct Vector {
  double x_len;
  double y_len;
  Vector(double x_len, double y_len) : x_len(x_len), y_len(y_len) {}
  // Vector(double x_len, double y_len) : x_len(x_len), y_len(y_len) {}
  Vector(Point p1, Point p2) : x_len((p2 - p1).x_pos), y_len((p2 - p1).y_pos) {}
  double operator*(const Vector& v1) const {
    return x_len * v1.y_len - v1.x_len * y_len;
  }
  double operator^(const Vector& v1) const {
    return x_len * y_len + v1.x_len * v1.y_len;
  }
  double GetLength() const { return sqrt(x_len * x_len + y_len * y_len); }
  double GetAngleToOx() const {
    Vector ox(1, 0);
    double sin_angle = -(ox * (*this)) / GetLength();
    return asin(sin_angle);
  }
};

bool SortByAngleCmp(const Point& p1, const Point& p2) {
  if (p2 == begin) {
    return false;
  }
  if (p1 == begin) {
    return true;
  }
  Vector v1 = Vector(begin, p1);
  Vector v2 = Vector(begin, p2);
  if (v1 * v2 == 0) {
    return begin.GetSquareOfDistance(p1) < begin.GetSquareOfDistance(p2);
  }
  return v1 * v2 > 0;
}

class Polygon {
 public:
  Polygon() {}
  Polygon(std::vector<Point>& points) : points_(points) {}
  size_t GetMinPointIndex() const {
    size_t min_point_index = 0;
    for (size_t i = 1; i < points_.size(); ++i) {
      if (points_[i] < points_[min_point_index]) {
        min_point_index = i;
      }
    }
    return min_point_index;
  }
  Point GetMinPoint() const {
    Point min_point = points_[GetMinPointIndex()];
    return min_point;
  }
  double GetDoubledSquare() const {
    Point random_point = Point(0LL, 0LL);
    double square = 0;
    for (size_t i = 0; i + 1 < GetVerticesNum(); ++i) {
      Vector v1 = Vector(random_point, points_[i]);
      Vector v2 = Vector(random_point, points_[i + 1]);
      square += v1 * v2;
    }
    Vector v1 = Vector(random_point, points_[GetVerticesNum() - 1]);
    Vector v2 = Vector(random_point, points_[0]);
    square += v1 * v2;
    return std::abs(square);
  }
  Polygon GetMBO() const {
    begin = GetMinPoint();
    std::vector<Point> points;
    points.insert(points.begin(), points_.begin(), points_.end());
    std::sort(points.begin(), points.end(), SortByAngleCmp);
    std::vector<Point> ans_points = {points[0], points[1]};
    for (size_t i = 2; i < GetVerticesNum(); ++i) {
      while (ans_points.size() >= 2 &&
             Vector(ans_points[ans_points.size() - 2],
                    ans_points[ans_points.size() - 1]) *
                     Vector(ans_points[ans_points.size() - 1], points[i]) <=
                 0) {
        ans_points.pop_back();
        // std::cout << ans_points;
        // std::cout << "KEKKKKK\n\n";
      }
      ans_points.push_back(points[i]);
      // std::cout << "LMAO\n";
      // std::cout << ans_points;
    }
    Polygon ans_poly(ans_points);
    return ans_poly;
  }
  bool ContainsPoint(const Point& point) const {
    Vector v_base(points_[0], point);
    Vector v1(points_[0], points_[1]);
    Vector v2(points_[0], points_[points_.size() - 1]);
    if (v_base * v2 < 0 || v_base * v1 > 0) {
      return false;
    }
    size_t left_bound = 1;
    size_t right_bound = points_.size() - 1;
    while (right_bound - left_bound > 1) {
      size_t middle_bound = (right_bound + left_bound) / 2;
      v1 = Vector(points_[0], points_[middle_bound]);
      if (v_base * v1 <= 0) {
        left_bound = middle_bound;
      } else {
        right_bound = middle_bound;
      }
    }
    v_base = Vector(points_[left_bound], points_[left_bound + 1]);
    v1 = Vector(points_[left_bound], points_[0]);
    v2 = Vector(points_[left_bound], point);
    return ((v_base * v1) * (v_base * v2) >= 0);
  }
  Point operator[](size_t index) const {
    return points_[index % points_.size()];
  }
  std::vector<Point> GetPoints() const { return points_; }
  size_t GetVerticesNum() const { return points_.size(); }

 private:
  std::vector<Point> points_;
};

Point operator+(const Point& p1, const Point& p2) {
  Point result(p1.x_pos + p2.x_pos, p1.y_pos + p2.y_pos);
  return result;
}

Point operator+(const Point& point, const Vector& vector) {
  Point result(point.x_pos + vector.x_len, point.y_pos + vector.y_len);
  return result;
}

Polygon operator+(const Polygon& polygon1, const Polygon& polygon2) {
  size_t cur_point_ind1 = polygon1.GetMinPointIndex();
  size_t cur_point_ind2 = polygon2.GetMinPointIndex();
  size_t passed1 = 1;
  size_t passed2 = 1;
  size_t vertices_num1 = polygon1.GetVerticesNum();
  size_t vertices_num2 = polygon2.GetVerticesNum();
  std::vector<Point> ans_points = {polygon1[cur_point_ind1] +
                                   polygon2[cur_point_ind2]};
  for (size_t i = 0; i < vertices_num1 + vertices_num2; ++i) {
    Vector v1(polygon1[cur_point_ind1], polygon1[cur_point_ind1 + 1]);
    Vector v2(polygon2[cur_point_ind2], polygon2[cur_point_ind2 + 1]);
    //    double angle1 = v1.GetAngleToOx();
    //    double angle2 = v2.GetAngleToOx();
    if (v1 * v2 >= 0 && passed1 < vertices_num1 + 1) {
      ++cur_point_ind1;
      ++passed1;
    }
    if (v1 * v2 <= 0 && passed2 < vertices_num2 + 1) {
      ++cur_point_ind2;
      ++passed2;
    }
    if (ans_points[ans_points.size() - 1] ==
        polygon1[cur_point_ind1] + polygon2[cur_point_ind2]) {
      continue;
    }
    ans_points.push_back(polygon1[cur_point_ind1] + polygon2[cur_point_ind2]);
    //    if (passed2 >= vertices_num2 || (angle1 > angle2 && passed1 <
    //    vertices_num1)) {
    //      ans_points.push_back(ans_points[ans_points.size() - 1] + v1);
    //      ++cur_point_ind1;
    //      ++passed1;
    //    } else if (passed2 < vertices_num2) {
    //      ans_points.push_back(ans_points[ans_points.size() - 1] + v2);
    //      ++cur_point_ind2;
    //      ++passed2;
    //    }
    // std::cout << cur_point_ind1 << " " << cur_point_ind2 << " CURRENT\n";
  }
  ans_points.pop_back();
  Polygon result(ans_points);
  return result;
}

std::istream& operator>>(std::istream& is, Point& point) {
  double x_pos;
  double y_pos;
  is >> x_pos >> y_pos;
  point = Point(x_pos, y_pos);
  return is;
}

std::istream& operator>>(std::istream& is, Polygon& polygon) {
  size_t vertices_num;
  is >> vertices_num;
  std::vector<Point> points;
  for (size_t i = 0; i < vertices_num; ++i) {
    Point point;
    is >> point;
    points.push_back(point);
  }
  polygon = Polygon(points);
  return is;
}

class Map {
 public:
  Map(const Polygon& polygon1, const Polygon& polygon2, const Polygon& polygon3)
      : polygon1_(polygon1), polygon2_(polygon2), polygon3_(polygon3) {}
  void Build() { evolve_area_ = polygon1_ + polygon2_ + polygon3_; }
  std::string CanMagnemiteEvolve(const Point& point) const {
    Point new_point = Point(point.x_pos * 3, point.y_pos * 3);
    if (evolve_area_.ContainsPoint(new_point)) {
      return "YES\n";
    }
    return "NO\n";
  }

 private:
  Polygon polygon1_;
  Polygon polygon2_;
  Polygon polygon3_;
  Polygon evolve_area_;
};

int main() {
  Polygon polygon1;
  Polygon polygon2;
  Polygon polygon3;
  std::cin >> polygon1 >> polygon2 >> polygon3;
  Polygon res = polygon1 + polygon2 + polygon3;
  Map map(polygon1, polygon2, polygon3);
  map.Build();
  size_t queries_num;
  std::cin >> queries_num;
  for (size_t i = 0; i < queries_num; ++i) {
    Point query_point;
    std::cin >> query_point;
    std::cout << map.CanMagnemiteEvolve(query_point);
  }
}
