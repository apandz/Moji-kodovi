#include <iostream>

template <typename Tip> class Lista {
public:
  Lista() {}
  virtual ~Lista() {}
  virtual int brojElemenata() const = 0;
  virtual Tip &trenutni() = 0;
  virtual const Tip &trenutni() const = 0;
  virtual bool prethodni() = 0;
  virtual bool sljedeci() = 0;
  virtual void pocetak() = 0;
  virtual void kraj() = 0;
  virtual void obrisi() = 0;
  virtual void dodajIspred(const Tip &el) = 0;
  virtual void dodajIza(const Tip &el) = 0;
  virtual Tip &operator[](int i) = 0;
  virtual const Tip &operator[](int i) const = 0;
};

template <typename Tip> class NizLista : public Lista<Tip> {
  Tip** niz;
  int brojElMax,brojEl, tr;

public:
  NizLista() : niz{},brojElMax{10},brojEl{}, tr{} {niz=new Tip*[brojElMax]{};}
  NizLista(const NizLista& nl) {
      niz = new Tip*[nl.brojElMax];
      brojElMax=nl.brojElMax; brojEl=nl.brojEl; tr=nl.tr;
      for(int i=0; i<brojEl; i++) niz[i]=new Tip(*(nl.niz[i]));
  } 
  NizLista(NizLista&& nl) {
      niz=nl.niz; brojElMax=nl.brojElMax; brojEl=nl.brojEl; tr=nl.tr;
      nl.niz=nullptr; nl.brojElMax=0; nl.brojEl=0; nl.tr=0;
  }
  ~NizLista() {for(int i=0; i<brojEl; i++) if(niz[i])delete niz[i]; delete[] niz;}
  NizLista& operator=(const NizLista& nl) {
      for(int i=0; i<brojEl; i++) if(niz[i])delete niz[i]; delete[] niz;
      niz = new Tip*[nl.brojElMax];
      brojElMax=nl.brojElMax; brojEl=nl.brojEl; tr=nl.tr;
      for(int i=0; i<brojEl; i++) niz[i]=new Tip(*(nl.niz[i]));
      return *this;
  } 
  NizLista& operator=(NizLista&& nl) {
      for(int i=0; i<brojEl; i++) if(niz[i])delete niz[i]; delete[] niz;
      niz=nl.niz; brojElMax=nl.brojElMax; brojEl=nl.brojEl; tr=nl.tr;
      nl.niz=nullptr; nl.brojElMax=0; nl.brojEl=0; nl.tr=0;
      return *this;
  }
  int brojElemenata() const override { return brojEl; }
  Tip &trenutni() override {
    if (!brojEl)
      throw std::range_error("Lista je prazna!");
    return *(niz[tr]);
  }
  const Tip &trenutni() const override {
    if (!brojEl)
      throw std::range_error("Lista je prazna!");
    return *(niz[tr]);
  }
  bool prethodni() override {
    if (!brojEl)
      throw std::range_error("Lista je prazna!");
    else {
      if (tr == 0)
        return false;
      else
        tr--;
    }
    return true;
  }
  bool sljedeci() override {
    if (!brojEl)
      throw std::range_error("Lista je prazna!");
    else {
      if (tr == brojEl - 1)
        return false;
      else
        tr++;
    }
    return true;
  }
  void pocetak() override {
    if (!brojEl)
      throw std::range_error("Lista je prazna!");
    else
      tr = 0;
  }
  void kraj() override {
    if (!brojEl)
      throw std::range_error("Lista je prazna!");
    else
      tr = brojEl - 1;
  }
  void obrisi() override {
    if (!brojEl)
      throw std::range_error("Lista je prazna!");
    delete niz[tr]; niz[tr]=nullptr;
    for (int i = tr; i < brojEl - 1; i++) {
        niz[i] = niz[i + 1];
    }
    if(brojEl==1) tr=0;
    else if(tr==brojEl-1) tr--;
      brojEl--;
  }
  void dodajIspred(const Tip &el) override {
    if (!brojEl) {
      niz[0] = new Tip(el);
      tr = 0;
    } else if (brojEl == brojElMax) {
      brojElMax *= 2;
      Tip **novi = new Tip *[brojElMax] {};
      for (int i = 0; i < brojEl + 1; i++) {
        if (i < tr)
          novi[i] = niz[i];
        if (i > tr)
          novi[i] = niz[i - 1];
      }
      novi[tr] = new Tip(el);
      delete[] niz;
      niz = novi;
      novi = nullptr;
      tr++;
    } else {
      for (int i = brojEl; i > tr; i--) {
        niz[i] = niz[i - 1];
      }
      niz[tr] = new Tip(el);
      tr++;
    }
    brojEl++;
  }
  void dodajIza(const Tip &el) override {
    if (!brojEl) {
      niz[0] = new Tip(el);
      tr = 0;
    } else if(brojEl==brojElMax){
        brojElMax*=2;
        Tip** novi=new Tip*[brojElMax]{};
        for(int i=0; i<brojEl+1; i++) {
            if(i<=tr) novi[i]=niz[i];
            if(i>tr+1) novi[i]=niz[i-1];
        }
        novi[tr+1] = new Tip(el);
        delete[] niz;
        niz=novi;
        novi=nullptr; 
    }else {
      for (int i = brojEl; i > tr + 1; i--) {
        niz[i] = niz[i - 1];
      }
      niz[tr + 1] = new Tip(el);
    }
    brojEl++;
  }
  Tip &operator[](int i) override {
    if (i < 0 || i >= brojEl)
      throw std::range_error("Indeks negativan ili veći od veličine niza!");
    return *niz[i];
  }
  const Tip &operator[](int i) const override {
    if (i < 0 || i >= brojEl)
      throw std::range_error("Indeks negativan ili veći od veličine niza!");
    return *niz[i];
  }
};

