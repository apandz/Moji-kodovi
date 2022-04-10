#include <algorithm>
#include <cmath>
#include <iostream>
#include <limits>
#include <vector>

#define eps 1e-10

class AbstractInterpolator {
  mutable int xi;

protected:
  int Locate(double x) const;
  std::vector<std::pair<double, double>> data;

public:
  AbstractInterpolator(const std::vector<std::pair<double, double>> &data);
  virtual double operator()(double x) const = 0;
};

AbstractInterpolator::AbstractInterpolator(
    const std::vector<std::pair<double, double>> &data)
    : xi(0) {
  std::vector<int> v;
  for (int i = 0; i < data.size(); i++) {
    v.push_back(i);
  }
  sort(v.begin(), v.end(), [&data](int a, int b) -> bool {
    if (std::fabs(data.at(a).first - data.at(b).first) <=
        eps * (std::fabs(data.at(a).first) + std::fabs(data.at(b).first)))
      throw std::domain_error("Invalid data set");
    return data.at(a).first < data.at(b).first;
  });
  this->data.resize(data.size());
  for (int i = 0; i < data.size(); i++) {
    this->data.at(i) = data.at(v.at(i));
  }
}

int AbstractInterpolator::Locate(double x) const {
  if (std::fabs(data.at(0).first - x) <=
          eps * (std::fabs(data.at(0).first) + std::fabs(x)) ||
      data.at(0).first > x) {
    return 0;
  }
  if (data.at(data.size() - 1).first < x) {
    return data.size();
  }
  if (data.at(xi).first < x && x <= data.at(xi + 1).first)
    return xi + 1;
  if (xi > 0) {
    if (data.at(xi - 1).first < x && x <= data.at(xi).first)
      return xi;
  }
  if (xi + 1 < data.size()) {
    if (data.at(xi + 1).first < x && x <= data.at(xi + 2).first)
      return xi + 2;
  }
  xi = std::lower_bound(data.begin(), data.end(), x,
                        [](std::pair<double, double> p1, double x1) -> bool {
                          return p1.first < x1;
                        }) -
       data.begin() - 1;
  return xi + 1;
}

class LinearInterpolator : public AbstractInterpolator {
public:
  LinearInterpolator(const std::vector<std::pair<double, double>> &data);
  double operator()(double x) const override;
};

LinearInterpolator::LinearInterpolator(
    const std::vector<std::pair<double, double>> &data)
    : AbstractInterpolator(data) {}

double LinearInterpolator::operator()(double x) const {
  int i = Locate(x);
  double y = 0;
  auto i_1 = data.at(1), i_ = data.at(0);
  if (i == data.size()) {
    i_1 = data.at(data.size() - 1);
    i_ = data.at(data.size() - 2);
  } else if (i != 0) {
    i_1 = data.at(i);
    i_ = data.at(i - 1);
  }
  y = (i_1.first - x) * i_.second / (i_1.first - i_.first) +
      (x - i_.first) * i_1.second / (i_1.first - i_.first);
  return y;
}

class PolynomialInterpolator : public AbstractInterpolator {
  std::vector<double> dd, q;

public:
  PolynomialInterpolator(const std::vector<std::pair<double, double>> &data);
  double operator()(double x) const override;
  void AddPoint(const std::pair<double, double> &p);
  std::vector<double> GetCoefficients() const;
};

PolynomialInterpolator::PolynomialInterpolator(
    const std::vector<std::pair<double, double>> &data)
    : AbstractInterpolator(data) {
  int n = data.size();
  for (int i = 0; i < n; i++) {
    q.push_back(data.at(i).second);
  }
  dd.push_back(q.at(0));
  for (int j = 1; j < n; j++) {
    for (int i = n; i > j; i--) {
      q.at(i - 1) =
          (q.at(i - 1) - q.at(i - 2)) /
          (this->data.at(i - 1).first - this->data.at(i - j - 1).first);
      if (i == n)
        dd.push_back(q.at(i - 1));
    }
  }
}

double PolynomialInterpolator::operator()(double x) const {
  double f = q.at(data.size() - 1);
  for (int i = data.size() - 1; i > 0; i--) {
    f = f * (x - data.at(i - 1).first) + q.at(i - 1);
  }
  return f;
}

