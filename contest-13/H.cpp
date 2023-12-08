#include <algorithm>
#include <cmath>
#include <iostream>
#include <vector>

struct Point {
  long long x_pos;
  long long y_pos;
  Point() : x_pos(0), y_pos(0) {}
  Point(long long x_pos, long long y_pos) : x_pos(x_pos), y_pos(y_pos) {}
  // Point(long long x_pos, long long y_pos) : x_pos(x_pos), y_pos(y_pos) {}
  long long GetSquareOfDistance(const Point& p1) const {
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
  long long x_len;
  long long y_len;
  Vector(long long x_len, long long y_len) : x_len(x_len), y_len(y_len) {}
  // Vector(long long x_len, long long y_len) : x_len(x_len), y_len(y_len) {}
  Vector(Point p1, Point p2) : x_len((p2 - p1).x_pos), y_len((p2 - p1).y_pos) {}
  long long operator*(const Vector& v1) const {
    return x_len * v1.y_len - v1.x_len * y_len;
  }
  long long operator^(const Vector& v1) const {
    return x_len * y_len + v1.x_len * v1.y_len;
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
  Point GetMinPoint() const {
    Point min_point = points_[0];
    for (size_t i = 1; i < points_.size(); ++i) {
      if (points_[i] < min_point) {
        min_point = points_[i];
      }
    }
    return min_point;
  }
  long long GetDoubledSquare() const {
    Point random_point = Point(0LL, 0LL);
    long long square = 0;
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
  std::vector<Point> GetPoints() const { return points_; }
  size_t GetVerticesNum() const { return points_.size(); }

 private:
  std::vector<Point> points_;
};

std::string FormatAns(long long doubled_square) {
  if (doubled_square % 2 == 0) {
    return std::to_string(doubled_square / 2) + ".0";
  }
  return std::to_string(doubled_square / 2) + ".5";
}

int main() {
  long long num;
  std::cin >> num;
  std::vector<Point> points;
  long long x_pos;
  long long y_pos;
  std::cin >> x_pos >> y_pos;
  points.push_back(Point(x_pos, y_pos));
  for (long long i = 1; i < num; ++i) {
    std::cin >> x_pos >> y_pos;
    if (Point(x_pos, y_pos) == points[points.size() - 1]) {
      continue;
    }
    points.push_back(Point(x_pos, y_pos));
  }
  Polygon polygon(points);
  Polygon ans = polygon.GetMBO();
  std::vector<Point> mbo_points = ans.GetPoints();
  std::cout << mbo_points.size() << "\n";
  std::cout << mbo_points[0].x_pos << " " << mbo_points[0].y_pos << "\n";
  for (size_t i = 1; i < mbo_points.size(); ++i) {
    std::cout << mbo_points[i].x_pos << " " << mbo_points[i].y_pos << "\n";
  }
  std::cout << FormatAns(ans.GetDoubledSquare());
}