/*Testovi NizLista*/
bool test_NL_brojElemenata() {
  NizLista<int> nl;
  for (int i = 0; i < 5; i++)
    nl.dodajIza(i);
  if (nl.brojElemenata() == 5)
    return true;
  return false;
}

bool test_NL_trenutni() {
  NizLista<int> nl;
  for (int i = 4; i < 7; i++) {
    nl.dodajIza(i);
  }
  for (int i = 5; i < 8; i++) {
    nl.dodajIspred(i);
  }
  nl.trenutni() = nl.trenutni() + 6;
  if (nl.trenutni() == 10)
    return true;
  return false;
}

bool test_NL_const_trenutni() {
  NizLista<int> nl;
  for (int i = 4; i < 7; i++) {
    nl.dodajIza(i);
  }
  for (int i = 5; i < 8; i++) {
    nl.dodajIspred(i);
  }
  const NizLista<int> nl1 = nl;
  if (nl.trenutni() == 4)
    return true;
  return false;
}

void test_NL_trenutni_izuzetak() {
  NizLista<int> nl;
  try {
    auto x=nl.trenutni();
  } catch (...) {
    throw;
  }
}

bool test_NL_prethodni() {
  NizLista<int> nl;
  for (int i = 4; i < 7; i++) {
    nl.dodajIza(i);
  }
  for (int i = 0; i < 4; i++) {
    nl.dodajIspred(i);
    nl.prethodni();
  }
  if (nl.trenutni() == 3)
    return true;
  return false;
}

void test_NL_prethodni_izuzetak() {
  NizLista<int> nl;
  try {
    nl.prethodni();
  } catch (...) {
    throw;
  }
}

bool test_NL_sljedeci() {
  NizLista<int> nl;
  for (int i = 4; i < 7; i++) {
    nl.dodajIza(i);
    nl.sljedeci();
  }
  for (int i = 0; i < 4; i++) {
    nl.dodajIspred(i);
  }
  if (nl.trenutni() == 6)
    return true;
  return false;
}

void test_NL_sljedeci_izuzetak() {
  NizLista<int> nl;
  try {
    nl.sljedeci();
  } catch (...) {
    throw;
  }
}

