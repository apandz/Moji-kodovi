#include <ctime>
#include <iostream>
#include <vector>

using namespace std;

template <typename tipk, typename tipv> class Mapa {
public:
  Mapa() {}
  virtual ~Mapa() {}
  virtual tipv &operator[](tipk k) = 0;
  virtual tipv operator[](tipk k) const = 0;
  virtual int brojElemenata() const = 0;
  virtual void obrisi() = 0;
  virtual void obrisi(const tipk &kljuc) = 0;
};

template <typename tipk, typename tipv> struct Cvor {
  Cvor *l, *d, *r; // lijevi, desni, roditelj
  tipk klj;        // kljuc
  tipv vr;         // vrijednost
};

template <typename tipk, typename tipv>
class BinStabloMapa : public Mapa<tipk, tipv> {
  Cvor<tipk, tipv> *kor; // korijen
  int br;

  void obrisiSve(Cvor<tipk, tipv> *kor1);
  void kopija(Cvor<tipk, tipv> *r, const Cvor<tipk, tipv> *c1,
              Cvor<tipk, tipv> *&c2);
  Cvor<tipk, tipv> *&trazi(const tipk &x, Cvor<tipk, tipv> *&kor1,
                           Cvor<tipk, tipv> *rod);
  const Cvor<tipk, tipv> *trazi(const tipk &x,
                                const Cvor<tipk, tipv> *kor1) const;

public:
  BinStabloMapa();
  ~BinStabloMapa();
  BinStabloMapa(const BinStabloMapa &bsm);
  BinStabloMapa(BinStabloMapa &&bsm);
  BinStabloMapa &operator=(const BinStabloMapa &bsm);
  BinStabloMapa &operator=(BinStabloMapa &&bsm);
  tipv &operator[](tipk k) override;
  tipv operator[](tipk k) const override;
  int brojElemenata() const override;
  void obrisi() override;
  void obrisi(const tipk &kljuc) override;
};

template <typename tipk, typename tipv>
BinStabloMapa<tipk, tipv>::BinStabloMapa() {
  kor = nullptr;
  br = 0;
}

template <typename tipk, typename tipv>
BinStabloMapa<tipk, tipv>::~BinStabloMapa() {
  obrisiSve(kor);
  kor = nullptr;
  br = 0;
}

template <typename tipk, typename tipv>
void BinStabloMapa<tipk, tipv>::kopija(Cvor<tipk, tipv> *r,
                                       const Cvor<tipk, tipv> *c1,
                                       Cvor<tipk, tipv> *&c2) {
  if (c1) {
    c2 = new Cvor<tipk, tipv>();
    c2->klj = c1->klj;
    c2->vr = c1->vr;
    c2->r = r;
    kopija(c2, c1->l, c2->l);
    kopija(c2, c1->d, c2->d);
  }
}

template <typename tipk, typename tipv>
BinStabloMapa<tipk, tipv>::BinStabloMapa(const BinStabloMapa &bsm) {
  kopija(nullptr, bsm.kor, kor);
  br = bsm.br;
}

template <typename tipk, typename tipv>
BinStabloMapa<tipk, tipv>::BinStabloMapa(BinStabloMapa &&bsm) {
  kor = bsm.kor;
  bsm.kor = nullptr;
  br = bsm.br;
  bsm.br = 0;
}

template <typename tipk, typename tipv>
BinStabloMapa<tipk, tipv> &
BinStabloMapa<tipk, tipv>::operator=(const BinStabloMapa &bsm) {
  if (&bsm != this) {
    obrisiSve(kor);
    kopija(nullptr, bsm.kor, kor);
    br = bsm.br;
  }
  return *this;
}

template <typename tipk, typename tipv>
BinStabloMapa<tipk, tipv> &
BinStabloMapa<tipk, tipv>::operator=(BinStabloMapa &&bsm) {
  if (&bsm != this) {
    obrisiSve(kor);
    kor = bsm.kor;
    bsm.kor = nullptr;
    br = bsm.br;
    bsm.br = 0;
  }
  return *this;
}

