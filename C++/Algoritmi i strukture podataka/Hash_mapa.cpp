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

template <typename tipk, typename tipv>
class HashMapa : public Mapa<tipk, tipv> {
  pair<tipk, tipv> *niz;
  bool *imaEl; 
  int vel, max;
  unsigned int (*f)(tipk,unsigned int);
  int dajInd(unsigned int ind, bool* imaEl1);

public:
  HashMapa();
  ~HashMapa();
  HashMapa(const HashMapa &hm);
  HashMapa(HashMapa &&hm);
  HashMapa &operator=(const HashMapa &hm);
  HashMapa &operator=(HashMapa &&hm);
  tipv &operator[](tipk k) override;
  tipv operator[](tipk k) const override;
  int brojElemenata() const override;
  void obrisi() override;
  void obrisi(const tipk &kljuc) override;
  void definisiHashFunkciju(unsigned int (*f)(tipk,unsigned int));
};

template <typename tipk, typename tipv>
HashMapa<tipk, tipv>::HashMapa() : vel(0), max(10), f(nullptr) {
  niz = new pair<tipk, tipv>[max]{};
  imaEl = new bool[max]{};
}

template <typename tipk, typename tipv> HashMapa<tipk, tipv>::~HashMapa() {
  if (niz) {
    delete[] niz;
  }
  if(imaEl) {
    delete[] imaEl;
  }
}

template <typename tipk, typename tipv>
HashMapa<tipk, tipv>::HashMapa(const HashMapa &hm) : vel(hm.vel), max(hm.max), f(hm.f) {
  niz = new pair<tipk, tipv>[max]{};
  imaEl = new bool[max]{};
  for (int i = 0; i < max; i++) {
    niz[i].first = hm.niz[i].first;
    niz[i].second = hm.niz[i].second;
    imaEl[i]=hm.imaEl[i];
  }
}

template <typename tipk, typename tipv>
HashMapa<tipk, tipv>::HashMapa(HashMapa &&hm) : vel(hm.vel), max(hm.max), f(hm.f) {
  niz = hm.niz;
  imaEl=hm.imaEl;
  hm.niz = nullptr;
  hm.imaEl=nullptr;
  hm.f=nullptr;
  hm.vel = 0;
  hm.max = 0;
}

template <typename tipk, typename tipv>
HashMapa<tipk, tipv> &HashMapa<tipk, tipv>::operator=(const HashMapa &hm) {
  if (&hm != this) {
    delete[] niz;
    delete[] imaEl;
    max = hm.max;
    niz = new pair<tipk, tipv>[max]{};
    imaEl = new bool[max]{};
    vel = hm.vel;
    f=hm.f;
    for (int i = 0; i < max; i++) {
      niz[i].first = hm.niz[i].first;
      niz[i].second = hm.niz[i].second;
      imaEl[i]=hm.imaEl[i];
    }
  }
  return *this;
}

template <typename tipk, typename tipv>
HashMapa<tipk, tipv> &HashMapa<tipk, tipv>::operator=(HashMapa &&hm) {
  if (&hm != this) {
    delete[] niz;
    delete[] imaEl;
    niz = hm.niz;
    imaEl=hm.imaEl;
    vel = hm.vel;
    max = hm.max;
    f=hm.f;
    hm.niz = nullptr;
    hm.imaEl=nullptr;
    hm.vel = 0;
    hm.max = 0;
    hm.f=nullptr;
  }
  return *this;
}

template <typename tipk, typename tipv>
int HashMapa<tipk, tipv>::dajInd(unsigned int ind, bool* imaEl1) {
    for(int i=ind; i<max; i++) {
        if(!imaEl1[i]) return i;
    }
    for(int i=0; i<ind; i++) {
        if(!imaEl1[i]) return i;
    }
    return ind;
}

template <typename tipk, typename tipv>
tipv &HashMapa<tipk, tipv>::operator[](tipk k) {
  if(f==nullptr) {
      throw domain_error("Hash funkcija nije definisana");
  }
  if (vel == 0) {
    if (max == 0) {
      max = 10;
      niz = new pair<tipk, tipv>[max]{};
      imaEl= new bool[max]{};
    }
    int indd=f(k,max);
    niz[indd] = make_pair(k, tipv());
    imaEl[indd]=true;
    vel++;
    return niz[indd].second;
  }
  unsigned int ind = f(k,max);
  for (int i = ind; i < max; i++) {
    if (niz[i].first == k && imaEl[i]) {
      return niz[i].second;
    }
  }
  for (int i = 0; i < ind; i++) {
    if (niz[i].first == k && imaEl[i]) {
      return niz[i].second;
    }
  }
  if (vel == max) {
    pair<tipk, tipv> *niz1 = new pair<tipk, tipv>[2*max]{};
    bool *imaEl1 = new bool[2*max]{};
    for (int i = 0; i < max; i++) {
      int ind1=dajInd(f(niz[i].first,2*max),imaEl1);
      niz1[ind1].first = niz[i].first;
      imaEl1[ind1]=true;
    }
    max *= 2;
    delete[] niz;
    delete[] imaEl;
    niz = niz1;
    imaEl=imaEl1;
  }
  ind=dajInd(f(k,max),imaEl);
  niz[ind] = make_pair(k, tipv());
  imaEl[ind]=true;
  vel++;
  return niz[ind].second;
}

