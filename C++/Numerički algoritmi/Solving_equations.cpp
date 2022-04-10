#include <algorithm>
#include <cmath>
#include <complex>
#include <functional>
#include <iostream>
#include <limits>
#include <vector>

template <typename FunType>
bool BracketRoot(FunType f, double x0, double &a, double &b,
                 double hinit = 1e-5, double hmax = 1e10, double lambda = 1.4) {
  if (hinit <= 0 || hmax <= 0 || lambda <= 0) {
    throw std::domain_error("Invalid parameters");
  }
  double a1 = x0;
  double f1 = f(a1);
  double h = hinit;
  while (std::fabs(h) < hmax) {
    double b1 = a1 + h;
    double f2 = f(b1);
    if (f1 * f2 <= 0) {
      if (b1 < a1) {
        a = b1;
        b = a1;
      } else {
        a = a1;
        b = b1;
      }
      return true;
    }
    h *= lambda;
    a1 = b1;
    f1 = f2;
  }
  a1 = x0;
  f1 = f(a1);
  h = hinit;
  while (std::fabs(h) < hmax) {
    double b1 = a1 - h;
    double f2 = f(b1);
    if (f1 * f2 <= 0) {
      if (b1 < a1) {
        a = b1;
        b = a1;
      } else {
        a = a1;
        b = b1;
      }
      return true;
    }
    h *= lambda;
    a1 = b1;
    f1 = f2;
  }

  return false;
}

enum RegulaFalsiMode { Unmodified, Illinois, Slavic, IllinoisSlavic };

template <typename FunType>
double RegulaFalsiSolve(FunType f, double a, double b,
                        RegulaFalsiMode mode = Slavic, double eps = 1e-10,
                        int maxiter = 100) {
  if (eps <= 0 || maxiter <= 0) {
    throw std::domain_error("Invalid parameters");
  }
  double f1 = f(a);
  double f2 = f(b);
  if ((f1 <= 0 && f2 <= 0) || (f1 > 0 && f2 > 0)) {
    throw std::range_error("Root must be bracketed");
  }

  if (mode == Unmodified) {
    double c = a;
    double cold = b;
    int it = 0;
    while (std::fabs(c - cold) > eps) {
      if (it++ == maxiter) {
        throw std::logic_error("Given accuracy has not achieved");
      }
      cold = c;
      c = (a * f2 - b * f1) / (f2 - f1);
      double f3 = f(c);
      if (std::fabs(f3) < eps)
        return c;
      if (f1 * f3 < 0) {
        b = a;
        f2 = f1;
      }
      a = c;
      f1 = f3;
    }
    return c;
  } else if (mode == Illinois) {
    double c = a;
    double cold = b;
    int it = 0;
    while (std::fabs(c - cold) > eps) {
      if (it++ == maxiter) {
        throw std::logic_error("Given accuracy has not achieved");
      }
      cold = c;
      c = (a * f2 - b * f1) / (f2 - f1);
      double f3 = f(c);
      if (std::fabs(f3) < eps)
        return c;
      if (f1 * f3 < 0) {
        b = a;
        f2 = f1;
      } else {
        f2 /= 2;
      }
      a = c;
      f1 = f3;
    }
    return c;
  } else if (mode == Slavic) {
    std::function<double(double)> phi = [](double x) {
      return x / (1 + std::fabs(x));
    };
    f1 = phi(f1);
    f2 = phi(f2);
    double c = a;
    double cold = b;
    int it = 0;
    while (std::fabs(c - cold) > eps) {
      if (it++ == maxiter) {
        throw std::logic_error("Given accuracy has not achieved");
      }
      cold = c;
      c = (a * f2 - b * f1) / (f2 - f1);
      double f3 = phi(f(c));
      if (std::fabs(f3) < eps)
        return c;
      if (f1 * f3 < 0) {
        b = a;
        f2 = f1;
      }
      a = c;
      f1 = f3;
    }
    return c;
  } else {
    std::function<double(double)> phi = [](double x) {
      return x / (1 + std::fabs(x));
    };
    f1 = phi(f1);
    f2 = phi(f2);
    double c = a;
    double cold = b;
    int it = 0;
    while (std::fabs(c - cold) > eps) {
      if (it++ == maxiter) {
        throw std::logic_error("Given accuracy has not achieved");
      }
      cold = c;
      c = (a * f2 - b * f1) / (f2 - f1);
      double f3 = phi(f(c));
      if (std::fabs(f3) < eps)
        return c;
      if (f1 * f3 < 0) {
        b = a;
        f2 = f1;
      } else {
        f2 /= 2;
      }
      a = c;
      f1 = f3;
    }
    return c;
  }
  return 0;
}

