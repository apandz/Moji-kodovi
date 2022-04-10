// NA 2017/2018: Zadaća 4, Zadatak 2
#include <cmath>
#include <iostream>
#include <vector>

template <typename FunType>
std::pair<double, bool> RombergIntegration(FunType f, double a, double b,
                                           double eps = 1e-8,
                                           int nmax = 1000000, int nmin = 50) {
  if (eps < 0 || nmin < 0 || nmax < 0 || nmax < nmin) {
    throw std::domain_error("Bad parameter");
  }
  int n = 2;
  double h = (b - a) / n;
  double s = (f(a) + f(b)) / 2;
  double Iold = s;
  std::vector<double> I;
  for (int i = 1; n <= nmax; i++) {
    for (int j = 1; j <= n / 2; j++) {
      s += f(a + (2 * j - 1) * h);
    }
    I.push_back(h * s);
    double p = 4;
    for (int k = i - 2; k >= 0; k--) {
      I.at(k) = (p * I.at(k + 1) - I.at(k)) / (p - 1);
      p *= 4;
    }
    if (std::fabs(I.at(0) - Iold) <= eps && n >= nmin)
      return {I.at(0), true};
    Iold = I.at(0);
    h /= 2;
    n *= 2;
  }
  return {Iold, false};
}

template <typename FunType>
std::pair<double, bool>
TanhSinhIntegration(FunType f, double a, double b, double eps = 1e-8,
                    int nmax = 1000000, int nmin = 20, double range = 3.5) {
  if (eps < 0 || nmin < 0 || nmax < 0 || nmax < nmin || range < 0) {
    throw std::domain_error("Bad parameter");
  }
  int z = 1;
  if (a > b) {
    z = -1;
    double temp = a;
    a = b;
    b = temp;
  }
  int n = 2;
  double h = 2 * range / n;
  double p = (b + a) / 2;
  double q = (b - a) / 2;
  double s = 0;
  double Iold = s;
  const double pi = 4 * atan(1);
  while (n < nmax) {
    for (int i = 1; i <= n / 2; i++) {
      double t = -range + (2 * i - 1) * h;
      double u = pi * sinh(t) / 2;
      double v = f(p + q * tanh(u));
      if (std::isfinite(v)) {
        s += q * pi * cosh(t) * v / (2 * cosh(u) * cosh(u));
      }
    }
    double I = h * s;
    if (n >= nmin && std::fabs(I - Iold) <= eps)
      return {I * z, true};
    Iold = I;
    n *= 2;
    h /= 2;
  }
  return {Iold * z, false};
}

std::pair<double, bool> operator+(std::pair<double, bool> p1,
                                  std::pair<double, bool> p2) {
  return {p1.first + p2.first, p1.second && p2.second};
}

template <typename FunType>
std::pair<double, bool> AdaptiveAux(FunType f, double a, double b, double eps,
                                    double f1, double f2, double f3, double r) {
  if (!std::isfinite(f1)) {
    f1 = 0;
  }
  if (!std::isfinite(f2)) {
    f2 = 0;
  }
  if (!std::isfinite(f3)) {
    f3 = 0;
  }
  double c = (a + b) / 2;
  double I1 = (b - a) * (f1 + 4 * f3 + f2) / 6;
  double f4 = f((a + c) / 2);
  double f5 = f((c + b) / 2);
  double I2 = (b - a) * (f1 + 4 * f4 + 2 * f3 + 4 * f5 + f2) / 12;
  if (std::fabs(I1 - I2) <= eps) {
    return {I2, true};
  }
  if (r <= 0) {
    return {I2, false};
  }
  return AdaptiveAux(f, a, c, eps, f1, f3, f4, r - 1) +
         AdaptiveAux(f, c, b, eps, f3, f2, f5, r - 1);
}