template <typename tipk, typename tipv>
tipv HashMapa<tipk, tipv>::operator[](tipk k) const {
  if(f==nullptr) {
      throw domain_error("Hash funkcija nije definisana");
  }
  if (vel == 0) {
    return tipv();
  }
  unsigned int ind = f(k,max);
  for (int i = ind; i < max; i++) {
    if (niz[i].first == k && imaEl[i]) {
      return niz[i].second;
    }
  }
  for (int i = 0; i < ind; i++) {
    if (niz[i].first == k && imaEl[i]) {
      return niz[i].second;
    }
  }
  return tipv();
}

template <typename tipk, typename tipv>
int HashMapa<tipk, tipv>::brojElemenata() const {
  return vel;
}

template <typename tipk, typename tipv> void HashMapa<tipk, tipv>::obrisi() {
  delete[] niz;
  delete[] imaEl;
  niz = nullptr;
  imaEl=nullptr;
  vel = 0;
  max = 0;
}

template <typename tipk, typename tipv>
void HashMapa<tipk, tipv>::obrisi(const tipk &kljuc) {
  int ind=f(kljuc,max);
  for (int i = ind; i < max; i++) {
    if (niz[i].first == kljuc && imaEl[i]) {
      imaEl[i]=false;
      vel--;
      return;
    }
  }
  for (int i = 0; i < ind; i++) {
    if (niz[i].first == kljuc && imaEl[i]) {
      imaEl[i]=false;
      vel--;
      return;
    }
  }
}

template <typename tipk, typename tipv>
void HashMapa<tipk, tipv>::definisiHashFunkciju(unsigned int (*f)(tipk,unsigned int)) {
    if(f==nullptr) {
        throw domain_error("Hash funkcija nije definisana");
    }
    this->f=f;
}

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
      return;
    }
  }
}

unsigned int mojhash1(int ulaz, unsigned int max) {
    unsigned int suma=51;
    suma=suma*35+ulaz;
    return suma%max;
}