template <typename FunType>
double RiddersSolve(FunType f, double a, double b, double eps = 1e-10,
                    int maxiter = 100) {
  if (eps <= 0 || maxiter <= 0) {
    throw std::domain_error("Invalid parameters");
  }
  double f1 = f(a);
  double f2 = f(b);
  if ((f1 <= 0 && f2 <= 0) || (f1 > 0 && f2 > 0)) {
    throw std::range_error("Root must be bracketed");
  }
  int it = 0;
  while (std::fabs(b - a) > eps) {
    if (it++ == maxiter) {
      throw std::logic_error("Given accuracy has not achieved");
    }
    double c = (a + b) / 2;
    double f3 = f(c);
    if (std::fabs(f3) < eps) {
      return c;
    }
    int sgn = 0;
    if (f1 - f2 < 0)
      sgn = -1;
    else if (f1 - f2 > 0)
      sgn = 1;
    double d = c + f3 * (c - a) * sgn / (std::sqrt(f3 * f3 - f1 * f2));
    double f4 = f(d);
    if (std::fabs(f4) < eps) {
      return d;
    }
    if (f3 * f4 <= 0) {
      a = c;
      b = d;
      f1 = f3;
      f2 = f4;
    } else if (f1 * f4 <= 0) {
      b = d;
      f2 = f4;
    } else {
      a = d;
      f1 = f4;
    }
  }
  return (a + b) / 2;
}

template <typename FunType1, typename FunType2>
double NewtonRaphsonSolve(FunType1 f, FunType2 fprim, double x0,
                          double eps = 1e-10, double damping = 0,
                          int maxiter = 100) {
  if (eps <= 0 || maxiter <= 0 || damping < 0 || damping >= 1) {
    throw std::domain_error("Invalid parameters");
  }
  double dx = std::numeric_limits<double>::infinity();
  double v = f(x0);
  double d = fprim(x0);
  int it = 0;
  while (std::fabs(dx) > eps) {
    if (it++ == maxiter || std::fabs(fprim(x0)) <= eps || !std::isfinite(x0)) {
      throw std::logic_error("Convergence has not achieved");
    }
    if (std::fabs(v) <= eps) {
      return x0;
    }
    dx = v / d;
    double w = v;
    v = f(x0 - dx);
    d = fprim(x0 - dx);
    while (std::fabs(v) > std::fabs(w) || !std::isfinite(v) || d == 0) {
      dx *= damping;
      v = f(x0 - dx);
      d = fprim(x0 - dx);
    }
    x0 -= dx;
  }
  return x0;
}

std::complex<double> operator*(std::complex<double> c1,
                               std::complex<double> c2) {
  return {c1.real() * c2.real() - c1.imag() * c2.imag(),
          c1.real() * c2.imag() + c1.imag() * c2.real()};
}

std::complex<double> operator*(double n, std::complex<double> c) {
  return {n * c.real(), 0};
}

