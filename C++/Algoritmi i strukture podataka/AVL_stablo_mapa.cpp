#include <ctime>
#include <iomanip>
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

template <typename tipk, typename tipv> struct AVLCvor {
  AVLCvor *l, *d, *r; // lijevi, desni, roditelj
  tipk klj;           // kljuc
  tipv vr;            // vrijednost
  int blns;
};

template <typename tipk, typename tipv>
class AVLStabloMapa : public Mapa<tipk, tipv> {
  AVLCvor<tipk, tipv> *kor; // korijen
  int br;

  void obrisiSve(AVLCvor<tipk, tipv> *kor1);
  void kopija(AVLCvor<tipk, tipv> *r, const AVLCvor<tipk, tipv> *c1,
              AVLCvor<tipk, tipv> *&c2);
  const AVLCvor<tipk, tipv> *trazi(const tipk &x,
                                   const AVLCvor<tipk, tipv> *kor1) const;
  void lijevo(AVLCvor<tipk, tipv> *krit, AVLCvor<tipk, tipv> *prvo);
  void desno(AVLCvor<tipk, tipv> *krit, AVLCvor<tipk, tipv> *prvo);
  void lijevoDesno(AVLCvor<tipk, tipv> *krit, AVLCvor<tipk, tipv> *prvo,
                   AVLCvor<tipk, tipv> *drugo);
  void desnoLijevo(AVLCvor<tipk, tipv> *krit, AVLCvor<tipk, tipv> *prvo,
                   AVLCvor<tipk, tipv> *drugo);
  void azurirajBalans(AVLCvor<tipk, tipv> *cv);

public:
  AVLStabloMapa();
  ~AVLStabloMapa();
  AVLStabloMapa(const AVLStabloMapa &bsm);
  AVLStabloMapa(AVLStabloMapa &&bsm);
  AVLStabloMapa &operator=(const AVLStabloMapa &bsm);
  AVLStabloMapa &operator=(AVLStabloMapa &&bsm);
  tipv &operator[](tipk k) override;
  tipv operator[](tipk k) const override;
  int brojElemenata() const override;
  void obrisi() override;
  void obrisi(const tipk &kljuc) override;
};

template <typename tipk, typename tipv>
AVLStabloMapa<tipk, tipv>::AVLStabloMapa() {
  kor = nullptr;
  br = 0;
}

template <typename tipk, typename tipv>
AVLStabloMapa<tipk, tipv>::~AVLStabloMapa() {
  obrisiSve(kor);
  kor = nullptr;
  br = 0;
}

template <typename tipk, typename tipv>
void AVLStabloMapa<tipk, tipv>::kopija(AVLCvor<tipk, tipv> *r,
                                       const AVLCvor<tipk, tipv> *c1,
                                       AVLCvor<tipk, tipv> *&c2) {
  if (c1) {
    c2 = new AVLCvor<tipk, tipv>();
    c2->klj = c1->klj;
    c2->vr = c1->vr;
    c2->r = r;
    c2->blns = c1->blns;
    kopija(c2, c1->l, c2->l);
    kopija(c2, c1->d, c2->d);
  }
}

template <typename tipk, typename tipv>
AVLStabloMapa<tipk, tipv>::AVLStabloMapa(const AVLStabloMapa &bsm) {
  kopija(nullptr, bsm.kor, kor);
  br = bsm.br;
}

template <typename tipk, typename tipv>
AVLStabloMapa<tipk, tipv>::AVLStabloMapa(AVLStabloMapa &&bsm) {
  kor = bsm.kor;
  bsm.kor = nullptr;
  br = bsm.br;
  bsm.br = 0;
}

template <typename tipk, typename tipv>
AVLStabloMapa<tipk, tipv> &
AVLStabloMapa<tipk, tipv>::operator=(const AVLStabloMapa &bsm) {
  if (&bsm != this) {
    obrisiSve(kor);
    kopija(nullptr, bsm.kor, kor);
    br = bsm.br;
  }
  return *this;
}

