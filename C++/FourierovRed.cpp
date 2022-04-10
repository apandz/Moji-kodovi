#include <algorithm>
#include <cmath>
#include <functional>
#include <iomanip>
#include <iostream>
#include <utility>
#include <vector>

using namespace std;

class FourierovRed {
  double t;
  double *a = nullptr, *b = nullptr;
  int st;

public:
  FourierovRed(double T, initializer_list<double> aa,
               initializer_list<double> bb)
      : t(T) {
    if ((aa.size() == 0 || aa.size() == 1) && bb.size() == 0) {
      throw domain_error("Stepen mora biti pozitivan");
    }
    if (aa.size() > bb.size()) {
      st = aa.size() - 1;
    } else {
      st = bb.size();
    }
    try {
      a = new double[st + 1]{};
      try {
        b = new double[st + 1]{};
        copy(aa.begin(), aa.end(), a);
        copy(bb.begin(), bb.end(), b + 1);
      } catch (...) {
        delete[] a;
        throw;
      }
    } catch (...) {
      throw;
    }
  }
  FourierovRed(int n, double T, function<double(int)> f1,
               function<double(int)> f2)
      : t(T), st(n) {
    if (n <= 0)
      throw domain_error("Stepen mora biti pozitivan");
    try {
      a = new double[n + 1]{};
      try {
        b = new double[n + 1]{};
        a[0] = f1(0);
        for (int i = 1; i <= n; i++) {
          a[i] = f1(i);
          b[i] = f2(i);
        }
      } catch (...) {
        delete[] a;
        throw;
      }
    } catch (...) {
      throw;
    }
  }
  FourierovRed(int n, double p, double q, function<double(double)> f, int M) {
    if (n <= 0)
      throw domain_error("Stepen mora biti pozitivan");
    if (M <= 0)
      throw domain_error("Broj podintervala mora biti pozitivan");
    st = n;
    const double eps = 1e-10;
    if (p - q >= eps * (p + q))
      throw range_error("Neispravan interval");
    const double pi = atan(1) * 4;
    t = q - p;
    try {
      a = new double[n + 1]{};
      try {
        b = new double[n + 1]{};
        for (int i = 0; i <= n; i++) {
          double x = (f(p) + f(q)) * cos(2 * i * pi * p * 1. / t) * 1. / M;
          for (int j = 1; j < M; j++) {
            x += 2 * f(p + j * t * 1. / M) *
                 cos(2 * i * pi * (p * 1. / t + j * 1. / M)) * 1. / M;
          }
          a[i] = x;
        }
        for (int i = 1; i <= n; i++) {
          double x = (f(p) + f(q)) * sin(2 * i * pi * p * 1. / t) * 1. / M;
          for (int j = 1; j < M; j++) {
            x += 2 * f(p + j * t * 1. / M) *
                 sin(2 * i * pi * (p * 1. / t + j * 1. / M)) * 1. / M;
          }
          b[i] = x;
        }
      } catch (...) {
        delete[] a;
        throw;
      }
    } catch (...) {
      throw;
    }
  }
  FourierovRed(const FourierovRed &f);
  FourierovRed(FourierovRed &&f);
  ~FourierovRed() {
    delete[] a;
    delete[] b;
  }
  double operator()(double x);
  pair<double, double> operator[](int k) const;
  pair<double &, double &> operator[](int k);
  FourierovRed &operator=(const FourierovRed &f);
  FourierovRed &operator=(FourierovRed &&f);
};

FourierovRed::FourierovRed(const FourierovRed &f) {
  st = f.st;
  t = f.t;
  try {
    a = new double[f.st + 1]{};
    try {
      b = new double[f.st + 1]{};
      copy(f.a, f.a + f.st + 1, a);
      copy(f.b, f.b + f.st + 1, b);
    } catch (...) {
      delete[] a;
      throw;
    }
  } catch (...) {
    throw;
  }
}

FourierovRed::FourierovRed(FourierovRed &&f) {
  st = f.st;
  t = f.t;
  a = f.a;
  b = f.b;
  f.a = nullptr;
  f.b = nullptr;
  f.t = 0;
  f.st = 0;
}

double FourierovRed::operator()(double x) {
  double br = a[0] / 2.;
  const double pi = 4 * atan(1);
  for (int i = 1; i < st + 1; i++) {
    br += a[i] * cos(2 * i * pi * x / t) + b[i] * sin(2 * i * pi * x / t);
  }
  return br;
}

pair<double, double> FourierovRed::operator[](int k) const {
  if (k < 0 || k > st) {
    throw range_error("Neispravan indeks");
  }
  return {a[k], b[k]};
}

pair<double &, double &> FourierovRed::operator[](int k) {
  if (k < 0 || k > st) {
    throw range_error("Neispravan indeks");
  }
  return {a[k], b[k]};
}

FourierovRed &FourierovRed::operator=(const FourierovRed &f) {
  if (a != nullptr) {
    delete[] a;
  }
  if (b != nullptr) {
    delete[] b;
  }
  st = f.st;
  t = f.t;
  try {
    a = new double[f.st + 1]{};
    try {
      b = new double[f.st + 1]{};
      copy(f.a, f.a + f.st + 1, a);
      copy(f.b, f.b + f.st + 1, b);
    } catch (...) {
      delete[] a;
      throw;
    }
  } catch (...) {
    throw;
  }
  return *this;
}

FourierovRed &FourierovRed::operator=(FourierovRed &&f) {
  if (a != nullptr) {
    delete[] a;
  }
  if (b != nullptr) {
    delete[] b;
  }
  st = f.st;
  t = f.t;
  a = f.a;
  b = f.b;
  f.a = nullptr;
  f.b = nullptr;
  f.t = 0;
  f.st = 0;
  return *this;
}

int main() {
  const double pi = 4 * atan(1);
  FourierovRed f(
      65, 2 * pi,
      [pi](int x) {
        if (x == 0) {
          return 8 * pi * pi / 3;
        }
        return 4. / (x * x);
      },
      [pi](int x) { return -4 * pi / x; });
  FourierovRed f1(
      65, 0, 2 * pi, [](int x) { return x * x; }, 4);
  cout << f(3) << " " << f1(3);
  return 0;
}