template <typename tipk, typename tipv>
Cvor<tipk, tipv> *&BinStabloMapa<tipk, tipv>::trazi(const tipk &x,
                                                    Cvor<tipk, tipv> *&kor1,
                                                    Cvor<tipk, tipv> *rod) {
  if (kor1 == nullptr) {
    kor1 = new Cvor<tipk, tipv>();
    kor1->l = nullptr;
    kor1->d = nullptr;
    kor1->r = rod;
    kor1->klj = x;
    kor1->vr = tipv();
    br++;
    return kor1;
  }
  if (x == kor1->klj) {
    return kor1;
  }
  if (x < kor1->klj) {
    return trazi(x, kor1->l, kor1);
  }
  return trazi(x, kor1->d, kor1);
}

template <typename tipk, typename tipv>
const Cvor<tipk, tipv> *
BinStabloMapa<tipk, tipv>::trazi(const tipk &x,
                                 const Cvor<tipk, tipv> *kor1) const {
  if (kor1 == nullptr || x == kor1->klj) {
    return kor1;
  }
  if (x < kor1->klj) {
    return trazi(x, kor1->l);
  }
  return trazi(x, kor1->d);
}

template <typename tipk, typename tipv>
tipv &BinStabloMapa<tipk, tipv>::operator[](tipk k) {
  return trazi(k, kor, nullptr)->vr;
}

template <typename tipk, typename tipv>
tipv BinStabloMapa<tipk, tipv>::operator[](tipk k) const {
  auto tr = trazi(k, kor);
  if (!tr) {
    return tipv();
  }
  return tr->vr;
}

template <typename tipk, typename tipv>
int BinStabloMapa<tipk, tipv>::brojElemenata() const {
  return br;
}

template <typename tipk, typename tipv>
void BinStabloMapa<tipk, tipv>::obrisi() {
  obrisiSve(kor);
  kor = nullptr;
  br = 0;
}

template <typename tipk, typename tipv>
void BinStabloMapa<tipk, tipv>::obrisi(const tipk &kljuc) {
  Cvor<tipk, tipv> *p = kor, *q = nullptr;
  Cvor<tipk, tipv> *tmp = nullptr, *pp = nullptr, *rp = nullptr;
  while (p != 0 && kljuc != p->klj) {
    q = p;
    if (kljuc < p->klj) {
      p = p->l;
    } else {
      p = p->d;
    }
  }
  if (!p) {
    return;
    // throw domain_error("Čvor nije pronađen");
  }
  if (!p->l) {
    rp = p->d;
  } else if (!p->d) {
    rp = p->l;
  } else {
    pp = p;
    rp = p->l;
    tmp = rp->d;
    while (tmp) {
      pp = rp;
      rp = tmp;
      tmp = rp->d;
    }
    if (pp != p) {
      pp->d = rp->l;
      rp->l = p->l;
    }
    rp->d = p->d;
  }
  if (!q) {
    kor = rp;
  } else if (p == q->l) {
    q->l = rp;
  } else {
    q->d = rp;
  }
  delete p;
  if (br > 0)
    br--;
}

template <typename tipk, typename tipv>
void BinStabloMapa<tipk, tipv>::obrisiSve(Cvor<tipk, tipv> *kor1) {
  if (kor1 != nullptr) {
    obrisiSve(kor1->l);
    obrisiSve(kor1->d);
    delete kor1;
    kor1 = nullptr;
  }
}

template <typename tipk, typename tipv>
class NizMapa : public Mapa<tipk, tipv> {
  pair<tipk, tipv> *niz;
  int vel, max;

public:
  NizMapa();
  ~NizMapa();
  NizMapa(const NizMapa &nm);
  NizMapa(NizMapa &&nm);
  NizMapa &operator=(const NizMapa &nm);
  NizMapa &operator=(NizMapa &&nm);
  tipv &operator[](tipk k) override;
  tipv operator[](tipk k) const override;
  int brojElemenata() const override;
  void obrisi() override;
  void obrisi(const tipk &kljuc) override;
};

