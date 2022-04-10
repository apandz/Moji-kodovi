#include <iostream>
#include <vector>

using namespace std;

template <typename TipOznake> class Grana;
template <typename TipOznake> class Cvor;
template <typename TipOznake> class GranaIterator;

template <typename TipOznake> class UsmjereniGraf {
  UsmjereniGraf(const UsmjereniGraf &ug);
  UsmjereniGraf(UsmjereniGraf &&ug);
  UsmjereniGraf &operator=(const UsmjereniGraf &ug);
  UsmjereniGraf &operator=(UsmjereniGraf &&ug);

public:
  UsmjereniGraf(int n) {}
  virtual ~UsmjereniGraf() {}
  virtual int dajBrojCvorova() const = 0;
  virtual void postaviBrojCvorova(int n) = 0;
  virtual void dodajGranu(int pcv, int dcv, double tez = 0) = 0;
  virtual void obrisiGranu(int pcv, int dcv) = 0;
  virtual void postaviTezinuGrane(int pcv, int dcv, double tez = 0) = 0;
  virtual double dajTezinuGrane(int pcv, int dcv) = 0;
  virtual bool postojiGrana(int pcv, int dcv) = 0;
  virtual void postaviOznakuCvora(int brcv, TipOznake oznaka) = 0;
  virtual TipOznake dajOznakuCvora(int brcv) = 0;
  virtual void postaviOznakuGrane(int pcv, int dcv, TipOznake oznaka) = 0;
  virtual TipOznake dajOznakuGrane(int pcv, int dcv) = 0;
  virtual Grana<TipOznake> dajGranu(int pcv, int dcv) = 0;
  virtual Cvor<TipOznake> dajCvor(int brcv) = 0;
  virtual GranaIterator<TipOznake> dajGranePocetak() = 0;
  virtual GranaIterator<TipOznake> dajGraneKraj() = 0;
};

template <typename TipOznake> class Grana {
  TipOznake oz;
  bool ozn;
  double tez;
  int pcv, dcv;
  UsmjereniGraf<TipOznake> *graf;

public:
  Grana(int pcv, int dcv, UsmjereniGraf<TipOznake> *graf, double tez = 0,
        bool ozn = false, TipOznake oz = TipOznake())
      : pcv(pcv), dcv(dcv), graf(graf), tez(tez), ozn(ozn), oz(oz) {}
  double dajTezinu() { return tez; }
  void postaviTezinu(double tez) {
    this->tez = tez;
    graf->postaviTezinuGrane(pcv, dcv, tez);
  }
  TipOznake dajOznaku() {
    if (!ozn) {
      return TipOznake();
    }
    return oz;
  }
  void postaviOznaku(TipOznake oz) {
    this->oz = oz;
    ozn = true;
    graf->postaviOznakuGrane(pcv, dcv, oz);
  }
  Cvor<TipOznake> dajPolazniCvor() { return graf->dajCvor(pcv); }
  Cvor<TipOznake> dajDolazniCvor() { return graf->dajCvor(dcv); }
  bool operator==(const Grana &grana) const {
    return grana.ozn == this->ozn && grana.oz == this->oz &&
           grana.tez == this->tez && grana.pcv == this->pcv &&
           grana.dcv == this->dcv && grana.graf == this->graf;
  }
  UsmjereniGraf<TipOznake> *dajGraf() { return graf; }
};

template <typename TipOznake> class Cvor {
  TipOznake oz;
  bool ozn;
  UsmjereniGraf<TipOznake> *graf;
  int rbr;

public:
  Cvor(int rbr, UsmjereniGraf<TipOznake> *graf, bool ozn = false,
       TipOznake oz = TipOznake())
      : graf(graf), rbr(rbr), ozn(ozn), oz(oz) {}
  TipOznake dajOznaku() {
    if (!ozn) {
      return TipOznake();
    }
    return oz;
  }
  void postaviOznaku(TipOznake oz) {
    graf->postaviOznakuCvora(rbr, oz);
    this->oz = oz;
    ozn = true;
  }
  int dajRedniBroj() { return rbr; }
};

template <typename TipOznake> class GranaIterator {
  Grana<TipOznake> grana;
  UsmjereniGraf<TipOznake> *graf;

public:
  GranaIterator(Grana<TipOznake> grana) : grana(grana), graf(grana.dajGraf()) {}
  Grana<TipOznake> operator*() { return grana; }
  bool operator==(const GranaIterator &iter) const {
    return iter.grana == this->grana;
  }
  bool operator!=(const GranaIterator &iter) const {
    return !(iter.grana == this->grana);
  }
  GranaIterator &operator++() {
    int ukcv = graf->dajBrojCvorova();
    int pcv = grana.dajPolazniCvor().dajRedniBroj();
    int dcv = grana.dajDolazniCvor().dajRedniBroj();
    bool nadjena = false;
    for (int i = pcv; i < ukcv; i++) {
      int j = 0;
      if (i == pcv)
        j = dcv + 1;
      for (; j < ukcv; j++) {
        if (graf->postojiGrana(i, j)) {
          grana = graf->dajGranu(i, j);
          nadjena = true;
          break;
        }
      }
      if (nadjena == true) {
        break;
      }
    }
    return *this;
  }
  GranaIterator operator++(int) {
    GranaIterator gi(*this);
    ++(*this);
    return gi;
  }
};

template <typename TipOznake>
class MatricaGraf : public UsmjereniGraf<TipOznake> {
  vector<pair<bool, TipOznake>> cvorovi;
  vector<vector<pair<bool, pair<double, pair<bool, TipOznake>>>>> grane;
  int brgr;

