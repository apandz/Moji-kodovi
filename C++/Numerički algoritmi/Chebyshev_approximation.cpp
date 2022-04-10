#include <cmath>
#include <iostream>
#include <limits>
#include <vector>

class ChebyshevApproximation {
  int m;
  double xmin, xmax;
  std::vector<double> c;
  ChebyshevApproximation(std::vector<double> c1, double xmin, double xmax);

public:
  template <typename FunType>
  ChebyshevApproximation(FunType f, double xmin, double xmax, int n);
  void set_m(int m);
  void trunc(double eps);
  double operator()(double x) const;
  double derivative(double x) const;
  ChebyshevApproximation derivative() const;
  ChebyshevApproximation antiderivative() const;
  double integrate(double a, double b) const;
  double integrate() const;
};

ChebyshevApproximation::ChebyshevApproximation(std::vector<double> c1,
                                               double xmin, double xmax)
    : c(c1), m(c1.size() - 1), xmin(xmin), xmax(xmax) {}

template <typename FunType>
ChebyshevApproximation::ChebyshevApproximation(FunType f, double xmin,
                                               double xmax, int n)
    : m(n), xmin(xmin), xmax(xmax) {
  if (xmin > xmax || fabs(xmin - xmax) <= 1e-10 * (fabs(xmin) + fabs(xmax)) ||
      n < 1) {
    throw std::domain_error("Bad parameters");
  }
  std::vector<double> v(n + 1), w(n + 2);
  c.resize(n + 1);
  const double pi = 4 * atan(1);
  for (int i = 0; i <= n + 1; i++) {
    w.at(i) = cos(pi * i * 1. / (2 * n + 2));
  }
  for (int i = 0; i <= n / 2; i++) {
    v.at(i) = f((xmin + xmax + (xmax - xmin) * w.at(2 * i + 1)) / 2.);
  }
  for (int i = n / 2 + 1; i <= n; i++) {
    v.at(i) = f((xmin + xmax - (xmax - xmin) * w.at(2 * n + 1 - 2 * i)) / 2.);
  }
  for (int k = 0; k <= n; k++) {
    double s = 0;
    for (int i = 0; i <= n; i++) {
      int p = (k * (2 * i + 1)) % (4 * n + 4);
      if (p > 2 * n + 2) {
        p = 4 * n + 4 - p;
      }
      if (p > n + 1) {
        s -= v.at(i) * w.at(2 * n + 2 - p);
      } else {
        s += v.at(i) * w.at(p);
      }
      c.at(k) = 2 * s * 1. / (n + 1);
    }
  }
}

void ChebyshevApproximation::set_m(int m) {
  if (m <= 1 || m > c.size() - 1) {
    throw std::domain_error("Bad order");
  }
  this->m = m;
}

void ChebyshevApproximation::trunc(double eps) {
  if (eps < 0) {
    throw std::domain_error("Bad tolerance");
  }
  for (int i = m; i >= 0; i--) {
    if (std::fabs(c.at(i)) > eps) {
      m = i;
      break;
    }
  }
  if (m <= 1 || m >= c.size() - 1) {
    throw std::domain_error("Bad tolerance");
  }
}

double ChebyshevApproximation::operator()(double x) const {
  if (x < xmin || x > xmax) {
    throw std::domain_error("Bad argument");
  }
  double t = (2 * x - xmin - xmax) / (xmax - xmin);
  double p = 1, q = t;
  double s = c.at(0) / 2. + c.at(1) * t;
  for (int k = 2; k <= m; k++) {
    double r = 2 * t * q - p;
    s += c.at(k) * r;
    p = q;
    q = r;
  }
  return s;
}

double ChebyshevApproximation::derivative(double x) const {
  if (x < xmin || x > xmax) {
    throw std::domain_error("Bad argument");
  }
  double t = (2 * x - xmin - xmax) / (xmax - xmin);
  double p = 1;
  double q = 4 * t;
  double s = c.at(1) + 4 * c.at(2) * t;
  for (int k = 3; k <= m; k++) {
    double r = k * (2 * t * q * 1. / (k - 1) - p * 1. / (k - 2));
    s += c.at(k) * r;
    p = q;
    q = r;
  }
  return 2 * s * 1. / (xmax - xmin);
}

ChebyshevApproximation ChebyshevApproximation::derivative() const {
  std::vector<double> c1(c.size());
  double mi = 4. / (xmax - xmin);
  c1.at(m - 1) = mi * m * c.at(m);
  c1.at(m - 2) = mi * (m - 1) * c.at(m - 1);
  for (int k = m - 3; k >= 0; k--) {
    c1.at(k) = c1.at(k + 2) + mi * (k + 1) * c.at(k + 1);
  }
  return ChebyshevApproximation(c1, xmin, xmax);
}