void PolynomialInterpolator::AddPoint(const std::pair<double, double> &p) {
  for (int i = 0; i < data.size(); i++) {
    if (std::fabs(data.at(i).first - p.first) <=
        eps * (std::fabs(data.at(i).first) + std::fabs(p.first))) {
      throw std::domain_error("Invalid point");
    }
  }
  data.push_back(p);
  int n = data.size();
  q.push_back(p.second);
  for (int j = 1; j <= n - 1; j++) {
    if (j == 1)
      q.at(n - 1) =
          (q.at(n - 1) - data.at(n - 2).second) /
          (this->data.at(n - 1).first - this->data.at(n - j - 1).first);
    else {
      double x = dd.at(j - 1);
      dd.at(j - 1) = q.at(n - 1);
      q.at(n - 1) = (q.at(n - 1) - x) / (this->data.at(n - 1).first -
                                         this->data.at(n - j - 1).first);
    }
  }
  dd.push_back(q.at(n - 1));
}

std::vector<double> PolynomialInterpolator::GetCoefficients() const {
  int n = data.size();
  std::vector<double> p(n), w(n + 1), v(n + 1);
  w.at(0) = 1;
  for (int i = 1; i <= n; i++) {
    w.at(i) = w.at(i - 1);
    for (int j = i - 1; j >= 1; j--) {
      w.at(j) = w.at(j - 1) - data.at(i - 1).first * w.at(j);
    }
    w.at(0) *= -data.at(i - 1).first;
  }
  for (int i = 1; i <= n; i++) {
    double a = 1;
    for (int j = 1; j <= n; j++) {
      if (j != i) {
        a *= data.at(i - 1).first - data.at(j - 1).first;
      }
    }
    a = data.at(i - 1).second / a;
    for (int j = 0; j <= n; j++) {
      v.at(j) = w.at(j);
    }
    for (int j = n - 1; j >= 0; j--) {
      v.at(j) += data.at(i - 1).first * v.at(j + 1);
      p.at(j) += a * v.at(j + 1);
    }
  }
  return p;
}

class PiecewisePolynomialInterpolator : public AbstractInterpolator {
  int k;

public:
  PiecewisePolynomialInterpolator(
      const std::vector<std::pair<double, double>> &data, int order);
  double operator()(double x) const override;
};

PiecewisePolynomialInterpolator::PiecewisePolynomialInterpolator(
    const std::vector<std::pair<double, double>> &data, int order)
    : AbstractInterpolator(data) {
  if (order < 1 || order >= data.size()) {
    throw std::domain_error("Invalid order");
  }
  k = order;
}

double PiecewisePolynomialInterpolator::operator()(double x) const {
  int i = Locate(x), min = 0, max = 0, n = data.size();
  if (k % 2 == 0) {
    min = i - k / 2;
    max = i + k / 2;
    if (min < 1) {
      min = 1;
      max = k + 1;
    } else if (max > n) {
      max = n;
      min = max - k;
    }
  } else {
    min = i - (k - 1) / 2;
    max = i + (k + 1) / 2;
    if (min < 1) {
      min = 1;
      max = k + 1;
    } else if (max > n) {
      max = n;
      min = max - k;
    }
  }
  double s = 0;
  for (int l = min; l <= max; l++) {
    double p = data.at(l - 1).second;
    for (int j = min; j <= max; j++) {
      if (j != l) {
        p *= (x - data.at(j - 1).first) /
             (data.at(l - 1).first - data.at(j - 1).first);
      }
    }
    s += p;
  }
  return s;
}

class SplineInterpolator : public AbstractInterpolator {
  std::vector<double> r;

public:
  SplineInterpolator(const std::vector<std::pair<double, double>> &data);
  double operator()(double x) const override;
};

