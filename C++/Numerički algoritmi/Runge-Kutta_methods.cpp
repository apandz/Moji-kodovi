#include <algorithm>
#include <cmath>
#include <complex>
#include <functional>
#include <iostream>
#include <limits>
#include <vector>

template <typename FunType>
double RK4Step(FunType f, double x, double y, double h) {
  double k1 = f(x, y);
  double k2 = f(x + h / 2, y + h * k1 / 2);
  double k3 = f(x + h / 2, y + h * k2 / 2);
  double k4 = f(x + h, y + h * k3);
  return y + h * (k1 + 2 * k2 + 2 * k3 + k4) / 6;
}

template <typename FunType>
std::vector<std::pair<double, double>>
RK4Integrator(FunType f, double x0, double y0, double xmax, double h,
              double eps = 1e-8, bool adaptive = false) {
  if ((h > 0 && xmax < x0) || (h <= 0 && xmax > x0))
    return {{x0, y0}};
  std::vector<std::pair<double, double>> co;
  if (!adaptive) {
    double x = x0, y = y0;
    if (h > 0) {
      while (x <= xmax + eps) {
        co.push_back({x, y});
        double k1 = f(x, y);
        double k2 = f(x + h / 2, y + h * k1 / 2);
        double k3 = f(x + h / 2, y + h * k2 / 2);
        double k4 = f(x + h, y + h * k3);
        y = y + h * (k1 + 2 * k2 + 2 * k3 + k4) / 6;
        x += h;
      }
    } else {
      while (x >= xmax - eps) {
        co.push_back({x, y});
        double k1 = f(x, y);
        double k2 = f(x + h / 2, y + h * k1 / 2);
        double k3 = f(x + h / 2, y + h * k2 / 2);
        double k4 = f(x + h, y + h * k3);
        y = y + h * (k1 + 2 * k2 + 2 * k3 + k4) / 6;
        x += h;
      }
    }
  } else {
    if (h > 0) {
      double x = x0, y = y0;
      co.push_back({x, y});
      while (x <= xmax + eps) {
        double u = RK4Step(f, x, y, h / 2);
        double v = RK4Step(f, x + h / 2, u, h / 2);
        double w = RK4Step(f, x, y, h);
        double delta = std::fabs(w - v) / h;
        if (delta <= eps) {
          x += h;
          y = v;
          co.push_back({x, y});
        }
        h = h * std::min(5.0, 0.9 * std::pow(eps / delta, 1 / 4.));
      }
      if (std::fabs(co.at(co.size() - 1).first - xmax) > eps) {
        co.erase(co.begin());
        h = xmax - co.at(co.size() - 1).first;
        double u = RK4Step(f, x, y, h / 2);
        double v = RK4Step(f, x + h / 2, u, h / 2);
        double w = RK4Step(f, x, y, h);
        co.at(co.size() - 1) = {xmax, v};
      }
    } else {
      double x = x0, y = y0;
      co.push_back({x, y});
      while (x >= xmax - eps) {
        double u = RK4Step(f, x, y, h / 2);
        double v = RK4Step(f, x + h / 2, u, h / 2);
        double w = RK4Step(f, x, y, h);
        double delta = std::fabs(w - v) / ((-1) * h);
        if (delta <= eps) {
          x += h;
          y = v;
          co.push_back({x, y});
        }
        h *= std::min(5.0, 0.9 * std::pow(eps / delta, 1 / 4.));
      }
      if (std::fabs(co.at(co.size() - 1).first - xmax) > eps) {
        co.erase(co.begin());
        h = xmax - co.at(co.size() - 1).first;
        double u = RK4Step(f, x, y, h / 2);
        double v = RK4Step(f, x + h / 2, u, h / 2);
        double w = RK4Step(f, x, y, h);
        co.at(co.size() - 1) = {xmax, v};
      }
    }
  }
  return co;
}

void test1() {
  auto res = RK4Integrator([](double x, double y) { return 2 * x + y + 1; }, 0,
                           1, 1.5, 0.1);
  auto exact = [](double x) { return -5 - 10 * x + 4 * std::exp(x); };
  std::cout << "Expected output:\n"
            << "0 1 -1\n0.1 1.22068 -1.57932\n0.2 1.48561 -2.11439\n0.3 "
               "1.79943 -2.60056\n0.4 2.1673 -3.0327\n0.5 2.59488 "
               "-3.40511\n0.6 3.08847 -3.71152\n0.7 3.65501 -3.94499\n0.8 "
               "4.30216 -4.09784\n0.9 5.03841 -4.16159\n1 5.87312 "
               "-4.12687\n1.1 6.81665 -3.98334\n1.2 7.88046 -3.71953\n1.3 "
               "9.07717 -3.32281\n1.4 10.4208 -2.7792\n1.5 11.9267 -2.07324"
            << std::endl;
  std::cout << "Actual output:\n";
  for (int i = 0; i < res.size(); i++) {
    double x = res[i].first;
    std::cout << x << " " << res[i].second << " " << exact(x) << std::endl;
  }
}