template <typename tipk, typename tipv>
AVLStabloMapa<tipk, tipv> &
AVLStabloMapa<tipk, tipv>::operator=(AVLStabloMapa &&bsm) {
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
void AVLStabloMapa<tipk, tipv>::lijevo(AVLCvor<tipk, tipv> *krit,
                                       AVLCvor<tipk, tipv> *prvo) {
  if (krit->r != nullptr) {
    prvo->r = krit->r;
    if (krit->r->d == krit) {
      krit->r->d = prvo;
    } else {
      krit->r->l = prvo;
    }
  } else {
    prvo->r = nullptr;
  }
  krit->r = prvo;
  if (prvo->l != nullptr) {
    krit->d = prvo->l;
  } else {
    krit->d = nullptr;
  }
  prvo->l = krit;
}

template <typename tipk, typename tipv>
void AVLStabloMapa<tipk, tipv>::desno(AVLCvor<tipk, tipv> *krit,
                                      AVLCvor<tipk, tipv> *prvo) {
  if (krit->r != nullptr) {
    prvo->r = krit->r;
    if (krit->r->d == krit) {
      krit->r->d = prvo;
    } else {
      krit->r->l = prvo;
    }
  } else {
    prvo->r = nullptr;
  }
  krit->r = prvo;
  if (prvo->d != nullptr) {
    krit->l = prvo->d;
  } else {
    krit->l = nullptr;
  }
  prvo->d = krit;
}

template <typename tipk, typename tipv>
void AVLStabloMapa<tipk, tipv>::lijevoDesno(AVLCvor<tipk, tipv> *krit,
                                            AVLCvor<tipk, tipv> *prvo,
                                            AVLCvor<tipk, tipv> *drugo) {
  if (drugo->d == nullptr && drugo->l == nullptr) {
    prvo->d = nullptr;
  }
  if (drugo->d != nullptr) {
    prvo->d = drugo->d;
    prvo->d->r = prvo;
    drugo->d = nullptr;
  }
  if (drugo->l != nullptr) {
    prvo->d = drugo->l;
    prvo->d->r = prvo;
  }

  drugo->l = prvo;
  drugo->r = krit;
  prvo->r = drugo;
  krit->l = drugo;

  desno(krit, drugo);
}

template <typename tipk, typename tipv>
void AVLStabloMapa<tipk, tipv>::desnoLijevo(AVLCvor<tipk, tipv> *krit,
                                            AVLCvor<tipk, tipv> *prvo,
                                            AVLCvor<tipk, tipv> *drugo) {
  if (drugo->d == nullptr && drugo->l == nullptr) {
    prvo->l = nullptr;
  }
  if (drugo->d != nullptr) {
    prvo->l = drugo->d;
    prvo->l->r = prvo;
  }
  if (drugo->l != nullptr) {
    prvo->l = drugo->l;
    prvo->l->r = prvo;
    drugo->l = nullptr;
  }
  drugo->d = prvo;
  drugo->r = krit;
  prvo->r = drugo;
  krit->d = drugo;

  lijevo(krit, drugo);
}

template <typename tipk, typename tipv>
void AVLStabloMapa<tipk, tipv>::azurirajBalans(AVLCvor<tipk, tipv> *cv) {
  while (cv->r != nullptr) {
    if (cv->r->l == cv) {
      cv->r->blns++;
    } else {
      cv->r->blns--;
    }
    if (cv->r->blns < -1) {
      if (cv->blns == -1) {
        cv->r->blns = 0;
        cv->blns = 0;
        lijevo(cv->r, cv);
        return;
      }
      if (cv->r->l == cv) {
        int dr_blns = cv->d->blns;
        cv->d->blns = 0;
        if (dr_blns == 1) {
          cv->r->blns = 0;
          cv->blns = -1;
        } else if (dr_blns == -1) {
          cv->r->blns = 1;
          cv->blns = 0;
        } else {
          cv->r->blns = 0;
          cv->blns = 0;
        }
        desnoLijevo(cv->r, cv, cv->d);
      } else {
        int dr_blns = cv->l->blns;
        cv->l->blns = 0;
        if (dr_blns == 1) {
          cv->r->blns = 0;
          cv->blns = -1;
        } else if (dr_blns == -1) {
          cv->r->blns = 1;
          cv->blns = 0;
        } else {
          cv->r->blns = 0;
          cv->blns = 0;
        }
        desnoLijevo(cv->r, cv, cv->l);
      }
      return;
    }
    if (cv->r->blns > 1) {
      if (cv->blns == 1) {
        cv->r->blns = 0;
        cv->blns = 0;
        desno(cv->r, cv);
        return;
      }
      if (cv->r->l == cv) {
        int dr_blns = cv->d->blns;
        cv->d->blns = 0;
        if (dr_blns == 1) {
          cv->r->blns = -1;
          cv->blns = 0;
        } else if (dr_blns == -1) {
          cv->r->blns = 0;
          cv->blns = 1;
        } else {
          cv->r->blns = 0;
          cv->blns = 0;
        }
        lijevoDesno(cv->r, cv, cv->d);
      } else {
        int dr_blns = cv->l->blns;
        cv->l->blns = 0;
        if (dr_blns == 1) {
          cv->r->blns = -1;
          cv->blns = 0;
        } else if (dr_blns == -1) {
          cv->r->blns = 0;
          cv->blns = 1;
        } else {
          cv->r->blns = 0;
          cv->blns = 0;
        }
        lijevoDesno(cv->r, cv, cv->l);
      }
      return;
    }
    cv = cv->r;
  }
}

template <typename tipk, typename tipv>
const AVLCvor<tipk, tipv> *
AVLStabloMapa<tipk, tipv>::trazi(const tipk &x,
                                 const AVLCvor<tipk, tipv> *kor1) const {
  if (kor1 == nullptr || x == kor1->klj) {
    return kor1;
  }
  if (x < kor1->klj) {
    return trazi(x, kor1->l);
  }
  return trazi(x, kor1->d);
}

template <typename tipk, typename tipv>
tipv &AVLStabloMapa<tipk, tipv>::operator[](tipk k) {
  AVLCvor<tipk, tipv> **kor1 = &kor, *rod = nullptr;
  while (*kor1 != nullptr) {
    if (k == (*kor1)->klj) {
      return (*kor1)->vr;
    }
    rod = *kor1;
    if (k < (*kor1)->klj) {
      kor1 = &(*kor1)->l;
    } else {
      kor1 = &(*kor1)->d;
    }
  }
  if (*kor1 == nullptr) {
    *kor1 = new AVLCvor<tipk, tipv>();
    (*kor1)->l = nullptr;
    (*kor1)->d = nullptr;
    (*kor1)->r = rod;
    (*kor1)->klj = k;
    (*kor1)->vr = tipv();
    (*kor1)->blns = 0;
    if (rod != nullptr) {
      if (rod->l != nullptr && rod->d != nullptr &&
          (rod->blns == 1 || rod->blns == -1)) {
        rod->blns = 0;
      } else {
        azurirajBalans(*kor1);
      }
    } else {
      kor = *kor1;
    }
    br++;
    return (*kor1)->vr;
  }
  return (*kor1)->vr;
}

template <typename tipk, typename tipv>
tipv AVLStabloMapa<tipk, tipv>::operator[](tipk k) const {
  auto tr = trazi(k, kor);
  if (!tr) {
    return tipv();
  }
  return tr->vr;
}

template <typename tipk, typename tipv>
int AVLStabloMapa<tipk, tipv>::brojElemenata() const {
  return br;
}

template <typename tipk, typename tipv>
void AVLStabloMapa<tipk, tipv>::obrisi() {
  obrisiSve(kor);
  kor = nullptr;
  br = 0;
}

template <typename tipk, typename tipv>
void AVLStabloMapa<tipk, tipv>::obrisi(const tipk &kljuc) {
  AVLCvor<tipk, tipv> *p = kor, *q = nullptr;
  AVLCvor<tipk, tipv> *tmp = nullptr, *pp = nullptr, *rp = nullptr;
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
      if (pp->d != nullptr)
        pp->d->r = pp;
      rp->l = p->l;
      if (rp->l != nullptr)
        rp->l->r = rp;
    }
    rp->d = p->d;
    if (rp->d != nullptr)
      rp->d->r = rp;
  }
  if (!q) {
    kor = rp;
    kor->r = nullptr;
    if (p->l != nullptr) {
      p->l->r = kor;
    }
    if (p->d != nullptr) {
      p->d->r = kor;
    }
    if (kor->l != nullptr) {
      kor->l->r = kor;
    }
    if (kor->d != nullptr) {
      kor->d->r = kor;
    }
  } else if (p == q->l) {
    q->l = rp;
    if (q->l != nullptr) {
      q->l->r = q;
    }
    q->blns--;
  } else {
    q->d = rp;
    if (q->d != nullptr) {
      q->d->r = q;
    }
    q->blns++;
  }
  delete p;
  if (br > 0)
    br--;
}