SplineInterpolator::SplineInterpolator(
    const std::vector<std::pair<double, double>> &data)
    : AbstractInterpolator(data) {
  int n = this->data.size();
  r.resize(n);
  std::vector<double> a(n - 1);
  for (int i = 1; i < n - 1; i++) {
    a.at(i) = 2 * (this->data.at(i + 1).first - this->data.at(i - 1).first);
    r.at(i) = 3 * ((this->data.at(i + 1).second - this->data.at(i).second) /
                       (this->data.at(i + 1).first - this->data.at(i).first) -
                   (this->data.at(i).second - this->data.at(i - 1).second) /
                       (this->data.at(i).first - this->data.at(i - 1).first));
  }
  for (int i = 1; i < n - 2; i++) {
    double mi = (this->data.at(i + 1).first - this->data.at(i).first) / a.at(i);
    a.at(i + 1) -= mi * (this->data.at(i + 1).first - this->data.at(i).first);
    r.at(i + 1) -= mi * r.at(i);
  }
  r.at(n - 2) /= a.at(n - 2);
  for (int i = n - 3; i > 0; i--) {
    r.at(i) = (r.at(i) - (this->data.at(i + 1).first - this->data.at(i).first) *
                             r.at(i + 1)) /
              a.at(i);
  }
}

double SplineInterpolator::operator()(double x) const {
  int i = Locate(x);
  if (i == 0) {
    i = 1;
  } else if (i == data.size()) {
    i--;
  }
  double t = x - data.at(i - 1).first;
  double dx = data.at(i).first - data.at(i - 1).first;
  double s = (r.at(i) - r.at(i - 1)) / (3 * dx);
  double q = (data.at(i).second - data.at(i - 1).second) / dx -
             dx * (r.at(i) + 2 * r.at(i - 1)) / 3.;
  return data.at(i - 1).second + t * (q + t * (r.at(i - 1) + t * s));
}

class BarycentricInterpolator : public AbstractInterpolator {
  int d;
  std::vector<double> w;

public:
  BarycentricInterpolator(const std::vector<std::pair<double, double>> &data,
                          int order);
  double operator()(double x) const override;
  std::vector<double> GetWeights() const;
};

BarycentricInterpolator::BarycentricInterpolator(
    const std::vector<std::pair<double, double>> &data, int order)
    : AbstractInterpolator(data) {
  if (order < 0 || order >= data.size()) {
    throw std::domain_error("Invalid order");
  }
  d = order;
  int n = this->data.size();
  w.resize(n);
  for (int i = 1; i <= n; i++) {
    int min = i, max = 1;
    if (i - d > max)
      max = i - d;
    if (n - d < min)
      min = n - d;
    double p = 0;
    for (int k = max; k <= min; k++) {
      p = 1;
      for (int j = k; j <= k + d; j++) {
        if (j != i) {
          p /= this->data.at(i - 1).first - this->data.at(j - 1).first;
        }
      }
      if (k % 2 == 0) {
        p *= -1;
      }
    }
    w.at(i - 1) += p;
  }
}

double BarycentricInterpolator::operator()(double x) const {
  double p = 0, q = 0;
  for (int i = 1; i <= data.size(); i++) {
    if (std::fabs(x - data.at(i - 1).first) <=
        eps * (std::fabs(x) + std::fabs(data.at(i - 1).first)))
      return data.at(i - 1).second;
    double u = w.at(i - 1) / (x - data.at(i - 1).first);
    p += u * data.at(i - 1).second;
    q += u;
  }
  return p / q;
}

std::vector<double> BarycentricInterpolator::GetWeights() const {
  std::vector<double> w1(w);
  return w1;
}

class TrigonometricInterpolator : public AbstractInterpolator {
  double o;

public:
  TrigonometricInterpolator(const std::vector<std::pair<double, double>> &data);
  double operator()(double x) const override;
};

TrigonometricInterpolator::TrigonometricInterpolator(
    const std::vector<std::pair<double, double>> &data)
    : AbstractInterpolator(data) {
  int n = this->data.size();
  o = this->data.at(n - 1).second - this->data.at(0).second;
  if (!(std::fabs(o) <= eps*(std::fabs(this->data.at(n - 1).second)+std::fabs(this->data.at(0).second)))) {
    throw std::domain_error("Function is not periodic");
  }
  o=std::fabs(this->data.at(0).second-this->data.at(1).second);
  o = (8 * std::atan(1)) / o;
}