bool test_NL_pocetak() {
  NizLista<int> nl;
  for (int i = 4; i < 7; i++) {
    nl.dodajIza(i);
    nl.sljedeci();
  }
  for (int i = 0; i < 4; i++) {
    nl.dodajIspred(i);
    nl.prethodni();
  }
  nl.pocetak();
  if (nl.trenutni() == 4)
    return true;
  return false;
}

void test_NL_pocetak_izuzetak() {
  NizLista<int> nl;
  try {
    nl.pocetak();
  } catch (...) {
    throw;
  }
}

bool test_NL_kraj() {
  NizLista<int> nl;
  for (int i = 4; i < 7; i++) {
    nl.dodajIza(i);
    nl.sljedeci();
  }
  for (int i = 0; i < 4; i++) {
    nl.dodajIspred(i);
    nl.prethodni();
  }
  nl.kraj();
  if (nl.trenutni() == 6)
    return true;
  return false;
}

void test_NL_kraj_izuzetak() {
  NizLista<int> nl;
  try {
    nl.kraj();
  } catch (...) {
    throw;
  }
}

bool test_NL_obrisi() {
  NizLista<int> nl;
  for (int i = 0; i < 7; i++) {
    nl.dodajIza(i);
    nl.sljedeci();
  }
  for (int i = 0; i < 3; i++)
    nl.prethodni();
  nl.obrisi();
  if (nl.trenutni() == 4)
    return true;
  return false;
}

void test_NL_obrisi_izuzetak() {
  NizLista<int> nl;
  try {
    nl.obrisi();
  } catch (...) {
    throw;
  }
}

bool test_NL_dodajIspred() {
  NizLista<std::string> nl;
  nl.dodajIspred("Hi");
  nl.dodajIspred("how");
  nl.prethodni();
  nl.dodajIspred("are");
  nl.prethodni();
  nl.dodajIspred("you");
  nl.sljedeci();
  if (nl.trenutni() == "how")
    return true;
  return false;
}

bool test_NL_dodajIza() {
  NizLista<int> nl;
  for (int i = 6; i < 12; i++) {
    nl.dodajIza(i);
    nl.sljedeci();
  }
  if (nl.trenutni() == 11)
    return true;
  return false;
}

bool test_NL_operatorUglasteZ() {
  NizLista<int> nl;
  for (int i = 0; i < 7; i++) {
    nl.dodajIza(0);
  }
  int suma = 0;
  for (int i = 0; i < 7; i++) {
    nl[i] = i + 1;
    suma += nl[i];
  }
  if (suma == 28)
    return true;
  return false;
}

void test_NL_operatorUglasteZ_izuzetak() {
  NizLista<int> nl;
  nl.dodajIspred(5);
  try {
    auto x=nl[1];
  } catch (...) {
    throw;
  }
}

template <typename Tip> struct Cvor {
  Tip element;
  Cvor *veza = nullptr;
};