void test2() {
  auto res = RK4Integrator(
      [](double x, double y) { return 3 * x * x + 2 * y + 1; }, 0, 1, 3, 0.1);
  auto exact = [](double x) { return -7 * x + 3 * std::exp(x); };
  std::cout << "Expected output:\n"
            << "0 1 -1\n0.1 1.22068 -1.57932\n0.2 1.48561 -2.11439\n0.3 "
               "1.79943 -2.60056\n0.4 2.1673 -3.0327\n0.5 2.59488 "
               "-3.40511\n0.6 3.08847 -3.71152\n0.7 3.65501 -3.94499\n0.8 "
               "4.30216 -4.09784\n0.9 5.03841 -4.16159\n1 5.87312 "
               "-4.12687\n1.1 6.81665 -3.98334\n1.2 7.88046 -3.71953\n1.3 "
               "9.07717 -3.32281\n1.4 10.4208 -2.7792\n1.5 11.9267 -2.07324"
            << std::endl;
  std::cout << "Actual output:\n";
  for (int i = 0; i < res.size(); i++) {
    double x = res[i].first;
    std::cout << x << " " << res[i].second << " " << exact(x) << std::endl;
  }
}

void test3() {
  auto res = RK4Integrator([](double x, double y) { return 7 * x + y + 7; }, 0,
                           1, -0.8, -0.1, 1e-8, true);
  auto exact = [](double x) { return -3 - 2 * x + 4 * std::exp(x); };
  std::cout << "Expected output:\n"
            << "-0.0154357 0.878292 0.969603\n-0.0308262 0.760444 "
               "0.940229\n-0.0462763 0.645606 0.911665\n-0.061786 0.533763 "
               "0.883908\n-0.0773557 0.424899 0.856954\n-0.0929859 0.318997 "
               "0.830797\n-0.108677 0.21604 0.805434\n-0.12443 0.116012 "
               "0.780861\n-0.140244 0.0188976 0.757072\n-0.156121 -0.0753207 "
               "0.734064\n-0.172061 -0.166659 0.711833\n-0.188065 -0.255132 "
               "0.690374\n-0.204132 -0.340756 0.669683\n-0.220264 -0.423548 "
               "0.649756\n-0.236461 -0.503522 0.630589\n-0.252723 -0.580695 "
               "0.612178\n-0.269051 -0.655081 0.594519\n-0.285446 -0.726695 "
               "0.577607\n-0.301908 -0.795554 0.56144\n-0.318438 -0.861671 "
               "0.546013\n-0.335036 -0.925062 0.531322\n-0.351702 -0.985741 "
               "0.517363\n-0.368438 -1.04372 0.504132\n-0.385244 -1.09902 "
               "0.491627\n-0.40212 -1.15165 0.479842\n-0.419068 -1.20163 "
               "0.468774\n-0.436087 -1.24897 0.45842\n-0.453178 -1.29368 "
               "0.448776\n-0.470343 -1.33578 0.439838\n-0.487581 -1.37528 "
               "0.431603\n-0.504893 -1.4122 0.424067\n-0.522279 -1.44654 "
               "0.417227\n-0.539742 -1.47833 0.411079\n-0.55728 -1.50757 "
               "0.405619\n-0.574895 -1.53428 0.400846\n-0.592588 -1.55847 "
               "0.396754\n-0.610359 -1.58015 0.393341\n-0.628209 -1.59934 "
               "0.390604\n-0.646138 -1.61605 0.388539\n-0.664147 -1.63029 "
               "0.387144\n-0.682238 -1.64207 0.386414\n-0.70041 -1.6514 "
               "0.386347\n-0.718664 -1.65831 0.38694\n-0.737002 -1.66279 "
               "0.38819\n-0.755423 -1.66486 0.390093\n-0.773929 -1.66454 "
               "0.392648\n-0.792521 -1.66182 0.39585\n-0.8 -1.66007 0.397316"
            << std::endl;
  std::cout << "Actual output:\n";
  for (int i = 0; i < res.size(); i++) {
    double x = res[i].first;
    std::cout << x << " " << res[i].second << " " << exact(x) << std::endl;
  }
}

int main() {
  test1();
  std::cout << std::endl;
  test2();
  std::cout << std::endl;
  test3();
  return 0;
}