ChebyshevApproximation ChebyshevApproximation::antiderivative() const {
  std::vector<double> c1(m + 2);
  double mi = (xmax - xmin) / 4;
  for (int k = 1; k <= m + 1; k++) {
    double s = 0;
    if (k != m && k != m + 1) {
      s = c.at(k + 1);
    }
    c1.at(k) = mi * (c.at(k - 1) - s) * 1. / k;
  }
  return ChebyshevApproximation(c1, xmin, xmax);
}

double ChebyshevApproximation::integrate(double a, double b) const {
  if (a < xmin || a > xmax || b < xmin || b > xmax) {
    throw std::domain_error("Bad interval");
  }
  auto F(this->antiderivative());
  return F(b) - F(a);
}

double ChebyshevApproximation::integrate() const {
  double s = 0;
  for (int k = 1; k <= (m - 1) / 2 + 1; k++) {
    s += 2 * c.at(2 * k) * 1. / (1 - 4 * k * k);
  }
  s += c.at(0);
  s *= (xmax - xmin) / 2.;
  return s;
}

// Testovi

void test_set_m_exception_1() {
  const double pi = 4 * atan(1);
  auto fun = [](double x) { return sin(x); };
  ChebyshevApproximation ca(fun, 0, pi, 8);
  std::cout << "Test set_m exception 1:\nOčekivan output: Bad order\nStvaran "
               "output: ";
  try {
    ca.set_m(-1);
  } catch (std::domain_error e) {
    std::cout << e.what();
  } catch (...) {
    std::cout << "Pogrešan izuzetak";
  }
}

void test_set_m_exception_2() {
  const double pi = 4 * atan(1);
  auto fun = [](double x) { return sin(x); };
  ChebyshevApproximation ca(fun, 0, pi, 8);
  std::cout << "Test set_m exception 2:\nOčekivan output: Bad order\nStvaran "
               "output: ";
  try {
    ca.set_m(13);
  } catch (std::domain_error e) {
    std::cout << e.what();
  } catch (...) {
    std::cout << "Pogrešan izuzetak";
  }
}

void test_trunc_exception_1() {
  const double pi = 4 * atan(1);
  auto fun = [](double x) { return sin(x); };
  ChebyshevApproximation ca(fun, 0, pi, 8);
  std::cout << "Test trunc exception 1:\nOčekivan output: Bad "
               "tolerance\nStvaran output: ";
  try {
    ca.trunc(-0.0001);
  } catch (std::domain_error e) {
    std::cout << e.what();
  } catch (...) {
    std::cout << "Pogrešan izuzetak";
  }
}

void test_trunc_exception_2() {
  const double pi = 4 * atan(1);
  auto fun = [](double x) { return sin(x); };
  ChebyshevApproximation ca(fun, 0, pi, 8);
  std::cout << "Test trunc exception 2:\nOčekivan output: Bad "
               "tolerance\nStvaran output: ";
  try {
    ca.trunc(1000);
  } catch (std::domain_error e) {
    std::cout << e.what();
  } catch (...) {
    std::cout << "Pogrešan izuzetak";
  }
}

void test_operator_brackets_exception_1() {
  const double pi = 4 * atan(1);
  auto fun = [](double x) { return sin(x); };
  ChebyshevApproximation ca(fun, 0, pi, 8);
  std::cout << "Test operator() exception 1:\nOčekivan output: Bad "
               "argument\nStvaran output: ";
  try {
    ca(-5);
  } catch (std::domain_error e) {
    std::cout << e.what();
  } catch (...) {
    std::cout << "Pogrešan izuzetak";
  }
}

void test_operator_brackets_exception_2() {
  const double pi = 4 * atan(1);
  auto fun = [](double x) { return sin(x); };
  ChebyshevApproximation ca(fun, 0, pi, 8);
  std::cout << "Test operator() exception 2:\nOčekivan output: Bad "
               "argument\nStvaran output: ";
  try {
    ca(4);
  } catch (std::domain_error e) {
    std::cout << e.what();
  } catch (...) {
    std::cout << "Pogrešan izuzetak";
  }
}

void test_operator_brackets_1() {
  const double pi = 4 * atan(1);
  auto fun = [](double x) { return sin(x); };
  ChebyshevApproximation ca(fun, 0, pi, 8);
  std::cout << "Test operator() 1:\nOčekivan output: " << fun(3.);
  std::cout << "\nStvaran output: " << ca(3.);
}

void test_operator_brackets_2() {
  const double pi = 4 * atan(1);
  auto fun = [](double x) { return sin(x); };
  ChebyshevApproximation ca(fun, 0, pi, 8);
  std::cout << "Test operator() 2:\nOčekivan output: " << fun(0.56);
  std::cout << "\nStvaran output: " << ca(0.56);
}

void test_derivative_1() {
  const double pi = 4 * atan(1);
  auto fun = [](double x) { return sin(x); };
  ChebyshevApproximation ca(fun, 0, pi, 8);
  std::cout << "Test derivative 1:\nOčekivan output:\n0.997495 0.0707372 "
               "0.0707372 -0.997494\nStvaran output:\n";
  std::cout << fun(1.5) << " " << ca.derivative(1.5) << " "
            << ca.derivative()(1.5) << " " << ca.derivative().derivative(1.5);
}