template <typename Tip> class JednostrukaLista : public Lista<Tip> {
  Cvor<Tip> *prvi, *tr;

public:
  JednostrukaLista() : prvi{}, tr{} {}
  JednostrukaLista(const JednostrukaLista &jl) {
    prvi = new Cvor<Tip>;
    prvi->element=jl.prvi->element;
    prvi->veza= nullptr;
    tr = prvi;
    Cvor<Tip> *pok1 = prvi;
    for (Cvor<Tip> *pok = jl.prvi->veza; pok != nullptr; pok = pok->veza) {
      pok1->veza = new Cvor<Tip>;
      pok1->veza->element = pok->element;
      pok1->veza->veza = nullptr;
      pok1 = pok1->veza;
      if (pok == jl.tr)
        tr = pok1;
    }
  }
  JednostrukaLista(JednostrukaLista &&jl) {
    prvi = jl.prvi;
    tr = jl.tr;
    jl.prvi = nullptr;
    jl.tr = nullptr;
  }
  ~JednostrukaLista() {
    if (prvi) {
      if (prvi->veza) {
        tr = prvi->veza;
        delete prvi;
        prvi = nullptr;
        while (tr->veza) {
          prvi = tr->veza;
          delete tr;
          tr = prvi;
        }
        if (tr)
          delete tr;
      } else
        delete prvi;
      tr = nullptr;
      prvi = nullptr;
    }
  }
  JednostrukaLista &operator=(const JednostrukaLista &jl) {
    if (prvi) {
      if (prvi->veza) {
        tr = prvi->veza;
        delete prvi;
        prvi = nullptr;
        while (tr->veza) {
          prvi = tr->veza;
          delete tr;
          tr = prvi;
        }
        if (tr)
          delete tr;
      } else
        delete prvi;
      tr = nullptr;
      prvi = nullptr;
    }
    prvi = new Cvor<Tip>;
    prvi->element=jl.prvi->element;
    prvi->veza= nullptr;
    tr = prvi;
    Cvor<Tip> *pok1 = prvi;
    for (Cvor<Tip> *pok = jl.prvi->veza; pok != nullptr; pok = pok->veza) {
      pok1->veza = new Cvor<Tip>;
      pok1->veza->element = pok->element;
      pok1->veza->veza = nullptr;
      pok1 = pok1->veza;
      if (pok == jl.tr)
        tr = pok1;
    }
    return *this;
  }
  JednostrukaLista &operator=(JednostrukaLista &&jl) {
    if (prvi) {
      if (prvi->veza) {
        tr = prvi->veza;
        delete prvi;
        prvi = nullptr;
        while (tr->veza) {
          prvi = tr->veza;
          delete tr;
          tr = prvi;
        }
        if (tr)
          delete tr;
      } else
        delete prvi;
      tr = nullptr;
      prvi = nullptr;
    }
    prvi = jl.prvi;
    tr = jl.tr;
    jl.prvi = nullptr;
    jl.tr = nullptr;
    return *this;
  }
  int brojElemenata() const override {
    int brojEl = 0;
    for (Cvor<Tip> *pok = prvi; pok != nullptr; pok = pok->veza)
      brojEl++;
    return brojEl;
  }
  Tip &trenutni() override {
    if (!prvi)
      throw std::range_error("Lista je prazna!");
    return tr->element;
  }
  const Tip &trenutni() const override {
    if (!prvi)
      throw std::range_error("Lista je prazna!");
    return tr->element;
  }
  bool prethodni() override {
    if (!prvi)
      throw std::range_error("Lista je prazna!");
    else {
      if (tr == prvi)
        return false;
      else if (tr == prvi->veza)
        tr = prvi;
      else {
        for (Cvor<Tip> *pok = prvi->veza; pok != nullptr; pok = pok->veza) {
          if (pok->veza == tr) {
            tr = pok;
            break;
          }
        }
      }
    }
    return true;
  }
  bool sljedeci() override {
    if (!prvi)
      throw std::range_error("Lista je prazna!");
    else {
      if (!tr->veza)
        return false;
      else
        tr = tr->veza;
    }
    return true;
  }
  void pocetak() override {
    if (!prvi)
      throw std::range_error("Lista je prazna!");
    else
      tr = prvi;
  }
  void kraj() override {
    if (!prvi)
      throw std::range_error("Lista je prazna!");
    else {
      for (Cvor<Tip> *pok = prvi; pok != nullptr; pok = pok->veza) {
        if (!pok->veza) {
          tr = pok;
          break;
        }
      }
    }
  }
  void obrisi() override {
    if (!prvi)
      throw std::range_error("Lista je prazna!");
    else if (!prvi->veza) {
      delete prvi;
      prvi = nullptr;
      tr = nullptr;
    } else if (prvi->veza && tr == prvi) {
      prvi = prvi->veza;
      delete tr;
      tr = prvi;
    } else {
      for (Cvor<Tip> *pok = prvi; pok != nullptr; pok = pok->veza) {
        if (pok->veza == tr) {
          pok->veza = tr->veza;
          delete tr;
          if (pok->veza)
            tr = pok->veza;
          else
            tr = pok;
          break;
        }
      }
    }
  }
  void dodajIspred(const Tip &el) override {
    if (!prvi) {
      prvi = new Cvor<Tip>;
      prvi->element=el;
      prvi->veza=nullptr;
      tr = prvi;
    } else if (prvi && tr == prvi) {
      prvi = new Cvor<Tip>;
      prvi->element=el;
      prvi->veza=tr;
    } else {
      for (Cvor<Tip> *pok = prvi; pok != nullptr; pok = pok->veza) {
        if (pok->veza == tr) {
          pok->veza = new Cvor<Tip>;
          pok->veza->element=el;
          pok->veza->veza=tr;
          break;
        }
      }
    }
  }
  void dodajIza(const Tip &el) override {
    if (!prvi) {
      prvi = new Cvor<Tip>;
      prvi->element=el;
      prvi->veza=nullptr;
      tr = prvi;
    } else {
      for (Cvor<Tip> *pok = prvi; pok != nullptr; pok = pok->veza) {
        if (pok == tr) {
          Cvor<Tip>* pom=pok->veza;
          pok->veza = new Cvor<Tip>;
          pok->veza->element = el;
          pok->veza->veza = pom;
          break;
        }
      }
    }
  }
  Tip &operator[](int i) override {
    if (i < 0 || i >= brojElemenata())
      throw std::range_error("Indeks negativan ili veći od veličine niza!");
    if (i != 0) {
      int index = 1;
      for (Cvor<Tip> *pok = prvi->veza; pok != nullptr; pok = pok->veza) {
        if (index++ == i)
          return pok->element;
      }
    }
    return prvi->element;
  }
  const Tip &operator[](int i) const override {
    if (i < 0 || i >= brojElemenata())
      throw std::range_error("Indeks negativan ili veći od veličine niza!");
    if (i != 0) {
      int index = 1;
      for (Cvor<Tip> *pok = prvi->veza; pok != nullptr; pok = pok->veza) {
        if (index++ == i)
          return pok->element;
      }
    }
    return prvi->element;
  }
};