template <typename tipk, typename tipv>
NizMapa<tipk, tipv>::NizMapa() : vel(0), max(100) {
  niz = new pair<tipk, tipv>[max];
}

template <typename tipk, typename tipv> NizMapa<tipk, tipv>::~NizMapa() {
  if (niz) {
    delete[] niz;
  }
}

template <typename tipk, typename tipv>
NizMapa<tipk, tipv>::NizMapa(const NizMapa &nm) : vel(nm.vel), max(nm.max) {
  niz = new pair<tipk, tipv>[max];
  for (int i = 0; i < vel; i++) {
    niz[i] = nm.niz[i];
  }
}

template <typename tipk, typename tipv>
NizMapa<tipk, tipv>::NizMapa(NizMapa &&nm) : vel(nm.vel), max(nm.max) {
  niz = nm.niz;
  nm.niz = nullptr;
  nm.vel = 0;
  nm.max = 0;
}

template <typename tipk, typename tipv>
NizMapa<tipk, tipv> &NizMapa<tipk, tipv>::operator=(const NizMapa &nm) {
  if (&nm != this) {
    delete[] niz;
    max = nm.max;
    niz = new pair<tipk, tipv>[max];
    vel = nm.vel;
    for (int i = 0; i < vel; i++) {
      niz[i] = nm.niz[i];
    }
  }
  return *this;
}

template <typename tipk, typename tipv>
NizMapa<tipk, tipv> &NizMapa<tipk, tipv>::operator=(NizMapa &&nm) {
  if (&nm != this) {
    delete[] niz;
    niz = nm.niz;
    vel = nm.vel;
    max = nm.max;
    nm.niz = nullptr;
    nm.vel = 0;
    nm.max = 0;
  }
  return *this;
}

template <typename tipk, typename tipv>
tipv &NizMapa<tipk, tipv>::operator[](tipk k) {
  if (vel == 0) {
    if (max == 0) {
      max = 100;
      niz = new pair<tipk, tipv>[max];
    }
    niz[0] = make_pair(k, tipv());
    vel++;
    return niz[0].second;
  }
  for (int i = 0; i < vel; i++) {
    if (niz[i].first == k) {
      return niz[i].second;
    }
  }
  if (vel == max) {
    max *= 2;
    pair<tipk, tipv> *niz1 = new pair<tipk, tipv>[max];
    for (int i = 0; i < vel; i++) {
      niz1[i] = niz[i];
    }
    delete[] niz;
    niz = niz1;
  }
  niz[vel] = make_pair(k, tipv());
  vel++;
  return niz[vel - 1].second;
}

template <typename tipk, typename tipv>
tipv NizMapa<tipk, tipv>::operator[](tipk k) const {
  for (int i = 0; i < vel; i++) {
    if (niz[i].first == k) {
      return niz[i].second;
    }
  }
  return tipv();
}

template <typename tipk, typename tipv>
int NizMapa<tipk, tipv>::brojElemenata() const {
  return vel;
}

template <typename tipk, typename tipv> void NizMapa<tipk, tipv>::obrisi() {
  delete[] niz;
  niz = nullptr;
  vel = 0;
  max = 0;
}

template <typename tipk, typename tipv>
void NizMapa<tipk, tipv>::obrisi(const tipk &kljuc) {
  for (int i = 0; i < vel; i++) {
    if (niz[i].first == kljuc) {
      for (int j = i; j < vel - 1; j++) {
        niz[j] = niz[j + 1];
      }
      vel--;
    }
  }
}

