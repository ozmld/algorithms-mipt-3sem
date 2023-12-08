#include <cmath>
#include <iostream>
#include <vector>

const double kEps = 1e-5;

struct Complex {
  double re, im;

  Complex() : re(0), im(0) {}

  Complex(double re) : re(re), im(0) {}

  Complex(long long re) : re(re), im(0) {}

  Complex(int re) : re(re), im(0) {}

  Complex(double re, double im) : re(re), im(im) {}

  Complex(long long re, long long im) : re(re), im(im) {}

  bool operator==(const Complex& another) const {
    return std::abs(re - another.re) < kEps && std::abs(im - another.im) < kEps;
  }

  bool operator!=(const Complex& another) const { return !(*this == another); }

  Complex operator+(const Complex& another) const {
    return {re + another.re, im + another.im};
  }

  Complex& operator+=(const Complex& another) {
    re = re + another.re;
    im = im + another.im;
    return *this;
  }

  Complex operator-(const Complex& another) const {
    return {re - another.re, im - another.im};
  }

  Complex& operator-=(const Complex& another) {
    re = re - another.re;
    im = im - another.im;
    return *this;
  }

  Complex operator*(const Complex& another) const {
    return {re * another.re - im * another.im,
            re * another.im + im * another.re};
  }

  Complex& operator*=(const Complex& another) {
    re = re * another.re - im * another.im;
    im = re * another.im + im * another.re;
    return *this;
  }

  Complex& operator/=(long long del) {
    re /= del;
    im /= del;
    return *this;
  }

  Complex Conjugated() const { return {re, -im}; }
};

const double kPi = acos(-1);
const long long kMaxLg = 19;
const long long kMaxSize = 1 << kMaxLg;
long long rev_prep[kMaxSize];
Complex omegas[kMaxSize];

long long Rev(long long ii, long long lg) {
  long long res = 0;
  for (long long j = 0; j < lg; ++j) {
    if ((ii & (1 << j)) != 0) {
      res |= 1 << (lg - 1 - j);
    }
  }
  return res;
}

void CountRev(long long lg) {
  long long num = 1 << lg;
  for (long long i = 0; i < num; ++i) {
    rev_prep[i] = Rev(i, lg);
  }
}

void CountRootsFromOne(long long num) {
  for (long long i = 0; i < num; ++i) {
    double ang = 2 * kPi * i / num;
    omegas[i] = Complex(cos(ang), sin(ang));
  }
}

Complex GetOmega(long long coef, long long deg, bool inv, long long num) {
  return (inv ? omegas[num / deg * coef].Conjugated()
              : omegas[num / deg * coef]);
}

void FFT(Complex* arr, long long lg, bool reverse = false) {
  if (lg == 0) {
    return;
  }
  long long num = 1 << lg;
  for (long long i = 0; i < num; ++i) {
    long long revi = rev_prep[i];
    if (i < revi) {
      std::swap(arr[i], arr[revi]);
    }
  }
  for (long long len = 2; len <= num; len <<= 1) {
    for (long long ind = 0; ind < num; ind += len) {
      for (long long i = 0; i < len / 2; i++) {
        Complex omega = GetOmega(i, len, reverse, 1 << lg);
        Complex first_num = arr[ind + i];
        Complex second_num = arr[ind + i + len / 2] * omega;
        arr[ind + i] = first_num + second_num;
        arr[ind + i + len / 2] = first_num - second_num;
      }
    }
  }
  if (reverse) {
    for (long long i = 0; i < num; ++i) {
      arr[i] /= num;
    }
  }
}

long long UppperLog(long long num) {
  int lg = 0;
  while ((1 << lg) < num) {
    ++lg;
  }
  return lg;
}

long long Round(double num) {
  if (num >= 0) {
    return (long long)(num + 1. / 2);
  }
  return (long long)(num - 1. / 2);
}

void Multiply(Complex* result, Complex* first, Complex* second,
              long long log_k) {
  CountRev(log_k);
  CountRootsFromOne(1 << log_k);
  FFT(first, log_k);
  FFT(second, log_k);
  for (long long i = 0; i < (1 << log_k); ++i) {
    result[i] = first[i] * second[i];
  }
  FFT(result, log_k, true);
}

int main() {
  long long first_deg;
  std::cin >> first_deg;
  ++first_deg;
  Complex first[1 << kMaxLg];
  for (long long i = 0; i < first_deg; ++i) {
    long long coef;
    std::cin >> coef;
    first[i] = coef;
  }
  long long second_deg;
  std::cin >> second_deg;
  ++second_deg;
  Complex second[1 << kMaxLg];
  for (long long i = 0; i < second_deg; ++i) {
    long long coef;
    std::cin >> coef;
    second[i] = coef;
  }
  long long log_k = std::max(0LL, UppperLog(first_deg + second_deg) + 1);
  Complex result[1 << kMaxLg];
  Multiply(result, first, second, log_k);
  std::cout << (first_deg - 1) + (second_deg - 1) << " ";
  for (long long i = 0; i < (first_deg - 1) + (second_deg - 1) + 1; ++i) {
    std::cout << Round(result[i].re) << " ";
  }
}
