#include <algorithm>
#include <cmath>
#include <iostream>
#include <memory>
#include <utility>
#include <vector>

using namespace std;

typedef pair<double, double> Tacka;

class Trougao {
  Tacka t1, t2, t3;
  static double Izraz(Tacka t1, Tacka t2, Tacka t3) {
    const double eps = 1e-10;
    t1.first += eps;
    t1.second += eps;
    t2.first += eps;
    t2.second += eps;
    t3.first += eps;
    t3.second += eps;
    return t1.first * (t2.second - t3.second) -
           t2.first * (t1.second - t3.second) +
           t3.first * (t1.second - t2.second);
  }
  static double Duzina(Tacka t1, Tacka t2) {
    const double eps = 1e-10;
    t1.first += eps;
    t1.second += eps;
    t2.first += eps;
    t2.second += eps;
    return sqrt((t1.first - t2.first) * (t1.first - t2.first) +
                (t1.second - t2.second) * (t1.second - t2.second));
  }
  static double Ugao(Tacka tjeme, Tacka dr1, Tacka dr2) {
    double a, b, c;
    a = Duzina(tjeme, dr1);
    b = Duzina(tjeme, dr2);
    c = Duzina(dr1, dr2);
    return acos((a * a + b * b - c * c) / (2 * a * b));
  }
  static Tacka RotTacka(Tacka t, Tacka tc, double u) {
    return {tc.first + (t.first - tc.first) * cos(u) -
                (t.second - tc.second) * sin(u),
            tc.second + (t.first - tc.first) * sin(u) +
                (t.second - tc.second) * cos(u)};
  }
  static Tacka SkTacka(Tacka t, Tacka tc, double k) {
    return {tc.first + (t.first - tc.first) * k,
            tc.second + (t.second - tc.second) * k};
  }
  static bool Isti(double x1, double x2) {
    const double eps = 1e-10;
    return fabs(x1 - x2) <= eps * (fabs(x1) + fabs(x2));
  }
  static bool IsteTacke(Tacka t1, Tacka t2) {
    return Isti(t1.first, t2.first) && Isti(t1.second, t2.second);
  }

public:
  Trougao(Tacka ta1, Tacka ta2, Tacka ta3) {
    if (Orijentacija(ta1, ta2, ta3) == 0)
      throw domain_error("Nekorektne pozicije tjemena");
    t1 = ta1;
    t2 = ta2;
    t3 = ta3;
  }
  void Postavi(Tacka ta1, Tacka ta2, Tacka ta3) {
    if (Orijentacija(ta1, ta2, ta3) == 0)
      throw domain_error("Nekorektne pozicije tjemena");
    t1 = ta1;
    t2 = ta2;
    t3 = ta3;
  }
  void Postavi(int br, Tacka t) {
    if (br == 1) {
      if (Orijentacija(t, t2, t3) == 0)
        throw domain_error("Nekorektne pozicije tjemena");
      t1 = t;
    } else if (br == 2) {
      if (Orijentacija(t1, t, t3) == 0)
        throw domain_error("Nekorektne pozicije tjemena");
      t2 = t;
    } else if (br == 3) {
      if (Orijentacija(t1, t2, t) == 0)
        throw domain_error("Nekorektne pozicije tjemena");
      t3 = t;
    } else {
      throw range_error("Nekorektan indeks");
    }
  }
  static int Orijentacija(Tacka ta1, Tacka ta2, Tacka ta3) {
    const double eps = 1e-10;
    if (Izraz(ta1, ta2, ta3) - 0 < eps * (Izraz(ta1, ta2, ta3) + 0))
      return -1;
    if (Izraz(ta1, ta2, ta3) - 0 > eps * (Izraz(ta1, ta2, ta3) + 0))
      return 1;
    return 0;
  }
  Tacka DajTjeme(int br) const {
    if (br == 1) {
      return t1;
    } else if (br == 2) {
      return t2;
    } else if (br == 3) {
      return t3;
    } else {
      throw range_error("Nekorektan indeks");
    }
  }
  double DajStranicu(int br) const {
    if (br == 1) {
      return Duzina(t2, t3);
    } else if (br == 2) {
      return Duzina(t1, t3);
    } else if (br == 3) {
      return Duzina(t1, t2);
    } else {
      throw range_error("Nekorektan indeks");
    }
  }
  double DajUgao(int br) const {
    if (br == 1) {
      return Ugao(t1, t2, t3);
    } else if (br == 2) {
      return Ugao(t2, t1, t3);
    } else if (br == 3) {
      return Ugao(t3, t2, t1);
    } else {
      throw range_error("Nekorektan indeks");
    }
  }
  Tacka DajCentar() const {
    return {(t1.first + t2.first + t3.first) / 3.,
            (t1.second + t2.second + t3.second) / 3.};
  }
  double DajObim() const {
    return DajStranicu(1) + DajStranicu(2) + DajStranicu(3);
  }
  double DajPovrsinu() const { return fabs(Izraz(t1, t2, t3)) / 2.; }
  bool DaLiJePozitivnoOrijentiran() const {
    if (Orijentacija(t1, t2, t3) == 1)
      return true;
    return false;
  }
  bool DaLiJeUnutra(Tacka t) const {
    if (Orijentacija(t1, t2, t) != Orijentacija(t1, t2, t3) ||
        Orijentacija(t2, t3, t) != Orijentacija(t1, t2, t3) ||
        Orijentacija(t3, t1, t) != Orijentacija(t1, t2, t3))
      return false;
    return true;
  }
  void Ispisi() const {
    cout << "((" << t1.first << "," << t1.second << "),(" << t2.first << ","
         << t2.second << "),(" << t3.first << "," << t3.second << "))";
  }
  void Transliraj(double x, double y) {
    t1.first += x;
    t1.second += y;
    t2.first += x;
    t2.second += y;
    t3.first += x;
    t3.second += y;
  }
  void Centriraj(Tacka t) {
    double x, y;
    x = t.first - DajCentar().first;
    y = t.second - DajCentar().second;
    Transliraj(x, y);
  }
  void Rotiraj(Tacka t, double u) {
    t1 = RotTacka(t1, t, u);
    t2 = RotTacka(t2, t, u);
    t3 = RotTacka(t3, t, u);
  }
  void Skaliraj(Tacka t, double k) {
    const double eps = 1e-10;
    if (fabs(k - 0) <= eps * (fabs(k)))
      throw domain_error("Nekorektan faktor skaliranja");
    t1 = SkTacka(t1, t, k);
    t2 = SkTacka(t2, t, k);
    t3 = SkTacka(t3, t, k);
  }
  void Rotiraj(double u) {
    Tacka t = DajCentar();
    t1 = RotTacka(t1, t, u);
    t2 = RotTacka(t2, t, u);
    t3 = RotTacka(t3, t, u);
  }
  void Skaliraj(double k) {
    const double eps = 1e-10;
    if (fabs(k - 0) <= eps * (fabs(k)))
      throw domain_error("Nekorektan faktor skaliranja");
    Tacka t = DajCentar();
    t1 = SkTacka(t1, t, k);
    t2 = SkTacka(t2, t, k);
    t3 = SkTacka(t3, t, k);
  }
  friend bool DaLiSuIdenticni(const Trougao &t1, const Trougao &t2);
  friend bool DaLiSuPodudarni(const Trougao &t1, const Trougao &t2);
  friend bool DaLiSuSlicni(const Trougao &t1, const Trougao &t2);
};