/*Testovi JednostrukaLista*/
bool test_JL_brojElemenata() {
  JednostrukaLista<int> jl;
  for (int i = 0; i < 7; i++)
    jl.dodajIza(i);
  if (jl.brojElemenata() == 7)
    return true;
  return false;
}

bool test_JL_trenutni() {
  JednostrukaLista<int> jl;
  for (int i = 3; i < 6; i++) {
    jl.dodajIza(i);
  }
  for (int i = 8; i < 12; i++) {
    jl.dodajIspred(i);
  }
  jl.trenutni() = jl.trenutni() + 6;
  if (jl.trenutni() == 9)
    return true;
  return false;
}

bool test_JL_const_trenutni() {
  JednostrukaLista<int> jl;
  for (int i = 16; i < 19; i++) {
    jl.dodajIza(i);
  }
  for (int i = 5; i < 8; i++) {
    jl.dodajIspred(i);
  }
  const JednostrukaLista<int> jl1 = jl;
  if (jl.trenutni() == 16)
    return true;
  return false;
}

bool test_JL_prethodni() {
  JednostrukaLista<int> jl;
  for (int i = 4; i < 7; i++) {
    jl.dodajIza(i);
  }
  for (int i = 15; i < 18; i++) {
    jl.dodajIspred(i);
    jl.prethodni();
  }
  if (jl.trenutni() == 17)
    return true;
  return false;
}

bool test_JL_sljedeci() {
  JednostrukaLista<int> jl;
  for (int i = 15; i < 18; i++) {
    jl.dodajIza(i);
    jl.sljedeci();
  }
  for (int i = 0; i < 4; i++) {
    jl.dodajIspred(i);
  }
  if (jl.trenutni() == 17)
    return true;
  return false;
}

bool test_JL_pocetak() {
  JednostrukaLista<int> jl;
  for (int i = 0; i < 4; i++) {
    jl.dodajIza(i);
    jl.sljedeci();
  }
  for (int i = 5; i < 8; i++) {
    jl.dodajIspred(i);
    jl.prethodni();
  }
  jl.pocetak();
  if (jl.trenutni() == 0)
    return true;
  return false;
}

bool test_JL_kraj() {
  JednostrukaLista<int> jl;
  for (int i = 0; i < 4; i++) {
    jl.dodajIza(i);
    jl.sljedeci();
  }
  for (int i = 5; i < 8; i++) {
    jl.dodajIspred(i);
    jl.prethodni();
  }
  jl.kraj();
  if (jl.trenutni() == 3)
    return true;
  return false;
}