int main() {
  NizMapa<int, int> nm;
  BinStabloMapa<int, int> bsm;

  cout << "Dodavanje novih elemenata:\n--------------------------" << endl
       << "10000 elemenata:" << endl
       << endl;

  vector<int> br;

  for(int i=0; i<10000; i++) {
      br.push_back(rand());
  }

  clock_t vrijeme1 = clock();
  for (int i = 0; i < 10000; i++) {
    nm[br[i]] = i;
  }

  clock_t vrijeme2 = clock();
  double ukvrijeme = (vrijeme2 - vrijeme1) / (CLOCKS_PER_SEC / 1000);
  cout << "Vrijeme izvrsenja za NizMapu: " << ukvrijeme / 10000. << " ms."
       << endl;

  vrijeme1 = clock();
  for (int i = 0; i < 10000; i++) {
    bsm[br[i]] = i;
  }
  vrijeme2 = clock();
  ukvrijeme = (vrijeme2 - vrijeme1) / (CLOCKS_PER_SEC / 1000);
  cout << "Vrijeme izvrsenja za BinStabloMapu: " << ukvrijeme / 10000. << " ms."
       << endl;

  vector<int> br1;

  for(int i=0; i<50000; i++) {
      br1.push_back(rand());
  }

  NizMapa<int, int> nm1;
  BinStabloMapa<int, int> bsm1;

  cout << endl << "50000 elemenata:" << endl << endl;

  vrijeme1 = clock();
  for (int i = 0; i < 50000; i++) {
    nm1[br1[i]] = i;
  }
  vrijeme2 = clock();
  ukvrijeme = (vrijeme2 - vrijeme1) / (CLOCKS_PER_SEC / 1000);
  cout << "Vrijeme izvrsenja za NizMapu: " << ukvrijeme / 50000. << " ms."
       << endl;

  vrijeme1 = clock();
  for (int i = 0; i < 50000; i++) {
    bsm1[br1[i]] = i;
  }
  vrijeme2 = clock();
  ukvrijeme = (vrijeme2 - vrijeme1) / (CLOCKS_PER_SEC / 1000);
  cout << "Vrijeme izvrsenja za BinStabloMapu: " << ukvrijeme / 50000. << " ms."
       << endl;

  /*
  Dodavanje novog elementa je brže u klasi BinStabloMapa. Razlog tome je što NizMapa prvo traži element s vremenskom kompleksnošću O(n), 
  a samo dodavanje je O(1), tako da je sveukupno O(n), a u BinStabloMapi se traži čvor s vremenskom kompleksnošću O(log(n)), te se dodaje novi čvor
  za O(1), pa je sveukupno O(log(n)).
  */

  // Pristup postojećim elementima
  cout << endl
       << endl
       << "Pristup postojećim elementima:\n--------------------------\n"
       << "Mape s 10000 elemenata:" << endl
       << endl;

  vrijeme1 = clock();
  for (int i = 0; i < 10000; i++) {
    nm[br[i]];
  }
  vrijeme2 = clock();
  ukvrijeme = (vrijeme2 - vrijeme1) / (CLOCKS_PER_SEC / 1000);
  cout << "Vrijeme izvrsenja za NizMapu: " << ukvrijeme / 10000. << " ms."
       << endl;

  vrijeme1 = clock();
  for (int i = 0; i < 10000; i++) {
    bsm[br[i]];
  }
  vrijeme2 = clock();
  ukvrijeme = (vrijeme2 - vrijeme1) / (CLOCKS_PER_SEC / 1000);
  cout << "Vrijeme izvrsenja za BinStabloMapu: " << ukvrijeme / 10000. << " ms."
       << endl;

  cout << endl << "Mape s 50000 elemenata:" << endl << endl;

  vrijeme1 = clock();
  for (int i = 0; i < 50000; i++) {
    nm1[br1[i]];
  }
  vrijeme2 = clock();
  ukvrijeme = (vrijeme2 - vrijeme1) / (CLOCKS_PER_SEC / 1000);
  cout << "Vrijeme izvrsenja za NizMapu: " << ukvrijeme / 50000. << " ms."
       << endl;

  vrijeme1 = clock();
  for (int i = 0; i < 50000; i++) {
    bsm1[br1[i]];
  }
  vrijeme2 = clock();
  ukvrijeme = (vrijeme2 - vrijeme1) / (CLOCKS_PER_SEC / 1000);
  cout << "Vrijeme izvrsenja za BinStabloMapu: " << ukvrijeme / 50000. << " ms."
       << endl;

  /*
  Za pristup elementima je isti slučaj. NizMapa opet traži element s vremenskom kompleksnošću O(n), 
  a BinStabloMapa O(log(n)).
  */

  return 0;
}