  bool postojiCvor(int brcv) {
    if (brcv < 0 || brcv >= cvorovi.size()) {
      return false;
    }
    return true;
  }

public:
  MatricaGraf(int n) : UsmjereniGraf<TipOznake>(n), brgr(0) {
    grane.resize(n);
    for (int i = 0; i < n; i++) {
      grane.at(i).resize(n);
      cvorovi.push_back({false, TipOznake()});
    }
  }
  int dajBrojCvorova() const override { return cvorovi.size(); }
  void postaviBrojCvorova(int n) override {
    if (n < this->cvorovi.size())
      throw domain_error("Pokušaj smanjenja broja čvorova");
    grane.resize(n);
    for (int i = cvorovi.size(); i < n; i++) {
      cvorovi.push_back({false, TipOznake()});
      grane.at(i).resize(n);
    }
  }
  void dodajGranu(int pcv, int dcv, double tez = 0) override {
    if (!postojiGrana(pcv, dcv)) {
      grane.at(pcv).at(dcv) = {true, {tez, {false, TipOznake()}}};
      brgr++;
    } // else throw domain_error("Grana već postoji");
  }
  void obrisiGranu(int pcv, int dcv) override {
    if (postojiGrana(pcv, dcv)) {
      grane.at(pcv).at(dcv).first = false;
      brgr--;
    }
  }
  void postaviTezinuGrane(int pcv, int dcv, double tez = 0) override {
    if (postojiGrana(pcv, dcv)) {
      grane.at(pcv).at(dcv).second.first = tez;
    }
  }
  double dajTezinuGrane(int pcv, int dcv) override {
    if (postojiGrana(pcv, dcv)) {
      return grane.at(pcv).at(dcv).second.first;
    }
    return 0;
  }
  bool postojiGrana(int pcv, int dcv) override {
    if (pcv < 0 || dcv < 0 || pcv >= cvorovi.size() || dcv >= cvorovi.size()) {
      throw domain_error("Neispravni parametri");
    }
    return grane.at(pcv).at(dcv).first;
  }
  void postaviOznakuCvora(int brcv, TipOznake oznaka) override {
    if (postojiCvor(brcv)) {
      cvorovi.at(brcv) = {true, oznaka};
    }
  }
  TipOznake dajOznakuCvora(int brcv) override {
    if (postojiCvor(brcv)) {
      return cvorovi.at(brcv).second;
    }
    return TipOznake();
  }
  void postaviOznakuGrane(int pcv, int dcv, TipOznake oznaka) override {
    if (postojiGrana(pcv, dcv)) {
      grane.at(pcv).at(dcv).second.second = {true, oznaka};
    }
  }
  TipOznake dajOznakuGrane(int pcv, int dcv) override {
    if (!postojiGrana(pcv, dcv)) {
      return TipOznake();
    }
    return grane.at(pcv).at(dcv).second.second.second;
  }
  Grana<TipOznake> dajGranu(int pcv, int dcv) override {
    if (!postojiGrana(pcv, dcv)) {
      throw domain_error("Ne postoji grana");
    }
    pair<bool, pair<double, pair<bool, TipOznake>>> pom = grane.at(pcv).at(dcv);
    return Grana<TipOznake>(pcv, dcv, this, pom.second.first,
                            pom.second.second.first, pom.second.second.second);
  }
  Cvor<TipOznake> dajCvor(int brcv) override {
    if (!postojiCvor(brcv)) {
      throw domain_error("Ne postoji cvor");
    }
    return Cvor<TipOznake>(brcv, this, cvorovi.at(brcv).first,
                           cvorovi.at(brcv).second);
  }
  GranaIterator<TipOznake> dajGranePocetak() override {
    if (brgr == 0) {
      throw domain_error("Nema grana");
    }
    int n = cvorovi.size(), i1 = -1, j1 = -1;
    for (int i = 0; i < n; i++) {
      for (int j = 0; j < n; j++) {
        if (postojiGrana(i, j)) {
          i1 = i;
          j1 = j;
          break;
        }
      }
      if (i1 != -1) {
        break;
      }
    }
    return GranaIterator<TipOznake>(dajGranu(i1, j1));
  }
  GranaIterator<TipOznake> dajGraneKraj() override {
    if (brgr == 0) {
      throw domain_error("Nema grana");
    }
    int n = cvorovi.size(), i1 = -1, j1 = -1;
    for (int i = n - 1; i >= 0; i--) {
      for (int j = n - 1; j >= 0; j--) {
        if (postojiGrana(i, j)) {
          i1 = i;
          j1 = j;
          break;
        }
      }
      if (i1 != -1) {
        break;
      }
    }
    return GranaIterator<TipOznake>(dajGranu(i1, j1));
  }
};

int main() {
  UsmjereniGraf<bool> *g = new MatricaGraf<bool>(6);
  g->dodajGranu(0, 1, 2.5);
  g->dodajGranu(1, 0, 1.2);
  g->dodajGranu(1, 2, 0.1);
  g->dodajGranu(0, 0, 3.14);
  auto kk = g->dajGraneKraj();
  for (GranaIterator<bool> iter = g->dajGranePocetak(); iter != kk; ++iter) {
    cout << "(" << (*iter).dajPolazniCvor().dajRedniBroj() << ","
         << (*iter).dajDolazniCvor().dajRedniBroj() << ") -> "
         << (*iter).dajTezinu() << "; ";  
  }
  delete g;
  return 0;
}