template <typename FunType>
std::pair<double, bool> AdaptiveIntegration(FunType f, double a, double b,
                                            double eps = 1e-10,
                                            int maxdepth = 30, int nmin = 1) {
  if (eps < 0 || maxdepth < 0 || nmin < 0) {
    throw std::domain_error("Bad parameter");
  }
  std::pair<double, bool> s = {0, true};
  double h = (b - a) / nmin;
  for (int i = 1; i <= nmin; i++) {
    s = s +
        AdaptiveAux(f, a, a + h, eps, f(a), f(a + h), f(a + h / 2), maxdepth);
    a += h;
  }
  return s;
}

// Testovi

void test_RombergIntegration_1() {
  const double pi = 4 * atan(1);
  auto fun = [](double x) { return sin(x); };
  auto ri = RombergIntegration(fun, 0, pi);
  std::cout
      << "Test RombergIntegration 1:\nOčekivan output:\n2 1\nStvaran output:\n";
  std::cout << ri.first << " " << ri.second;
}

void test_RombergIntegration_2() {
  const double pi = 4 * atan(1);
  auto fun = [](double x) { return sin(x) * sin(x) + 1; };
  auto ri = RombergIntegration(fun, 0, 2 * pi);
  std::cout << "Test RombergIntegration 2:\nOčekivan output:\n9.42478 "
               "1\nStvaran output:\n";
  std::cout << ri.first << " " << ri.second;
}

void test_RombergIntegration_3() {
  const double pi = 4 * atan(1);
  auto fun = [](double x) { return x * x * x + 4 * x * x + 3 * x + 5; };
  auto ri = RombergIntegration(fun, 2, 4);
  std::cout << "Test RombergIntegration 3:\nOčekivan output:\n162.667 "
               "1\nStvaran output:\n";
  std::cout << ri.first << " " << ri.second;
}

void test_RombergIntegration_4() {
  const double pi = 4 * atan(1);
  auto fun = [](double x) {
    if (x == 0)
      return 0.;
    return 1. / std::sqrt(x);
  };
  auto ri = RombergIntegration(fun, 0, 3);
  std::cout << "Test RombergIntegration 4:\nOčekivan output:\n3.4612 "
               "0\nStvaran output:\n";
  std::cout << ri.first << " " << ri.second;
}

void test_RombergIntegration_5() {
  const double pi = 4 * atan(1);
  auto fun = [](double x) {
    if (x == 0)
      return 0.;
    return 1. / (x * x + 1);
  };
  auto ri = RombergIntegration(fun, -4, 2);
  std::cout << "Test RombergIntegration 5:\nOčekivan output:\n2.43297 "
               "1\nStvaran output:\n";
  std::cout << ri.first << " " << ri.second;
}

void test_RombergIntegration_exception_1() {
  const double pi = 4 * atan(1);
  auto fun = [](double x) { return sin(x); };
  std::cout << "Test RombergIntegration exception 1:\nOčekivan output: Bad "
               "parameter\nStvaran output: ";
  try {
    auto ri = RombergIntegration(fun, 0, pi, -2);
  } catch (std::domain_error e) {
    std::cout << e.what();
  } catch (...) {
    std::cout << "Pogrešan izuzetak";
  }
}

void test_RombergIntegration_exception_2() {
  const double pi = 4 * atan(1);
  auto fun = [](double x) { return sin(x); };
  std::cout << "Test RombergIntegration exception 2:\nOčekivan output: Bad "
               "parameter\nStvaran output: ";
  try {
    auto ri = RombergIntegration(fun, 0, pi, 2, 1000, 10000);
  } catch (std::domain_error e) {
    std::cout << e.what();
  } catch (...) {
    std::cout << "Pogrešan izuzetak";
  }
}

void test_TanhSinhIntegration_1() {
  const double pi = 4 * atan(1);
  auto fun = [](double x) { return sin(x); };
  auto ri = TanhSinhIntegration(fun, 0, pi);
  std::cout << "Test TanhSinhIntegration 1:\nOčekivan output:\n2 1\nStvaran "
               "output:\n";
  std::cout << ri.first << " " << ri.second;
}