template <typename tipk, typename tipv>
void AVLStabloMapa<tipk, tipv>::obrisiSve(AVLCvor<tipk, tipv> *kor1) {
  if (kor1 != nullptr) {
    obrisiSve(kor1->l);
    obrisiSve(kor1->d);
    delete kor1;
    kor1 = nullptr;
  }
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

int main() {
    /*Kad se vrati iz funkcija lijevo, desno, lijevoDesno i desnoLijevo u azurirajBalans i onda u operator[], 
    pokazivač na novi element postane nullptr. Ne znam zašto se to dešava. Pokušala sam riješiti problem na 
    nekoliko načina, ali nisam uspjela. Rotiranje se izvrši pravilno, ali pokazivač samo postane nullptr.*/
  AVLStabloMapa<int, int> avlsm;
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
    avlsm[br[i]] = i;
  }

  clock_t vrijeme2 = clock();
  double ukvrijeme = (vrijeme2 - vrijeme1) / (CLOCKS_PER_SEC / 1000);
  cout << "Vrijeme izvrsenja za AVLStabloMapu: " << ukvrijeme / 10000. << " ms."
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

  AVLStabloMapa<int, int> avlsm1;
  BinStabloMapa<int, int> bsm1;

  cout << endl << "50000 elemenata:" << endl << endl;

  vrijeme1 = clock();
  for (int i = 0; i < 50000; i++) {
    avlsm1[br1[i]] = i;
  }
  vrijeme2 = clock();
  ukvrijeme = (vrijeme2 - vrijeme1) / (CLOCKS_PER_SEC / 1000);
  cout << "Vrijeme izvrsenja za AVLStabloMapu: " << ukvrijeme / 50000. << " ms."
       << endl;

  vrijeme1 = clock();
  for (int i = 0; i < 50000; i++) {
    bsm1[br1[i]] = i;
  }
  vrijeme2 = clock();
  ukvrijeme = (vrijeme2 - vrijeme1) / (CLOCKS_PER_SEC / 1000);
  cout << "Vrijeme izvrsenja za BinStabloMapu: " << ukvrijeme / 50000. << " ms."
       << endl;


/*Dodavanje novog elementa je brže u klasi AVLStabloMapa. Razlog tome je što
je AVLStabloMapa balansirano i vrijeme izvršenja je uvijek O(log(n)), dok kod
BinStabloMapa, može se desiti da je unos u vremenu O(n), ako se unose sortirani
elementi.*/


  // Pristup postojećim elementima
  cout << endl
       << endl
       << "Pristup postojećim elementima:\n--------------------------\n"
       << "Mape s 10000 elemenata:" << endl
       << endl;

  vrijeme1 = clock();
  for (int i = 0; i < 10000; i++) {
    avlsm[br[i]];
  }
  vrijeme2 = clock();
  ukvrijeme = (vrijeme2 - vrijeme1) / (CLOCKS_PER_SEC / 1000);
  cout << "Vrijeme izvrsenja za AVLStabloMapu: " << ukvrijeme / 10000. << " ms."
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
    avlsm[br1[i]];
  }
  vrijeme2 = clock();
  ukvrijeme = (vrijeme2 - vrijeme1) / (CLOCKS_PER_SEC / 1000);
  cout << "Vrijeme izvrsenja za AVLStabloMapu: " << ukvrijeme / 50000. << " ms."
       << endl;

  vrijeme1 = clock();
  for (int i = 0; i < 50000; i++) {
    bsm1[br1[i]];
  }
  vrijeme2 = clock();
  ukvrijeme = (vrijeme2 - vrijeme1) / (CLOCKS_PER_SEC / 1000);
  cout << "Vrijeme izvrsenja za BinStabloMapu: " << ukvrijeme / 50000. << " ms."
       << endl;


  /*Za pristup elementima je isti slučaj. AVLStabloMapa opet traži element s
vremenskom kompleksnošću O(log(n)), a BinStabloMapa može tražiti i u O(n), u slučaju da su uneseni sortirani elementi.*/


  return 0;
}