std::pair<std::complex<double>, bool>
Laguerre(std::vector<std::complex<double>> p, int n, std::complex<double> x,
         int maxiter, double eps) {
  std::complex<double> dx = std::numeric_limits<double>::infinity();
  int k = 1;
  while (std::fabs(dx) > eps && k < maxiter) {
    std::complex<double> f = p.at(n);
    std::complex<double> d = 0;
    std::complex<double> s = 0;
    for (int i = n - 1; i >= 0; i--) {
      s = s * x + 2 * d;
      d = d * x + f;
      f = f * x + p.at(i);
    }
    if (std::fabs(f.real()) <= eps && std::fabs(f.imag()) < eps) {
      return {x, true};
    }
    std::complex<double> r = std::sqrt((n - 1) * ((n - 1) * d * d - n * f * s));
    if (std::fabs(d + r) > std::fabs(d - r)) {
      dx = n * f / (d + r);
    } else {
      dx = n * f / (d - r);
    }
    x -= dx;
    k++;
  }
  if (std::fabs(dx) <= eps) {
    return {x, true};
  }
  return {x, false};
}

std::pair<std::complex<double>, bool> Laguerre(std::vector<double> p, int n,
                                               std::complex<double> x,
                                               int maxiter, double eps) {
  std::complex<double> dx = std::numeric_limits<double>::infinity();
  int k = 1;
  while (std::fabs(dx) > eps && k < maxiter) {
    std::complex<double> f = p.at(n);
    std::complex<double> d = 0;
    std::complex<double> s = 0;
    for (int i = n - 1; i >= 0; i--) {
      s = s * x + 2 * d;
      d = d * x + f;
      f = f * x + p.at(i);
    }
    if (std::fabs(f) <= eps) {
      return {x, true};
    }
    std::complex<double> r = std::sqrt((n - 1) * ((n - 1) * d * d - n * f * s));
    if (std::fabs(d + r) > std::fabs(d - r)) {
      dx = n * f / (d + r);
    } else {
      dx = n * f / (d - r);
    }
    x -= dx;
    k++;
  }
  if (std::fabs(dx) <= eps) {
    return {x, true};
  }
  return {x, false};
}

std::vector<std::complex<double>>
PolyRoots(std::vector<std::complex<double>> coefficients, double eps = 1e-10,
          int maxiters = 100, int maxtrials = 10) {
  if (eps <= 0 || maxiters <= 0 || maxtrials <= 0) {
    throw std::domain_error("Invalid parameters");
  }
  int n = coefficients.size();
  std::vector<std::complex<double>> vec(n);
  n--;
  int i = n;
  int it = 0;
  while (i >= 1) {
    if (it++ == maxiters) {
      throw std::logic_error("Convergence has not achieved");
    }
    int t = 1;
    bool c = false;
    std::complex<double> x;
    while (!c && t < maxtrials) {
      x = {0, 0};
      std::pair<std::complex<double>, bool> lag =
          Laguerre(coefficients, i, x, maxiters, eps);
      x = lag.first;
      c = lag.second;
      t++;
    }
    if (!c)
      throw std::logic_error("Convergence has not achieved");
    if (std::fabs(x.imag()) <= eps) {
      x = x.real();
      vec.at(i) = x;
      std::complex<double> v = coefficients.at(i);
      for (int j = i - 1; j >= 0; j--) {
        std::complex<double> w = coefficients.at(j);
        coefficients.at(j) = v;
        v = w + x * v;
      }
      i--;
    }
  }
  std::sort(vec.begin(), vec.end(),
            [eps](std::complex<double> c1, std::complex<double> c2) {
              if (std::fabs(c1.real() - c2.real()) <= eps) {
                return c1.imag() < c2.imag();
              }
              return c1.real() < c2.real();
            });

  return vec;
}

