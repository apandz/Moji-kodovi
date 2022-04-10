#include <algorithm>
#include <cmath>
#include <complex>
#include <functional>
#include <iostream>
#include <limits>
#include <vector>

template <typename FunType>
double FindMinimum(FunType f, double x0, double eps = 1e-8, double hinit = 1e-5,
                   double hmax = 1e10, double lambda = 1.4) {
  if (eps <= 0 || hinit <= 0 || hmax <= 0 || lambda <= 0) {
    throw std::domain_error("Invalid parameters");
  }
  double a = x0 - hinit;
  double b = x0 + hinit;
  double c = x0;
  bool found = false;
  while (std::fabs(hinit) < hmax) {
    if (f(c + hinit) < f(c)) {
      b = c + hinit;
      a = c - hinit;
    } else if (f(c - hinit) < f(c)) {
      b = c - hinit;
      a = b - hinit;
    } else {
      a = c - hinit;
      b = c + hinit;
      found = true;
      break;
    }
    c = b;
    hinit *= lambda;
  }
  if (!found) {
    throw std::logic_error("Minimum has not found");
  }
  double phi = (1 + std::sqrt(5)) / 2;
  double d;
  if (std::fabs(c - a) < std::fabs(b - c)) {
    d = b - (b - c) / phi;
  } else {
    d = c;
    c = a + (c - a) / phi;
  }
  double u = f(c);
  double v = f(d);
  while (std::fabs(b - a) > eps) {
    if (u < v) {
      b = d;
      d = c;
      c = a + (c - a) / phi;
      v = u;
      u = f(c);
    } else {
      a = c;
      c = d;
      d = b - (b - d) / phi;
      u = v;
      v = f(d);
    }
  }
  return (a + b) / 2;
}

void test1() {
  std::cout << "Expected output: 4";
  std::cout << std::endl
            << "Actual output: "
            << FindMinimum([](double x) { return (x - 4) * (x - 4) + 5; }, 10);
  std::cout << std::endl;
}

void test2() {
  std::cout << "Expected output: -2.5";
  std::cout << std::endl
            << "Actual output: "
            << FindMinimum([](double x) { return 4 * x * x + 20 * x - 21; }, 5);
  std::cout << std::endl;
}

void test3() {
  std::cout << "Expected output: -1";
  std::cout << std::endl
            << "Actual output: "
            << FindMinimum([](double x) { return (x + 1) * (x + 1) + 5; }, 15);
  std::cout << std::endl;
}

void test4() {
  std::cout << "Expected output: Minimum has not found";
  std::cout << std::endl << "Actual output: ";
  try {
    std::cout << FindMinimum([](double x) { return 1 / x; }, 1);
  } catch (std::logic_error e) {
    std::cout << e.what() << std::endl;
  }
}

void test5() {
  std::cout << "Expected output: Invalid parameters";
  std::cout << std::endl << "Actual output: ";
  try {
    std::cout << FindMinimum([](double x) { return x; }, 1,-1);
  } catch (std::logic_error e) {
    std::cout << e.what() << std::endl;
  }
}

int main() {
  test1();
  std::cout << std::endl;
  test2();
  std::cout << std::endl;
  test3();
  std::cout << std::endl;
  test4();
  std::cout << std::endl;
  test5();
  return 0;
}
