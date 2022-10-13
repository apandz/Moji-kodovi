#include <cmath>
#include <iostream>

using namespace std;

class GradjaninBiH {
  string ime;
  long long int jmbg;
  GradjaninBiH *pret;
  static GradjaninBiH *zadnji;
  static bool TestirajDatum(int da, int mj, int go) {
    int brdana = 28;
    if (mj == 4 || mj == 6 || mj == 9 || mj == 11)
      brdana = 30;
    if (mj != 2)
      brdana = 31;
    if ((go % 4 == 0 && go % 100 != 0) || go % 400 == 0)
      brdana = 29;
    if (mj < 1 || mj > 12 || go < 0 || da < 1 || da > brdana)
      return false;
    return true;
  }
  static bool Jmbg(int &ref, int br, long long int jmbg1) {
    int suma = 0, n = 0, god = 0, mjes = 0, dan = 0, sfr = 0, pol = 0, c13 = 0,
        c13j = jmbg1 % 10;
    for (int i = 13; i > 0; i--) {
      if (i == 6 || i == 12)
        c13 += 2 * (jmbg1 % 10);
      if (i == 5 || i == 11)
        c13 += 3 * (jmbg1 % 10);
      if (i == 4 || i == 10)
        c13 += 4 * (jmbg1 % 10);
      if (i == 3 || i == 9)
        c13 += 5 * (jmbg1 % 10);
      if (i == 2 || i == 8)
        c13 += 6 * (jmbg1 % 10);
      if (i == 1 || i == 7)
        c13 += 7 * (jmbg1 % 10);
      if (i < 13) {
        suma += (jmbg1 % 10) * pow(10, n);
        n++;
        if (i == 10) {
          if (suma < 500)
            pol = 1;
          else
            pol = 2;
          n = 0;
          suma = 0;
        }
        if (i == 8) {
          sfr = suma;
          n = 0;
          suma = 0;
        }
        if (i == 5) {
          god = 1000 + suma;
          n = 0;
          suma = 0;
        }
        if (i == 3) {
          mjes = suma;
          n = 0;
          suma = 0;
        }
        if (i == 1)
          dan = suma;
      }
      jmbg1 /= 10;
    }
    if (!TestirajDatum(dan, mjes, god))
      return false;
    if (jmbg1 != 0)
      return false;
    c13 = 11 - (c13 % 11);
    if (c13 == 10)
      return false;
    if (c13 == 11)
      c13 = 0;
    if (c13 != c13j % 10)
      return false;
    if (br == 1)
      ref = dan;
    if (br == 2)
      ref = mjes;
    if (br == 3)
      ref = god;
    if (br == 4)
      ref = sfr;
    if (br == 5)
      ref = pol;
    if (br == 6)
      ref = c13;
    return true;
  }
  static long long int KrJmbg(int d, int m, int g, int sfr, int p) {
    long long int jmbg1 = 0;
    int suma = 0;
    while (g >= 1000)
      g -= 1000;
    jmbg1 += pow(10, 11) * (d % 10);
    suma += 6 * (d % 10);
    jmbg1 += pow(10, 12) * (d / 10);
    suma += 7 * (d / 10);
    jmbg1 += pow(10, 9) * (m % 10);
    suma += 4 * (m % 10);
    jmbg1 += pow(10, 10) * (m / 10);
    suma += 5 * (m / 10);
    jmbg1 += pow(10, 6) * (g % 10);
    suma += 7 * (g % 10);
    jmbg1 += pow(10, 7) * ((g / 10) % 10);
    suma += 2 * ((g / 10) % 10);
    jmbg1 += pow(10, 8) * (g / 100);
    suma += 3 * (g / 100);
    jmbg1 += pow(10, 4) * (sfr % 10);
    suma += 5 * (sfr % 10);
    jmbg1 += pow(10, 5) * (sfr / 10);
    suma += 6 * (sfr / 10);
    if (p == 2) {
      jmbg1 += pow(10, 3) * 5;
      suma += 4 * 5;
    }
    int pomsum = suma, tri = 0, usl = 0;
    suma = 11 - (suma % 11);
    if (suma == 11)
      suma = 0;
    jmbg1 += suma;
    while (1) {
      tri = 0;
      if (p == 2)
        tri = 500;
      while (1) {
        if (!PronadjiJMBG(jmbg1, zadnji)) {
          usl = 1;
          break;
        }
        jmbg1 += 10;
        tri++;
      }
      if (usl == 1)
        break;
      if (tri != 0 && tri != 500) {
        jmbg1 -= suma;
        suma = pomsum;
        suma += 2 * (tri % 10);
        tri /= 10;
        suma += 3 * (tri % 10);
        tri /= 10;
        suma += 4 * tri;
        suma = 11 - (suma % 11);
        if (suma == 11)
          suma = 0;
        jmbg1 += suma;
      }
    }
    return jmbg1;
  }
  static bool PronadjiJMBG(long long int jmbg1, GradjaninBiH *zad) {
    GradjaninBiH *pok = nullptr;
    pok = zad;
    while (1) {
      if (pok == nullptr) {
        break;
      }
      if (pok->DajJMBG() == jmbg1)
        return true;
      pok = pok->pret;
    }
    return false;
  }

public:
  enum Pol { Musko, Zensko };
  GradjaninBiH(string ime1, long long int jmbg1, GradjaninBiH *gr = nullptr) {
    int br = 0;
    if (!Jmbg(br, 0, jmbg1))
      throw logic_error("JMBG nije validan");
    else if (PronadjiJMBG(jmbg1, zadnji)) {
      throw logic_error("Vec postoji gradjanin sa istim JMBG");
    } else {
      ime = ime1;
      jmbg = jmbg1;
    }
    zadnji = this;
    pret = gr;
  }
  GradjaninBiH(string ime1, int dan, int mj, int god, int sfr, Pol p,
               GradjaninBiH *gr = nullptr)
      : ime(ime1) {
    if (!TestirajDatum(dan, mj, god) || sfr < 0 || sfr > 99)
      throw logic_error("Neispravni podaci");
    int pp = 1;
    if (p == GradjaninBiH::Pol::Zensko)
      pp = 2;
    jmbg = KrJmbg(dan, mj, god, sfr, pp);
    if (PronadjiJMBG(jmbg, gr))
      throw logic_error("JMBG nije validan");
    pret = gr;
    zadnji = this;
  }
  ~GradjaninBiH() {
    if (zadnji == this) {
      zadnji = zadnji->pret;
    }
    GradjaninBiH *pok = zadnji;
    while (1) {
      if (pok == nullptr)
        break;
      if (pok->pret == this) {
        if (pret == nullptr) {
          pok->pret = nullptr;
          break;
        } else {
          pok->pret = pok->pret->pret;
          break;
        }
      }
      pok = pok->pret;
    }
  }
  GradjaninBiH(const GradjaninBiH &gr) = delete;
  GradjaninBiH &operator=(const GradjaninBiH &gr) = delete;
  string DajImeIPrezime() const { return ime; }
  long long int DajJMBG() const { return jmbg; }
  int DajDanRodjenja() const {
    int d;
    Jmbg(d, 1, jmbg);
    return d;
  }
  int DajMjesecRodjenja() const {
    int m;
    Jmbg(m, 2, jmbg);
    return m;
  }
  int DajGodinuRodjenja() const {
    int g;
    Jmbg(g, 3, jmbg);
    return g;
  }
  int DajSifruRegije() const {
    int sfr;
    Jmbg(sfr, 4, jmbg);
    return sfr;
  }
  Pol DajPol() const {
    int p;
    Jmbg(p, 5, jmbg);
    if (p == 1)
      return Pol::Musko;
    return Pol::Zensko;
  }
  void PromijeniImeIPrezime(string ime1) { ime = ime1; }
};

GradjaninBiH *GradjaninBiH::zadnji = nullptr;

int main() {
  int n = 0;
  cout << "Unesite broj gradjana koje zelite unijeti po JMBG: ";
  cin >> n;
  cin.ignore(10000, '\n');
  for (int i = 0; i < n; i++) {
    cout << "Unos podataka za " << i + 1 << ". gradjanina\n";
    cout << "Unesite ime i prezime (u istom redu): ";
    string ime;
    getline(cin, ime);
    cout << "Unesite JMBG: ";
    long long int jmbg;
    cin >> jmbg;
    try {
      GradjaninBiH gr(ime, jmbg);
    } catch (logic_error e) {
      cout << e.what() << endl;
      i--;
      continue;
    }
  }
  return 0;
}