void test_TanhSinhIntegration_2() {
  const double pi = 4 * atan(1);
  auto fun = [](double x) { return sin(x) * sin(x) + 1; };
  auto ri = TanhSinhIntegration(fun, 0, 2 * pi);
  std::cout << "Test TanhSinhIntegration 2:\nOčekivan output:\n9.42478 "
               "1\nStvaran output:\n";
  std::cout << ri.first << " " << ri.second;
}

void test_TanhSinhIntegration_3() {
  const double pi = 4 * atan(1);
  auto fun = [](double x) { return x * x * x + 4 * x * x + 3 * x + 5; };
  auto ri = TanhSinhIntegration(fun, 2, 4);
  std::cout << "Test TanhSinhIntegration 3:\nOčekivan output:\n162.667 "
               "1\nStvaran output:\n";
  std::cout << ri.first << " " << ri.second;
}

void test_TanhSinhIntegration_4() {
  const double pi = 4 * atan(1);
  auto fun = [](double x) {
    if (x == 0)
      return 0.;
    return 1. / std::sqrt(x);
  };
  auto ri = TanhSinhIntegration(fun, 0, 3);
  std::cout << "Test TanhSinhIntegration 4:\nOčekivan output:\n3.4641 "
               "1\nStvaran output:\n";
  std::cout << ri.first << " " << ri.second;
}

void test_TanhSinhIntegration_5() {
  const double pi = 4 * atan(1);
  auto fun = [](double x) {
    if (x == 0)
      return 0.;
    return 1. / (x * x + 1);
  };
  auto ri = TanhSinhIntegration(fun, -4, 2);
  std::cout << "Test TanhSinhIntegration 5:\nOčekivan output:\n2.43297 "
               "1\nStvaran output:\n";
  std::cout << ri.first << " " << ri.second;
}

void test_TanhSinhIntegration_exception_1() {
  const double pi = 4 * atan(1);
  auto fun = [](double x) { return sin(x); };
  std::cout << "Test TanhSinhIntegration exception 1:\nOčekivan output: Bad "
               "parameter\nStvaran output: ";
  try {
    auto ri = TanhSinhIntegration(fun, 0, pi, -2);
  } catch (std::domain_error e) {
    std::cout << e.what();
  } catch (...) {
    std::cout << "Pogrešan izuzetak";
  }
}

void test_TanhSinhIntegration_exception_2() {
  const double pi = 4 * atan(1);
  auto fun = [](double x) { return sin(x); };
  std::cout << "Test TanhSinhIntegration exception 2:\nOčekivan output: Bad "
               "parameter\nStvaran output: ";
  try {
    auto ri = TanhSinhIntegration(fun, 0, pi, 2, 1000, 10000);
  } catch (std::domain_error e) {
    std::cout << e.what();
  } catch (...) {
    std::cout << "Pogrešan izuzetak";
  }
}

void test_AdaptiveIntegration_1() {
  const double pi = 4 * atan(1);
  auto fun = [](double x) { return sin(x); };
  auto ri = AdaptiveIntegration(fun, 0, pi);
  std::cout << "Test AdaptiveIntegration 1:\nOčekivan output:\n2 1\nStvaran "
               "output:\n";
  std::cout << ri.first << " " << ri.second;
}

void test_AdaptiveIntegration_2() {
  const double pi = 4 * atan(1);
  auto fun = [](double x) { return sin(x) * sin(x) + 1; };
  auto ri = AdaptiveIntegration(fun, 0, 2 * pi);
  std::cout << "Test AdaptiveIntegration 2:\nOčekivan output:\n9.42478 "
               "1\nStvaran output:\n";
  std::cout << ri.first << " " << ri.second;
}

void test_AdaptiveIntegration_3() {
  const double pi = 4 * atan(1);
  auto fun = [](double x) { return x * x * x + 4 * x * x + 3 * x + 5; };
  auto ri = AdaptiveIntegration(fun, 2, 4);
  std::cout << "Test AdaptiveIntegration 3:\nOčekivan output:\n162.667 "
               "1\nStvaran output:\n";
  std::cout << ri.first << " " << ri.second;
}