double TrigonometricInterpolator::operator()(double x) const {
  int n = data.size();
  double T = 0;
  if (n % 2 == 0) {
    for (int k = 0; k < n - 1; k++) {
      double p = 1;
      for (int j = 0; j < n - 1; j++) {
        if (j != k) {
          p *= std::sin(o * (x - data.at(j).first) / 2.) /
               std::sin(o * (data.at(k).first - data.at(j).first) / 2.);
        }
      }
      T += data.at(k).second * p;
    }
  } else {
    for (int k = 0; k < n - 1; k++) {
      double p = 1;
      for (int j = 0; j < n - 1; j++) {
        if (j != k) {
          p *= std::sin(o * (x - data.at(j).first) / 2.) /
               std::sin(o * (data.at(k).first - data.at(j).first) / 2.);
        }
      }
      double a = 0;
      for (int j = 0; j < n - 1; j++) {
        if (j != k) {
          a += data.at(j).first;
        }
      }
      a *= -1;
      p *= std::sin(o * (x - a) / 2.) / std::sin(o * (data.at(k).first - a) / 2.);
      T += data.at(k).second * p;
    }
  }
  return T;
}

void test_linear_1() {
    //polinom 3x^3+2x^2+x+1
    std::vector<std::pair<double,double>> data;
    for(double i=-5; i<=5; i+=0.25) {
        data.push_back({i,3*i*i*i+2*i*i+i+1});
    }
    LinearInterpolator li(data);
    double x=4.35;
    std::cout << "LinearInterpolator test:\n    Expected output: "<<3*x*x*x+2*x*x+x+1;
    std::cout << "\n    Actual output: " << li(x) <<"\n";
}

void test_linear_2() {
    //polinom x^2+2x+1
    std::vector<std::pair<double,double>> data;
    for(double i=-5; i<=5; i+=0.25) {
        data.push_back({i,i*i+2*i+1});
    }
    LinearInterpolator li(data);
    double x=1.35;
    std::cout << "LinearInterpolator test:\n    Expected output: "<<x*x+2*x+1;
    std::cout << "\n    Actual output: " << li(x) <<"\n";
}

void test_linear_3() {
    //polinom x^3+4x^2+2x+1
    std::vector<std::pair<double,double>> data;
    for(double i=-5; i<=5; i+=0.25) {
        data.push_back({i,i*i*i+4*i*i+2*i+1});
    }
    LinearInterpolator li(data);
    double x=-5.25;
    std::cout << "LinearInterpolator test:\n    Expected output: "<<x*x*x+4*x*x+2*x+1;
    std::cout << "\n    Actual output: " << li(x) <<"\n";
}

void test_linear_exception_1() {
    std::vector<std::pair<double,double>> data({{1,2},{2,3},{2,4}});
    std::cout << "LinearInterpolator exception test:" << std::endl;
  std::cout << "    Expected output:\nInvalid data set\n";
  try {
    LinearInterpolator li(data);
  } catch (std::domain_error e) {
    std::cout << "    Actual output:\n" << e.what();
  }
  std::cout << std::endl;
}

void test_polynomial_1() {
    //polinom 3x^3+2x^2+x+1
    std::vector<std::pair<double,double>> data;
    double x=-5;
    data.push_back({x,3*x*x*x+2*x*x+x+1});
    x=0;
    data.push_back({x,3*x*x*x+2*x*x+x+1});
    x=2;
    data.push_back({x,3*x*x*x+2*x*x+x+1});
    x=6;
    data.push_back({x,3*x*x*x+2*x*x+x+1});
    PolynomialInterpolator pi(data);
    x=4.35;
    std::cout << "PolynomialInterpolator test:\n    Expected output: "<<3*x*x*x+2*x*x+x+1;
    std::cout << "\n    Actual output: " << pi(x) <<"\n";
}

void test_polynomial_2() {
    //polinom x^2+2x+1
    std::vector<std::pair<double,double>> data;
    double x=-4;
    data.push_back({x,x*x+2*x+1});
    x=0;
    data.push_back({x,x*x+2*x+1});
    x=-2;
    data.push_back({x,x*x+2*x+1});
    x=3;
    data.push_back({x,x*x+2*x+1});
    PolynomialInterpolator pi(data);
    x=1;
    pi.AddPoint({x,x*x+2*x+1});
    x=1.35;
    std::cout << "PolynomialInterpolator test:\n    Expected output: "<<x*x+2*x+1;
    std::cout << "\n    Actual output: " << pi(x) <<"\n";
}

