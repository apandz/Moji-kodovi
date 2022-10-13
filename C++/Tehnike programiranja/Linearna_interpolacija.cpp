#include <algorithm>
#include <cmath>
#include <functional>
#include <iostream>
#include <iterator>
#include <vector>

using namespace std;

function<double(double)> LinearnaInterpolacija(vector<pair<double, double>> v) {
  const double eps = 1e-10;
  sort(v.begin(), v.end(),
       [eps](pair<double, double> a, pair<double, double> b) {
         if (fabs(a.first - b.first) <= eps * (fabs(a.first) + fabs(b.first)))
           throw domain_error("Neispravni cvorovi.");
         if (a.first - b.first > eps * (a.first + b.first))
           return false;
         return true;
       });
  return [v, eps](double x) {
    double xx = 0;
    if ((v.at(0).first - x > eps * (v.at(0).first + x)) ||
        (x - v.at(v.size() - 1).first > eps * (v.at(v.size() - 1).first + x)))
      throw range_error("\nArgument izvan opsega.");
    if (fabs(x - v.at(v.size() - 1).first) <=
        eps * (fabs(x) + fabs(v.at(v.size() - 1).first)))
      xx = v.at(v.size() - 1).second;
    else {
      pair<double, double> p = make_pair(x, 1000);
      auto it = lower_bound(v.begin(), v.end(), p);
      int br = it - v.begin();
      if (br > 0)
        xx = v.at(br - 1).second + (v.at(br).second - v.at(br - 1).second) *
                                       (x - v.at(br - 1).first) /
                                       (v.at(br).first - v.at(br - 1).first);
    }
    return xx;
  };
}

function<double(double)> LinearnaInterpolacija(function<double(double)> f,
                                               double xmin, double xmax,
                                               double delta) {
  const double eps = 1e-10;
  if (xmin - xmax > eps * (xmin + xmax) || 0 - delta > eps * (delta + 0) ||
      fabs(delta - 0) <= eps * (fabs(delta) + 0))
    throw domain_error("Nekorektni parametri.");
  vector<pair<double, double>> v;
  double min = xmin;
  while (1) {
    v.push_back(make_pair(min, f(min)));
    if (fabs(min - xmax) <= eps * (fabs(min) + fabs(xmax)))
      break;
    min += delta;
  }
  return [v, eps](double x) {
    double xx = 0;
    if ((v.at(0).first - x > eps * (v.at(0).first + x)) ||
        (x - v.at(v.size() - 1).first > eps * (v.at(v.size() - 1).first + x)))
      throw range_error("\nArgument izvan opsega.");
    if (fabs(x - v.at(v.size() - 1).first) <=
        eps * (fabs(x) + fabs(v.at(v.size() - 1).first)))
      xx = v.at(v.size() - 1).second;
    else {
      pair<double, double> p = make_pair(x, 1000);
      auto it = lower_bound(v.begin(), v.end(), p);
      int br = it - v.begin();
      if (br > 0)
        xx = v.at(br - 1).second + (v.at(br).second - v.at(br - 1).second) *
                                       (x - v.at(br - 1).first) /
                                       (v.at(br).first - v.at(br - 1).first);
    }
    return xx;
  };
}

int main() {
  cout << "Odaberite opciju (1 - unos cvorova, 2 - aproksimacija): ";
  int n = 0;
  cin >> n;
  cout << endl;
  if (n == 1) {
    cout << "Unesite broj cvorova: ";
    cin >> n;
    cout << endl;
    cout << "Unesite cvorove kao parove x i y:";
    try {
      vector<pair<double, double>> v(n);
      for (int i = 0; i < n; i++) {
        double x, y;
        cin >> x >> y;
        v.at(i) = make_pair(x, y);
      }
      cout << endl;
      try {
        auto f = LinearnaInterpolacija(v);
        while (1) {
          cout << "Unesite argument (ili \"kraj\" za kraj): ";
          double br = 0;
          cin >> br;
          if (!cin)
            break;
          try {
            cout << endl << "f(" << br << ") = " << f(br) << endl;
          } catch (range_error e) {
            cout << e.what() << endl;
          } catch (domain_error e) {
            cout << e.what() << endl;
          }
        }
      } catch (range_error e) {
        cout << e.what() << endl;
        return 0;
      } catch (domain_error e) {
        cout << e.what() << endl;
        return 0;
      }
    } catch (...) {
      cout << "Nedovoljno memorije" << endl;
      return 0;
    }
  }
  if (n == 2) {
    cout << "Unesite krajeve intervala i korak: ";
    double xmin = 0, xmax = 0, delta = 0;
    cin >> xmin >> xmax >> delta;
    cout << endl;
    try {
      auto f = LinearnaInterpolacija([](double x) { return x * x + sin(x); },
                                     xmin, xmax, delta);

      while (1) {
        cout << "Unesite argument (ili \"kraj\" za kraj): ";
        double br = 0;
        cin >> br;
        if (!cin)
          break;
        try {
          cout << endl
               << "f(" << br << ") = " << br * br + sin(br) << " "
               << "f_approx(" << br << ") = " << f(br) << endl;
        } catch (range_error e) {
          cout << e.what() << endl;
        } catch (domain_error e) {
          cout << e.what() << endl;
        }
      }
    } catch (range_error e) {
      cout << e.what() << endl;
      return 0;
    } catch (domain_error e) {
      cout << e.what() << endl;
      return 0;
    }
  }
  return 0;
}