bool DaLiSuIdenticni(const Trougao &t1, const Trougao &t2) {
  if (Trougao::IsteTacke(t1.DajTjeme(1), t2.DajTjeme(1)) &&
      Trougao::IsteTacke(t1.DajTjeme(2), t2.DajTjeme(2)) &&
      Trougao::IsteTacke(t1.DajTjeme(3), t2.DajTjeme(3)))
    return true;
  if (Trougao::IsteTacke(t1.DajTjeme(1), t2.DajTjeme(1)) &&
      Trougao::IsteTacke(t1.DajTjeme(3), t2.DajTjeme(2)) &&
      Trougao::IsteTacke(t1.DajTjeme(2), t2.DajTjeme(3)))
    return true;
  if (Trougao::IsteTacke(t1.DajTjeme(2), t2.DajTjeme(1)) &&
      Trougao::IsteTacke(t1.DajTjeme(1), t2.DajTjeme(2)) &&
      Trougao::IsteTacke(t1.DajTjeme(3), t2.DajTjeme(3)))
    return true;
  if (Trougao::IsteTacke(t1.DajTjeme(2), t2.DajTjeme(1)) &&
      Trougao::IsteTacke(t1.DajTjeme(3), t2.DajTjeme(2)) &&
      Trougao::IsteTacke(t1.DajTjeme(1), t2.DajTjeme(3)))
    return true;
  if (Trougao::IsteTacke(t1.DajTjeme(3), t2.DajTjeme(1)) &&
      Trougao::IsteTacke(t1.DajTjeme(1), t2.DajTjeme(2)) &&
      Trougao::IsteTacke(t1.DajTjeme(2), t2.DajTjeme(3)))
    return true;
  if (Trougao::IsteTacke(t1.DajTjeme(3), t2.DajTjeme(1)) &&
      Trougao::IsteTacke(t1.DajTjeme(2), t2.DajTjeme(2)) &&
      Trougao::IsteTacke(t1.DajTjeme(1), t2.DajTjeme(3)))
    return true;
  return false;
}