std::vector<std::complex<double>> PolyRoots(std::vector<double> coefficients,
                                            double eps = 1e-10,
                                            int maxiters = 100,
                                            int maxtrials = 10) {
  if (eps <= 0 || maxiters <= 0 || maxtrials <= 0) {
    throw std::domain_error("Invalid parameters");
  }
  int n = coefficients.size();
  std::vector<std::complex<double>> vec(n);
  n--;
  int i = n;
  int it = 0;
  while (i >= 1) {
    if (it++ == maxiters) {
      throw std::logic_error("Convergence has not achieved");
    }
    int t = 1;
    bool c = false;
    std::complex<double> x;
    while (!c && t < maxtrials) {
      x = {0, 0};
      std::pair<std::complex<double>, bool> lag =
          Laguerre(coefficients, i, x, maxiters, eps);
      x = lag.first;
      c = lag.second;
      t++;
    }
    if (!c) {
      throw std::logic_error("Convergence has not achieved");
    }
    if (std::fabs(x.imag()) <= eps) {
      x = x.real();
      vec.at(i) = x;
      double v = coefficients.at(i);
      for (int j = i - 1; j >= 0; j--) {
        double w = coefficients.at(j);
        coefficients.at(j) = v;
        v = w + x.real() * v;
      }
      i--;
    } else {
      vec.at(i) = x;
      vec.at(i - 1) = {x.real(), -x.imag()};
      double alfa = 2 * x.real();
      double beta = std::fabs(x) * std::fabs(x);
      double u = coefficients.at(i);
      double v = coefficients.at(i - 1) + alfa * u;
      for (int j = i - 2; j >= 0; j--) {
        double w = coefficients.at(j);
        coefficients.at(j) = u;
        u = v;
        v = w + alfa * v - beta * coefficients.at(j);
      }
      i -= 2;
    }
  }
  vec.erase(vec.begin());
  std::sort(vec.begin(), vec.end(),
            [eps](std::complex<double> c1, std::complex<double> c2) {
              if (std::fabs(c1.real() - c2.real()) <= eps) {
                return c1.imag() < c2.imag();
              }
              return c1.real() < c2.real();
            });

  return vec;
}

void testBracketRoot1() {
  double a = 0, b = 0;
  std::cout << "Test BracketRoot 1: "
            << BracketRoot([](double x) { return (x - 5) / 5; }, 0.1, a, b);
}

void testBracketRoot2() {
  double a = 0, b = 0;
  std::cout << "Test BracketRoot 2: "
            << BracketRoot([](double x) { return x * x * x + 1; }, 0.1, a, b);
}

void testBracketRoot3() {
  double a = 0, b = 0;
  std::cout << "Test BracketRoot 3 (exception): ";
  try {
    std::cout << BracketRoot([](double x) { return x * x * x + 1; }, 0.1, a, b,
                             -1);
  } catch (std::domain_error e) {
    std::cout << e.what() << std::endl;
  } catch (...) {
    std::cout << "Wrong exception" << std::endl;
  }
}

void testRegulaFalsi1() {
  auto f1 = [](double x) { return 0.05 * (std::exp(10 * (x - 3)) - 1); };
  std::cout << "Test RegulaFalsi 1:\nIllinois: "
            << RegulaFalsiSolve(f1, 0, 4, Illinois)
            << "\nSlavic: " << RegulaFalsiSolve(f1, 1, 4)
            << "\nIllinoisSlavic: "
            << RegulaFalsiSolve(f1, 0, 4, IllinoisSlavic) << std::endl;
}

void testRegulaFalsi2() {
  auto f1 = [](double x) { return 0.05 * (std::exp(10 * (x - 3)) - 1); };
  std::cout << "Test RegulaFalsi 2 (exception):\n";
  try {
    std::cout << "Unmodified: " << RegulaFalsiSolve(f1, 1, 4, Unmodified);
  } catch (std::logic_error e) {
    std::cout << e.what() << std::endl;
  } catch (...) {
    std::cout << "Wrong exception";
  }
}

void testRegulaFalsi3() {
  auto f1 = [](double x) { return 0.05 * (std::exp(10 * (x - 3)) - 1); };
  std::cout << "Test RegulaFalsi 3 (exception):\n";
  try {
    std::cout << RegulaFalsiSolve(f1, 1, 4, Unmodified, -1);
  } catch (std::domain_error e) {
    std::cout << e.what() << std::endl;
  } catch (...) {
    std::cout << "Wrong exception";
  }
}