void test_derivative_2() {
  const double pi = 4 * atan(1);
  auto fun = [](double x) { return sin(x); };
  ChebyshevApproximation ca(fun, 0, pi, 8);
  std::cout << "Test derivative 2:\nOčekivan output:\n0.714978 -0.699147 "
               "-0.699147 -0.714976 0.699158\nStvaran output:\n";
  std::cout << fun(2.345) << " " << ca.derivative(2.345) << " "
            << ca.derivative()(2.345) << " "
            << ca.derivative().derivative(2.345) << " "
            << ca.derivative().derivative().derivative(2.345);
}

void test_derivative_exception_1() {
  const double pi = 4 * atan(1);
  auto fun = [](double x) { return sin(x); };
  ChebyshevApproximation ca(fun, 0, pi, 8);
  std::cout << "Test derivative exception 1:\nOčekivan output: Bad "
               "argument\nStvaran output: ";
  try {
    ca.derivative(4);
  } catch (std::domain_error e) {
    std::cout << e.what();
  } catch (...) {
    std::cout << "Pogrešan izuzetak";
  }
}

void test_derivative_exception_2() {
  const double pi = 4 * atan(1);
  auto fun = [](double x) { return sin(x); };
  ChebyshevApproximation ca(fun, 0, pi, 8);
  std::cout << "Test derivative exception 2:\nOčekivan output: Bad "
               "argument\nStvaran output: ";
  try {
    ca.derivative(-1);
  } catch (std::domain_error e) {
    std::cout << e.what();
  } catch (...) {
    std::cout << "Pogrešan izuzetak";
  }
}

void test_integrate_1() {
  const double pi = 4 * atan(1);
  auto fun = [](double x) { return sin(x); };
  ChebyshevApproximation ca(fun, 0, pi, 8);
  std::cout << "Test integrate 1:\nOčekivan output:\n1.86758 1.0298 "
               "0.413269\nStvaran output:\n";
  std::cout << ca.integrate(0.5, 3) << " " << ca.integrate(1.23, 2.34) << " "
            << ca.integrate(0.88, 1.345);
}

void test_integrate_2() {
  const double pi = 4 * atan(1);
  auto fun = [](double x) { return sin(x); };
  ChebyshevApproximation ca(fun, 0, pi, 8);
  std::cout << "Test integrate 2:\nOčekivan output:\n2\nStvaran output:\n";
  std::cout << ca.integrate();
}

void test_integrate_ab_exception_1() {
  const double pi = 4 * atan(1);
  auto fun = [](double x) { return sin(x); };
  ChebyshevApproximation ca(fun, 0, pi, 8);
  std::cout << "Test integrate exception 1:\nOčekivan output: Bad "
               "interval\nStvaran output: ";
  try {
    ca.integrate(-5, 1);
  } catch (std::domain_error e) {
    std::cout << e.what();
  } catch (...) {
    std::cout << "Pogrešan izuzetak";
  }
}

void test_integrate_ab_exception_2() {
  const double pi = 4 * atan(1);
  auto fun = [](double x) { return sin(x); };
  ChebyshevApproximation ca(fun, 0, pi, 8);
  std::cout << "Test integrate exception 2:\nOčekivan output: Bad "
               "interval\nStvaran output: ";
  try {
    ca.integrate(1, 7);
  } catch (std::domain_error e) {
    std::cout << e.what();
  } catch (...) {
    std::cout << "Pogrešan izuzetak";
  }
}

int main() {
  std::cout << "Testovi za funkciju sin(x):" << std::endl
            << "-----------------------";
  std::cout << std::endl << std::endl;
  test_set_m_exception_1();
  std::cout << std::endl << std::endl;
  test_set_m_exception_2();
  std::cout << std::endl << std::endl;
  test_trunc_exception_1();
  std::cout << std::endl << std::endl;
  test_trunc_exception_2();
  std::cout << std::endl << std::endl;
  test_operator_brackets_exception_1();
  std::cout << std::endl << std::endl;
  test_operator_brackets_exception_2();
  std::cout << std::endl << std::endl;
  test_operator_brackets_1();
  std::cout << std::endl << std::endl;
  test_operator_brackets_2();
  std::cout << std::endl << std::endl;
  test_derivative_1();
  std::cout << std::endl << std::endl;
  test_derivative_2();
  std::cout << std::endl << std::endl;
  test_derivative_exception_1();
  std::cout << std::endl << std::endl;
  test_derivative_exception_2();
  std::cout << std::endl << std::endl;
  test_integrate_1();
  std::cout << std::endl << std::endl;
  test_integrate_2();
  std::cout << std::endl << std::endl;
  test_integrate_ab_exception_1();
  std::cout << std::endl << std::endl;
  test_integrate_ab_exception_2();

  return 0;
}