bool test_JL_obrisi() {
  JednostrukaLista<int> jl;
  for (int i = 0; i < 7; i++) {
    jl.dodajIza(i);
    jl.sljedeci();
  }
  for (int i = 4; i < 7; i++)
    jl.prethodni();
  jl.obrisi();
  if (jl.trenutni() == 4)
    return true;
  return false;
}

bool test_JL_dodajIspred() {
  JednostrukaLista<std::string> jl;
  jl.dodajIspred("Hi");
  jl.dodajIspred("how");
  jl.prethodni();
  jl.dodajIspred("are");
  jl.prethodni();
  jl.dodajIspred("you");
  jl.sljedeci();
  if (jl.trenutni() == "how")
    return true;
  return false;
}

bool test_JL_dodajIza() {
  JednostrukaLista<double> jl;
  for (int i = 7; i < 14; i++) {
    jl.dodajIza(i);
    jl.sljedeci();
  }
  if (jl.trenutni() == 13)
    return true;
  return false;
}

bool test_JL_operatorUglasteZ() {
  JednostrukaLista<int> jl;
  for (int i = 0; i < 7; i++) {
    jl.dodajIza(0);
  }
  int suma = 0;
  for (int i = 0; i < 7; i++) {
    jl[i] = i + 1;
    suma += jl[i];
  }
  if (suma == 28)
    return true;
  return false;
}

void test_JL_trenutni_izuzetak() {
  JednostrukaLista<int> jl;
  try {
    auto x=jl.trenutni();
  } catch (...) {
    throw;
  }
}

void test_JL_prethodni_izuzetak() {
  JednostrukaLista<int> jl;
  try {
    jl.prethodni();
  } catch (...) {
    throw;
  }
}

void test_JL_sljedeci_izuzetak() {
  JednostrukaLista<int> jl;
  try {
    jl.sljedeci();
  } catch (...) {
    throw;
  }
}

void test_JL_pocetak_izuzetak() {
  JednostrukaLista<int> jl;
  try {
    jl.pocetak();
  } catch (...) {
    throw;
  }
}

void test_JL_kraj_izuzetak() {
  JednostrukaLista<int> jl;
  try {
    jl.kraj();
  } catch (...) {
    throw;
  }
}

void test_JL_obrisi_izuzetak() {
  JednostrukaLista<int> jl;
  try {
    jl.obrisi();
  } catch (...) {
    throw;
  }
}

void test_JL_operatorUglasteZ_izuzetak() {
  JednostrukaLista<int> jl;
  jl.dodajIspred(5);
  try {
    auto x=jl[1];
  } catch (...) {
    throw;
  }
}