bool DaLiSuSlicni(const Trougao &t1, const Trougao &t2) {
  if (t2.Orijentacija(t2.DajTjeme(1), t2.DajTjeme(2), t2.DajTjeme(3)) ==
      t1.Orijentacija(t1.DajTjeme(1), t1.DajTjeme(2), t1.DajTjeme(3))) {
    if (Trougao::Isti(t1.DajStranicu(1) / t2.DajStranicu(1),
                      t1.DajStranicu(2) / t2.DajStranicu(2)) &&
        Trougao::Isti(t1.DajStranicu(3) / t2.DajStranicu(3),
                      t1.DajStranicu(2) / t2.DajStranicu(2)))
      return true;
    if (Trougao::Isti(t1.DajStranicu(2) / t2.DajStranicu(1),
                      t1.DajStranicu(3) / t2.DajStranicu(2)) &&
        Trougao::Isti(t1.DajStranicu(1) / t2.DajStranicu(3),
                      t1.DajStranicu(3) / t2.DajStranicu(2)))
      return true;
    if (Trougao::Isti(t1.DajStranicu(3) / t2.DajStranicu(1),
                      t1.DajStranicu(1) / t2.DajStranicu(2)) &&
        Trougao::Isti(t1.DajStranicu(2) / t2.DajStranicu(3),
                      t1.DajStranicu(1) / t2.DajStranicu(2)))
      return true;
  } else {
    if (Trougao::Isti(t1.DajStranicu(1) / t2.DajStranicu(1),
                      t1.DajStranicu(3) / t2.DajStranicu(2)) &&
        Trougao::Isti(t1.DajStranicu(2) / t2.DajStranicu(3),
                      t1.DajStranicu(3) / t2.DajStranicu(2)))
      return true;
    if (Trougao::Isti(t1.DajStranicu(2) / t2.DajStranicu(1),
                      t1.DajStranicu(1) / t2.DajStranicu(2)) &&
        Trougao::Isti(t1.DajStranicu(3) / t2.DajStranicu(3),
                      t1.DajStranicu(1) / t2.DajStranicu(2)))
      return true;
    if (Trougao::Isti(t1.DajStranicu(3) / t2.DajStranicu(1),
                      t1.DajStranicu(2) / t2.DajStranicu(2)) &&
        Trougao::Isti(t1.DajStranicu(1) / t2.DajStranicu(3),
                      t1.DajStranicu(2) / t2.DajStranicu(2)))
      return true;
  }
  return false;
}

bool DaLiSuPodudarni(const Trougao &t1, const Trougao &t2) {
  if (t2.Orijentacija(t2.DajTjeme(1), t2.DajTjeme(2), t2.DajTjeme(3)) ==
      t1.Orijentacija(t1.DajTjeme(1), t1.DajTjeme(2), t1.DajTjeme(3))) {
    if (Trougao::Isti(t1.DajStranicu(1), t2.DajStranicu(1)) &&
        Trougao::Isti(t1.DajStranicu(2), t2.DajStranicu(2)) &&
        Trougao::Isti(t1.DajStranicu(3), t2.DajStranicu(3)))
      return true;
    if (Trougao::Isti(t1.DajStranicu(2), t2.DajStranicu(1)) &&
        Trougao::Isti(t1.DajStranicu(3), t2.DajStranicu(2)) &&
        Trougao::Isti(t1.DajStranicu(1), t2.DajStranicu(3)))
      return true;
    if (Trougao::Isti(t1.DajStranicu(3), t2.DajStranicu(1)) &&
        Trougao::Isti(t1.DajStranicu(1), t2.DajStranicu(2)) &&
        Trougao::Isti(t1.DajStranicu(2), t2.DajStranicu(3)))
      return true;
  } else {
    if (Trougao::Isti(t1.DajStranicu(1), t2.DajStranicu(1)) &&
        Trougao::Isti(t1.DajStranicu(3), t2.DajStranicu(2)) &&
        Trougao::Isti(t1.DajStranicu(2), t2.DajStranicu(3)))
      return true;
    if (Trougao::Isti(t1.DajStranicu(3), t2.DajStranicu(1)) &&
        Trougao::Isti(t1.DajStranicu(2), t2.DajStranicu(2)) &&
        Trougao::Isti(t1.DajStranicu(1), t2.DajStranicu(3)))
      return true;
    if (Trougao::Isti(t1.DajStranicu(2), t2.DajStranicu(1)) &&
        Trougao::Isti(t1.DajStranicu(1), t2.DajStranicu(2)) &&
        Trougao::Isti(t1.DajStranicu(3), t2.DajStranicu(3)))
      return true;
  }
  return false;
}