void test_AdaptiveIntegration_4() {
  const double pi = 4 * atan(1);
  auto fun = [](double x) {
    if (x == 0)
      return 0.;
    return 1. / std::sqrt(x);
  };
  auto ri = AdaptiveIntegration(fun, 0, 3);
  std::cout << "Test AdaptiveIntegration 4:\nOčekivan output:\n3.46407 "
               "0\nStvaran output:\n";
  std::cout << ri.first << " " << ri.second;
}

void test_AdaptiveIntegration_5() {
  const double pi = 4 * atan(1);
  auto fun = [](double x) {
    if (x == 0)
      return 0.;
    return 1. / (x * x + 1);
  };
  auto ri = AdaptiveIntegration(fun, -4, 2);
  std::cout << "Test AdaptiveIntegration 5:\nOčekivan output:\n2.43297 "
               "1\nStvaran output:\n";
  std::cout << ri.first << " " << ri.second;
}

void test_AdaptiveIntegration_exception_1() {
  const double pi = 4 * atan(1);
  auto fun = [](double x) { return sin(x); };
  std::cout << "Test AdaptiveIntegration exception 1:\nOčekivan output: Bad "
               "parameter\nStvaran output: ";
  try {
    auto ri = AdaptiveIntegration(fun, 0, pi, -2);
  } catch (std::domain_error e) {
    std::cout << e.what();
  } catch (...) {
    std::cout << "Pogrešan izuzetak";
  }
}

void test_AdaptiveIntegration_exception_2() {
  const double pi = 4 * atan(1);
  auto fun = [](double x) { return sin(x); };
  std::cout << "Test AdaptiveIntegration exception 2:\nOčekivan output: Bad "
               "parameter\nStvaran output: ";
  try {
    auto ri = AdaptiveIntegration(fun, 0, pi, 2, 1000, -5);
  } catch (std::domain_error e) {
    std::cout << e.what();
  } catch (...) {
    std::cout << "Pogrešan izuzetak";
  }
}

int main() {
  std::cout << "Testovi za "
               "RombergIntegration\n--------------------------------------";
  std::cout << std::endl << std::endl;
  test_RombergIntegration_1();
  std::cout << std::endl << std::endl;
  test_RombergIntegration_2();
  std::cout << std::endl << std::endl;
  test_RombergIntegration_3();
  std::cout << std::endl << std::endl;
  test_RombergIntegration_4();
  std::cout << std::endl << std::endl;
  test_RombergIntegration_5();
  std::cout << std::endl << std::endl;
  test_RombergIntegration_exception_1();
  std::cout << std::endl << std::endl;
  test_RombergIntegration_exception_2();
  std::cout << std::endl << std::endl;
  std::cout << "Testovi za "
               "TanhSinhIntegration\n--------------------------------------";
  std::cout << std::endl << std::endl;
  test_TanhSinhIntegration_1();
  std::cout << std::endl << std::endl;
  test_TanhSinhIntegration_2();
  std::cout << std::endl << std::endl;
  test_TanhSinhIntegration_3();
  std::cout << std::endl << std::endl;
  test_TanhSinhIntegration_4();
  std::cout << std::endl << std::endl;
  test_TanhSinhIntegration_5();
  std::cout << std::endl << std::endl;
  test_TanhSinhIntegration_exception_1();
  std::cout << std::endl << std::endl;
  test_TanhSinhIntegration_exception_2();
  std::cout << std::endl << std::endl;
  std::cout << "Testovi za "
               "AdaptiveIntegration\n--------------------------------------";
  std::cout << std::endl << std::endl;
  test_AdaptiveIntegration_1();
  std::cout << std::endl << std::endl;
  test_AdaptiveIntegration_2();
  std::cout << std::endl << std::endl;
  test_AdaptiveIntegration_3();
  std::cout << std::endl << std::endl;
  test_AdaptiveIntegration_4();
  std::cout << std::endl << std::endl;
  test_AdaptiveIntegration_5();
  std::cout << std::endl << std::endl;
  test_AdaptiveIntegration_exception_1();
  std::cout << std::endl << std::endl;
  test_AdaptiveIntegration_exception_2();
  std::cout << std::endl << std::endl;
  return 0;
}