int main() {
  NizMapa<int, int> nm;
  BinStabloMapa<int, int> bsm;
  HashMapa<int,int> hm;
  hm.definisiHashFunkciju(mojhash1);

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

    vrijeme1 = clock();
  for (int i = 0; i < 10000; i++) {
    hm[br[i]] = i;
  }
  vrijeme2 = clock();
  ukvrijeme = (vrijeme2 - vrijeme1) / (CLOCKS_PER_SEC / 1000);
  cout << "Vrijeme izvrsenja za HashMapu: " << ukvrijeme / 10000. << " ms."
       << endl;

  vector<int> br1;

  for(int i=0; i<50000; i++) {
      br1.push_back(rand());
  }

  NizMapa<int, int> nm1;
  BinStabloMapa<int, int> bsm1;
  HashMapa<int,int> hm1;
  hm1.definisiHashFunkciju(mojhash1);

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

    vrijeme1 = clock();
  for (int i = 0; i < 50000; i++) {
    hm1[br1[i]] = i;
  }
  vrijeme2 = clock();
  ukvrijeme = (vrijeme2 - vrijeme1) / (CLOCKS_PER_SEC / 1000);
  cout << "Vrijeme izvrsenja za HashMapu: " << ukvrijeme / 50000. << " ms."
       << endl;

  /*
  Dodavanje novog elementa je najbrže u klasi BinStabloMapa, pa u NizMapi. Razlog tome je što NizMapa prvo traži element s vremenskom kompleksnošću O(n), 
  a samo dodavanje je O(1), tako da je sveukupno O(n), a u BinStabloMapi se traži čvor s vremenskom kompleksnošću O(log(n)), te se dodaje novi čvor
  za O(1), pa je sveukupno O(log(n)). A HashMapa također dodaje novi element s vremenskom kompleksnošću O(n), ali joj je potrebno malo više vremena od NizMape
  zbog kolizija.
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

    vrijeme1 = clock();
  for (int i = 0; i < 10000; i++) {
    hm[br[i]];
  }
  vrijeme2 = clock();
  ukvrijeme = (vrijeme2 - vrijeme1) / (CLOCKS_PER_SEC / 1000);
  cout << "Vrijeme izvrsenja za HashMapu: " << ukvrijeme / 10000. << " ms."
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

  vrijeme1 = clock();
  for (int i = 0; i < 50000; i++) {
    hm1[br1[i]];
  }
  vrijeme2 = clock();
  ukvrijeme = (vrijeme2 - vrijeme1) / (CLOCKS_PER_SEC / 1000);
  cout << "Vrijeme izvrsenja za HashMapu: " << ukvrijeme / 50000. << " ms."
       << endl;

  /*
  Za pristup elementima je isti slučaj za BinStabloMapu i NizMapu, a za HashMapu je brži pristup elementima nego dodavanje. 
  NizMapa opet traži čvor s vremenskom kompleksnošću O(n), a BinStabloMapa O(log(n)). Da nema kolizija, HashMapa bi imala vremensku
  kompleksnost O(1), s kolizijama ima vremensku kompleksnost O(n), ali je skoro duplo manja od NizMape, jer HashMapa kroz čitav niz prolazi
  samo kada se javljaju kolizije, a NizMapi se prolazak kroz čitav niz može desiti bilo kad.
  */

  // Brisanje elemenata
  cout << endl
       << endl
       << "Brisanje elemenata:\n--------------------------\n"
       << "Mape s 10000 elemenata:" << endl
       << endl;

  vrijeme1 = clock();
  for (int i = 0; i < 10000; i++) {
    nm.obrisi(br[i]);
  }
  vrijeme2 = clock();
  ukvrijeme = (vrijeme2 - vrijeme1) / (CLOCKS_PER_SEC / 1000);
  cout << "Vrijeme izvrsenja za NizMapu: " << ukvrijeme / 10000. << " ms."
       << endl;

  vrijeme1 = clock();
  for (int i = 0; i < 10000; i++) {
    bsm.obrisi(br[i]);
  }
  vrijeme2 = clock();
  ukvrijeme = (vrijeme2 - vrijeme1) / (CLOCKS_PER_SEC / 1000);
  cout << "Vrijeme izvrsenja za BinStabloMapu: " << ukvrijeme / 10000. << " ms."
       << endl;

    vrijeme1 = clock();
  for (int i = 0; i < 10000; i++) {
    hm.obrisi(br[i]);
  }
  vrijeme2 = clock();
  ukvrijeme = (vrijeme2 - vrijeme1) / (CLOCKS_PER_SEC / 1000);
  cout << "Vrijeme izvrsenja za HashMapu: " << ukvrijeme / 10000. << " ms."
       << endl;

  cout << endl << "Mape s 50000 elemenata:" << endl << endl;

  vrijeme1 = clock();
  for (int i = 0; i < 50000; i++) {
    nm1.obrisi(br1[i]);
  }
  vrijeme2 = clock();
  ukvrijeme = (vrijeme2 - vrijeme1) / (CLOCKS_PER_SEC / 1000);
  cout << "Vrijeme izvrsenja za NizMapu: " << ukvrijeme / 50000. << " ms."
       << endl;

  vrijeme1 = clock();
  for (int i = 0; i < 50000; i++) {
    bsm1.obrisi(br1[i]);
  }
  vrijeme2 = clock();
  ukvrijeme = (vrijeme2 - vrijeme1) / (CLOCKS_PER_SEC / 1000);
  cout << "Vrijeme izvrsenja za BinStabloMapu: " << ukvrijeme / 50000. << " ms."
       << endl;

  vrijeme1 = clock();
  for (int i = 0; i < 50000; i++) {
    hm1.obrisi(br1[i]);
  }
  vrijeme2 = clock();
  ukvrijeme = (vrijeme2 - vrijeme1) / (CLOCKS_PER_SEC / 1000);
  cout << "Vrijeme izvrsenja za HashMapu: " << ukvrijeme / 50000. << " ms."
       << endl;

  /*
  Brisanje elemenata kod BinStabloMape je iste kompleksnosti ko pristup elementu, jer se kod obje metode traže elementi u O(log(n)), dodatno kod brisanja
  se briše čvor u O(1) vremenu. Znači i ovdje je BinStabloMapa najefikasnija. Sljedeća je HashMapa, koja kao i kod pristupa elementima traži elemente u O(n) vremenu.
  Najgora je NizMapa jer premješta elemente, ali joj je kompleksnost O(n), ali znatno veća od HashMape.
  */
  return 0;
}