void test_polynomial_3() {
    //polinom 3x^3+x^2+2x+1
    std::vector<std::pair<double,double>> data;
    double x=-5;
    data.push_back({x,3*x*x*x+x*x+2*x+1});
    x=1;
    data.push_back({x,3*x*x*x+x*x+2*x+1});
    x=-2;
    data.push_back({x,3*x*x*x+x*x+2*x+1});
    x=4;
    data.push_back({x,3*x*x*x+x*x+2*x+1});
    PolynomialInterpolator pi(data);
    auto v=pi.GetCoefficients();
    std::cout << "PolynomialInterpolator test:\n    Expected output:\n1 2 1 3";
    std::cout << "\n    Actual output:\n";
    for(auto a: v) std::cout<<a<<" ";
    std::cout<<std::endl;
}

void test_polynomial_exception_1() {
    //polinom x^2+2x+1
  std::cout << "PolynomialInterpolator exception test:" << std::endl;
  std::cout << "    Expected output:\nInvalid point\n";
  try {
    std::vector<std::pair<double,double>> data;
    double x=-4;
    data.push_back({x,x*x+2*x+1});
    x=0;
    data.push_back({x,x*x+2*x+1});
    x=-2;
    data.push_back({x,x*x+2*x+1});
    x=3;
    data.push_back({x,x*x+2*x+1});
    PolynomialInterpolator pi(data);
    x=0;
    pi.AddPoint({x,x*x+2*x+1});
  } catch (std::domain_error e) {
    std::cout << "    Actual output:\n" << e.what();
  }
  std::cout << std::endl;
}

void test_polynomial_exception_2() {
    std::vector<std::pair<double,double>> data({{1,2},{2,3},{2,4}});
    std::cout << "PolynomialInterpolator exception test:" << std::endl;
  std::cout << "    Expected output:\nInvalid data set\n";
  try {
    PolynomialInterpolator pi(data);
  } catch (std::domain_error e) {
    std::cout << "    Actual output:\n" << e.what();
  }
  std::cout << std::endl;
}

void test_piecewise_1() {
    //polinom 3x^3+2x^2+x+1
    std::vector<std::pair<double,double>> data;
    for(double i=-5; i<=5; i+=0.5) {
        data.push_back({i,3*i*i*i+2*i*i+i+1});
    }
    PiecewisePolynomialInterpolator ppi(data,3);
    double x=7.11;
    std::cout << "PiecewisePolynomialInterpolator test:\n    Expected output: "<<3*x*x*x+2*x*x+x+1;
    std::cout << "\n    Actual output: " << ppi(x) <<"\n";
}

void test_piecewise_2() {
    //polinom x^2+2x+1
    std::vector<std::pair<double,double>> data;
    for(double i=-5; i<=5; i+=0.5) {
        data.push_back({i,i*i+2*i+1});
    }
    PiecewisePolynomialInterpolator ppi(data,6);
    double x=-2.44;
    std::cout << "PiecewisePolynomialInterpolator test:\n    Expected output: "<<x*x+2*x+1;
    std::cout << "\n    Actual output: " << ppi(x) <<"\n";
}

void test_piecewise_3() {
    //polinom x^3+4x^2+2x+1
    std::vector<std::pair<double,double>> data;
    for(double i=-5; i<=5; i+=0.5) {
        data.push_back({i,i*i*i+4*i*i+2*i+1});
    }
    PiecewisePolynomialInterpolator ppi(data,5);
    double x=-12.7782;
    std::cout << "PiecewisePolynomialInterpolator test:\n    Expected output: "<<x*x*x+4*x*x+2*x+1;
    std::cout << "\n    Actual output: " << ppi(x) <<"\n";
}

void test_piecewise_exception_1() {
    //polinom x^2+2x+1
  std::cout << "PiecewisePolynomialInterpolator exception test:" << std::endl;
  std::cout << "    Expected output:\nInvalid order\n";
  try {
    std::vector<std::pair<double,double>> data;
    for(double i=-5; i<=5; i+=0.5) {
        data.push_back({i,i*i+2*i+1});
    }
    PiecewisePolynomialInterpolator ppi(data,0);
  } catch (std::domain_error e) {
    std::cout << "    Actual output:\n" << e.what();
  }
  std::cout << std::endl;
}