bool Manji(double x1, double x2) {
  const double eps = 1e-10;
  if (fabs(x1 - x2) <= eps * (fabs(x1) + fabs(x2))) {
    return false;
  }
  return (x1 - x2) < eps * (x1 + x2);
}

int main() {
  int n = 0;
  cout << "Unesite broj trouglova: ";
  cin >> n;
  try {
    vector<shared_ptr<Trougao>> v(n);
    for (int i = 0; i < n; i++) {
      double x1, y1, x2, y2, x3, y3;
      cout << "Unesite vrhove " << i + 1 << ". trougla [x1 y1 x2 y2 x3 y3]: ";
      cin >> x1 >> y1 >> x2 >> y2 >> x3 >> y3;
      try {
        v.at(i) = make_shared<Trougao>(Trougao({x1, y1}, {x2, y2}, {x3, y3}));
      } catch (domain_error e) {
        cout << e.what() << ", ponovite unos!" << endl;
        i--;
      } catch (range_error e) {
        cout << e.what() << ", ponovite unos!" << endl;
        i--;
      }
    }
    cout << "\nTrouglovi prije transformacija:\n";
    for_each(v.begin(), v.end(), [](const shared_ptr<Trougao> &tr) {
      tr->Ispisi();
      cout << endl;
    });
    cout << "\nUnesite vektor translacije [dx dy]:  ";
    double dx, dy;
    cin >> dx >> dy;
    cout << "Unesite ugao rotacije: ";
    double alfa;
    cin >> alfa;
    cout << "Unesite faktor skaliranja: ";
    double fakt;
    cin >> fakt;
    transform(v.begin(), v.end(), v.begin(),
              [dx, dy, alfa, fakt](shared_ptr<Trougao> tr) {
                tr->Transliraj(dx, dy);
                tr->Rotiraj(alfa);
                tr->Skaliraj(tr->DajTjeme(1), fakt);
                return tr;
              });
    stable_sort(
        v.begin(), v.end(),
        [](const shared_ptr<Trougao> &tr1, const shared_ptr<Trougao> &tr2) {
          return Manji(tr1->DajPovrsinu(), tr2->DajPovrsinu());
        });
    cout << "\nTrouglovi nakon transformacija i sortiranja:\n";
    for_each(v.begin(), v.end(), [](const shared_ptr<Trougao> &tr) {
      tr->Ispisi();
      cout << endl;
    });
    cout << "\nVrhovi trougla sa najmanjim obimom: ";
    (*min_element(
         v.begin(), v.end(),
         [](const shared_ptr<Trougao> &tr1, const shared_ptr<Trougao> &tr2) {
           return Manji(tr1->DajObim(), tr2->DajObim());
         }))
        ->Ispisi();
    cout << endl << endl;
    int usl = 0;
    for (int i = 0; i < n; i++) {
      for (int j = i + 1; j < n; j++) {
        if (DaLiSuIdenticni(*v.at(i), *v.at(j))) {
          if (usl == 0) {
            cout << "Parovi identicnih trouglova:\n";
            usl++;
          }
          v.at(i)->Ispisi();
          cout << " i ";
          v.at(j)->Ispisi();
          cout << endl;
        }
      }
    }
    if (usl == 0)
      cout << "Nema identicnih trouglova!\n";
    usl = 0;
    cout << endl;
    for (int i = 0; i < n; i++) {
      for (int j = i + 1; j < n; j++) {
        if (DaLiSuPodudarni(*v.at(i), *v.at(j))) {
          if (usl == 0) {
            cout << "Parovi podudarnih trouglova:\n";
            usl++;
          }
          v.at(i)->Ispisi();
          cout << " i ";
          v.at(j)->Ispisi();
          cout << endl;
        }
      }
    }
    if (usl == 0)
      cout << "Nema podudarnih trouglova!\n";
    usl = 0;
    cout << endl;
    for (int i = 0; i < n; i++) {
      for (int j = i + 1; j < n; j++) {
        if (DaLiSuSlicni(*v.at(i), *v.at(j))) {
          if (usl == 0) {
            cout << "Parovi slicnih trouglova:\n";
            usl++;
          }
          v.at(i)->Ispisi();
          cout << " i ";
          v.at(j)->Ispisi();
          cout << endl;
        }
      }
    }
    if (usl == 0)
      cout << "Nema slicnih trouglova!\n";
  } catch (...) {
  }
  return 0;
}