int main() {
  std::cout << "Pripremna zadaca 2, Zadatak 1" << std::endl;
  std::cout << "Klasa NizLista:" << std::endl
            << "Test metode brojElemenata: " << std::boolalpha
            << test_NL_brojElemenata() << std::endl;
  std::cout << "Test metode trenutni: " << test_NL_trenutni() << std::endl;
  std::cout << "Test metode trenutni s konstantnom klasom: "
            << test_NL_const_trenutni() << std::endl;
  std::cout << "Test bacanja izuzetka metode trenutni: ";
  try {
    test_NL_trenutni_izuzetak();
  } catch (std::range_error e) {
    std::cout << e.what() << std::endl;
  }
  std::cout << "Test metode prethodni: " << test_NL_prethodni() << std::endl;
  std::cout << "Test bacanja izuzetka metode prethodni: ";
  try {
    test_NL_prethodni_izuzetak();
  } catch (std::range_error e) {
    std::cout << e.what() << std::endl;
  }
  std::cout << "Test metode sljedeci: " << test_NL_sljedeci() << std::endl;
  std::cout << "Test bacanja izuzetka metode sljedeci: ";
  try {
    test_NL_sljedeci_izuzetak();
  } catch (std::range_error e) {
    std::cout << e.what() << std::endl;
  }
  std::cout << "Test metode pocetak: " << test_NL_pocetak() << std::endl;
  std::cout << "Test bacanja izuzetka metode pocetak: ";
  try {
    test_NL_pocetak_izuzetak();
  } catch (std::range_error e) {
    std::cout << e.what() << std::endl;
  }
  std::cout << "Test metode kraj: " << test_NL_kraj() << std::endl;
  std::cout << "Test bacanja izuzetka metode kraj: ";
  try {
    test_NL_kraj_izuzetak();
  } catch (std::range_error e) {
    std::cout << e.what() << std::endl;
  }
  std::cout << "Test metode obrisi: " << test_NL_obrisi() << std::endl;
  std::cout << "Test bacanja izuzetka metode obrisi: ";
  try {
    test_NL_obrisi_izuzetak();
  } catch (std::range_error e) {
    std::cout << e.what() << std::endl;
  }
  std::cout << "Test metode dodajIspred: " << test_NL_dodajIspred()
            << std::endl;
  std::cout << "Test metode dodajIza: " << test_NL_dodajIza() << std::endl;
  std::cout << "Test preklopljenog operatora []: " << test_NL_operatorUglasteZ()
            << std::endl;
  std::cout << "Test bacanja izuzetka preklopljenog operatora []: ";
  try {
    test_NL_operatorUglasteZ_izuzetak();
  } catch (std::range_error e) {
    std::cout << e.what() << std::endl << std::endl;
  }
  std::cout << "Klasa JednostrukaLista:" << std::endl
            << "Test metode brojElemenata: " << std::boolalpha
            << test_JL_brojElemenata() << std::endl;
  std::cout << "Test metode trenutni: " << test_JL_trenutni() << std::endl;
  std::cout << "Test metode trenutni s konstantnom klasom: "
            << test_JL_const_trenutni() << std::endl;
  std::cout << "Test bacanja izuzetka metode trenutni: ";
  try {
    test_JL_trenutni_izuzetak();
  } catch (std::range_error e) {
    std::cout << e.what() << std::endl;
  }
  std::cout << "Test metode prethodni: " << test_JL_prethodni() << std::endl;
  std::cout << "Test bacanja izuzetka metode prethodni: ";
  try {
    test_JL_prethodni_izuzetak();
  } catch (std::range_error e) {
    std::cout << e.what() << std::endl;
  }
  std::cout << "Test metode sljedeci: " << test_JL_sljedeci() << std::endl;
  std::cout << "Test bacanja izuzetka metode sljedeci: ";
  try {
    test_JL_sljedeci_izuzetak();
  } catch (std::range_error e) {
    std::cout << e.what() << std::endl;
  }
  std::cout << "Test metode pocetak: " << test_JL_pocetak() << std::endl;
  std::cout << "Test bacanja izuzetka metode pocetak: ";
  try {
    test_JL_pocetak_izuzetak();
  } catch (std::range_error e) {
    std::cout << e.what() << std::endl;
  }
  std::cout << "Test metode kraj: " << test_JL_kraj() << std::endl;
  std::cout << "Test bacanja izuzetka metode kraj: ";
  try {
    test_JL_kraj_izuzetak();
  } catch (std::range_error e) {
    std::cout << e.what() << std::endl;
  }
  std::cout << "Test metode obrisi: " << test_JL_obrisi() << std::endl;
  std::cout << "Test bacanja izuzetka metode obrisi: ";
  try {
    test_JL_obrisi_izuzetak();
  } catch (std::range_error e) {
    std::cout << e.what() << std::endl;
  }
  std::cout << "Test metode dodajIspred: " << test_JL_dodajIspred()
            << std::endl;
  std::cout << "Test metode dodajIza: " << test_JL_dodajIza() << std::endl;
  std::cout << "Test preklopljenog operatora []: " << test_JL_operatorUglasteZ()
            << std::endl;
  std::cout << "Test bacanja izuzetka preklopljenog operatora []: ";
  try {
    test_JL_operatorUglasteZ_izuzetak();
  } catch (std::range_error e) {
    std::cout << e.what() << std::endl << std::endl;
  }
  return 0;
}