void test_piecewise_exception_2() {
    std::vector<std::pair<double,double>> data({{1,2},{2,3},{2,4}});
    std::cout << "PiecewisePolynomialInterpolator exception test:" << std::endl;
  std::cout << "    Expected output:\nInvalid data set\n";
  try {
    PiecewisePolynomialInterpolator ppi(data,1);
  } catch (std::domain_error e) {
    std::cout << "    Actual output:\n" << e.what();
  }
  std::cout << std::endl;
}

void test_spline_1() {
    std::vector<std::pair<double,double>> data;
    const double pi=4*std::atan(1);
    for(double i=-2*pi; i<=2*pi; i+=pi/8.) {
        data.push_back({i,std::sin(i)});
    }
    SplineInterpolator si(data);
    double x=pi/8.;
    std::cout << "SplineInterpolator test:\n    Expected output: "<<std::sin(x);
    std::cout << "\n    Actual output: " << si(x) <<"\n";
}

void test_spline_2() {
    std::vector<std::pair<double,double>> data;
    const double pi=4*std::atan(1);
    for(double i=2*pi; i>=-2*pi; i-=pi/8.) {
        data.push_back({i,std::cos(i)});
    }
    SplineInterpolator si(data);
    double x=pi/8.;
    std::cout << "SplineInterpolator test:\n    Expected output: "<<std::cos(x);
    std::cout << "\n    Actual output: " << si(x) <<"\n";
}

void test_spline_3() {
    std::vector<std::pair<double,double>> data;
    const double pi=4*std::atan(1);
    for(double i=2*pi; i>=-2*pi; i-=pi/8.) {
        data.push_back({i,std::sin(i)});
    }
    SplineInterpolator si(data);
    double x=2*pi+pi/4.;
    std::cout << "SplineInterpolator test:\n    Expected output: "<<std::sin(x);
    std::cout << "\n    Actual output: " << si(x) <<"\n";
}

void test_spline_exception_1() {
    std::vector<std::pair<double,double>> data({{1,2},{2,3},{2,4}});
    std::cout << "SplineInterpolator exception test:" << std::endl;
  std::cout << "    Expected output:\nInvalid data set\n";
  try {
    SplineInterpolator si(data);
  } catch (std::domain_error e) {
    std::cout << "    Actual output:\n" << e.what();
  }
  std::cout << std::endl;
}

void test_barycentric_1() {
    //polinom 3x^3+2x^2+x+1
    std::vector<std::pair<double,double>> data;
    for(double i=-5; i<=5; i++) {
        data.push_back({i,3*i*i*i+2*i*i+i+1});
    }
    BarycentricInterpolator bi(data,2);
    double x=2;
    std::cout << "BarycentricInterpolator test:\n    Expected output: "<<3*x*x*x+2*x*x+x+1;
    std::cout << "\n    Actual output: " << bi(x) <<"\n";
}

void test_barycentric_2() {
    std::vector<std::pair<double,double>> data;
    for(int i=-3; i<=11; i+=2) {
        data.push_back({i,i*i});
    }
    BarycentricInterpolator bi(data,6);
    double x=5.45;
    std::cout << "BarycentricInterpolator test:\n    Expected output: "<<x*x;
    std::cout << "\n    Actual output: " << bi(x) <<"\n";
}

void test_barycentric_3() {
    std::vector<std::pair<double,double>> data;
    for(int i=-12; i<=30; i+=4) {
        data.push_back({i,i/2.});
    }
    BarycentricInterpolator bi(data,2);
    auto v=bi.GetWeights();
    std::cout << "BarycentricInterpolator test:\n    Expected output:\n0.03125 -0.03125 0.03125 -0.03125 0.03125 -0.03125 0.03125 -0.03125 0.03125 -0.0625 0.03125";
    std::cout << "\n    Actual output:\n";
    for(auto a: v) std::cout<<a<<" ";
    std::cout<<std::endl;
}

void test_barycentric_exception_1() {
  std::cout << "BarycentricInterpolator exception test:" << std::endl;
  std::cout << "    Expected output:\nInvalid order\n";
  try {
    std::vector<std::pair<double,double>> data;
    for(int i=-5; i<=5; i++) {
        data.push_back({i,i*i+2*i+1});
    }
    BarycentricInterpolator bi(data,data.size());
  } catch (std::domain_error e) {
    std::cout << "    Actual output:\n" << e.what();
  }
  std::cout << std::endl;
}