void testRidders1() {
  auto f1 = [](double x) { return 0.05 * (std::exp(10 * (x - 3)) - 1); };
  std::cout << "Test Ridders 1: " << RiddersSolve(f1, 1, 4) << std::endl;
}

void testRidders2() {
  auto f1 = [](double x) { return 0.05 * (std::exp(10 * (x - 3)) - 1); };
  std::cout << "Test Ridders 2: ";
  try {
    RiddersSolve(f1, 1, 4, 1, -1);
  } catch (std::domain_error e) {
    std::cout << e.what() << std::endl;
  } catch (...) {
    std::cout << "Wrong exception";
  }
}

void testNewtonRaphson1() {
  auto f = [](double x) { return x * x + 5 * x + 6; };
  auto fprim = [](double x) { return 2 * x + 5; };
  std::cout << "Test Newton-Raphson 1: " << NewtonRaphsonSolve(f, fprim, 1)
            << std::endl;
}

void testNewtonRaphson2() {
  auto f = [](double x) { return x * x + 5 * x + 6; };
  auto fprim = [](double x) { return 2 * x + 5; };
  std::cout << "Test Newton-Raphson 2: ";
  try {
    NewtonRaphsonSolve(f, fprim, 1,-1);
  } catch (std::logic_error e) {
    std::cout << e.what() << std::endl;
  } catch (...) {
    std::cout << "Wrong exception";
  }
}

void testPolyRoots1() {
  std::cout << "Test PolyRoots 1: ";
  std::vector<std::complex<double>> comp{{-42, -9}, {29, -6}, {-8, 3}, 1};
  std::vector<std::complex<double>> x{{1, 2}, 3, {4, -5}};
  try {
  std::vector<std::complex<double>> z1 = PolyRoots(comp);
  for (int i = 0; i < z1.size(); i++)
    std::cout << (std::abs(z1[i] - x[i]) < 1e-8) << " ";
   } catch (std::logic_error e) {
    std::cout << e.what() << std::endl;
  } catch (...) {
    std::cout << "Wrong exception";
  }
}

void testPolyRoots2() {
  std::cout << "Test PolyRoots 2: ";
  std::vector<double> real{6, 11, 6, 1};
  std::vector<std::complex<double>> y{-3, -2, -1};
  std::vector<std::complex<double>> z2 = PolyRoots(real);
  for (int i = 0; i < z2.size(); i++)
    std::cout << (std::abs(z2[i] - y[i]) < 1e-8) << " ";
  std::cout << std::endl;
}

void testPolyRoots3() {
  std::cout << "Test PolyRoots 3: ";
  std::vector<double> real{6, 11, 6, 1};
  std::vector<std::complex<double>> y{-3, -2, -1};
  try {
  std::vector<std::complex<double>> z2 = PolyRoots(real,1,-1);
  } catch (std::domain_error e) {
    std::cout << e.what() << std::endl;
  } catch (...) {
    std::cout << "Wrong exception";
  }
}

int main() {
  testBracketRoot1();
  std::cout << std::endl;
  testBracketRoot2();
  std::cout << std::endl;
  testBracketRoot3();
  std::cout << std::endl;
  testRegulaFalsi1();
  std::cout << std::endl;
  testRegulaFalsi2();
  std::cout << std::endl;
  testRegulaFalsi3();
  std::cout << std::endl;
  testRidders1();
  std::cout << std::endl;
  testRidders2();
  std::cout << std::endl;
  testNewtonRaphson1();
  std::cout << std::endl;
  testNewtonRaphson2();
  std::cout << std::endl;
  testPolyRoots1();
  std::cout << std::endl;
  testPolyRoots2();
  std::cout << std::endl;
  testPolyRoots3();
  std::cout << std::endl;
  return 0;
}