void test_barycentric_exception_2() {
    std::vector<std::pair<double,double>> data({{1,2},{2,3},{2,4}});
    std::cout << "BarycentricInterpolator exception test:" << std::endl;
  std::cout << "    Expected output:\nInvalid data set\n";
  try {
    BarycentricInterpolator bi(data,1);
  } catch (std::domain_error e) {
    std::cout << "    Actual output:\n" << e.what();
  }
  std::cout << std::endl;
}

void test_trigonometric_1() {
    std::vector<std::pair<double,double>> data;
    const double pi=4*std::atan(1);
    for(double i=-2*pi; i<=pi+pi/8.; i+=pi/8.) {
        data.push_back({i,2*std::sin(i)*std::sin(i)-2*std::sin(i)-1});
    }
    TrigonometricInterpolator ti(data);
    double x=pi/8.;
    std::cout << "TrigonometricInterpolator test:\n    Expected output: "<<2*std::sin(x)*std::sin(x)-2*std::sin(x)-1;
    std::cout << "\n    Actual output: " << ti(x) <<"\n";
}

void test_trigonometric_2() {
    std::vector<std::pair<double,double>> data;
    const double pi=4*std::atan(1);
    for(double i=2*pi; i>=0; i-=pi/2)
    data.push_back({i,std::cos(i)});
    TrigonometricInterpolator ti(data);
    double x=-pi/4.;
    std::cout << "TrigonometricInterpolator test:\n    Expected output: "<<std::cos(x);
    std::cout << "\n    Actual output: " << ti(x) <<"\n";
}

void test_trigonometric_exception_1() {
    std::vector<std::pair<double,double>> data({{1,2},{2,3},{2,4}});
    std::cout << "TrigonometricInterpolator exception test:" << std::endl;
  std::cout << "    Expected output:\nInvalid data set\n";
  try {
    TrigonometricInterpolator ti(data);
  } catch (std::domain_error e) {
    std::cout << "    Actual output:\n" << e.what();
  }
  std::cout << std::endl;
}

void test_trigonometric_exception_2() {
    std::cout << "TrigonometricInterpolator exception test:" << std::endl;
  std::cout << "    Expected output:\nFunction is not periodic\n";
  try {
    std::vector<std::pair<double,double>> data;
    const double pi=4*std::atan(1);
    for(double i=2*pi; i>=-2*pi; i-=pi/8.) {
        data.push_back({i,std::sin(i)});
    }
    TrigonometricInterpolator si(data);
  } catch (std::domain_error e) {
    std::cout << "    Actual output:\n" << e.what();
  }
  std::cout << std::endl;
}

int main() { 
    test_linear_1();
    std::cout<<std::endl;
    test_linear_2();
    std::cout<<std::endl;
    test_linear_3();
    std::cout<<std::endl;
    test_linear_exception_1();
    std::cout<<std::endl;
    test_polynomial_1();
    std::cout<<std::endl;
    test_polynomial_2();
    std::cout<<std::endl;
    test_polynomial_3();
    std::cout<<std::endl;
    test_polynomial_exception_1();
    std::cout<<std::endl;
    test_polynomial_exception_2();
    std::cout<<std::endl;
    test_piecewise_1();
    std::cout<<std::endl;
    test_piecewise_2();
    std::cout<<std::endl;
    test_piecewise_3();
    std::cout<<std::endl;
    test_piecewise_exception_1();
    std::cout<<std::endl;
    test_piecewise_exception_2();
    std::cout<<std::endl;
    test_spline_1();
    std::cout<<std::endl;
    test_spline_2();
    std::cout<<std::endl;
    test_spline_3();
    std::cout<<std::endl;
    test_spline_exception_1();
    std::cout<<std::endl;
    test_barycentric_1();
    std::cout<<std::endl;
    test_barycentric_2();
    std::cout<<std::endl;
    test_barycentric_3();
    std::cout<<std::endl;
    test_barycentric_exception_1();
    std::cout<<std::endl;
    test_barycentric_exception_2();
    std::cout<<std::endl;
    test_trigonometric_1();
    std::cout<<std::endl;
    test_trigonometric_2();
    std::cout<<std::endl;
    test_trigonometric_exception_1();
    std::cout<<std::endl;
    test_trigonometric_exception_2();
    return 0; }
