#include <iostream>

using namespace std;

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

template <typename Tip> class JednostrukaLista : public Lista<Tip> {
  struct Cvor {
    Tip element;
    Cvor *veza;
    Cvor(const Tip &el, Cvor *veza = nullptr) : element(el), veza(veza) {}
  };
  Cvor *prvi, *zadnji, *tr;
  int brojEl;
  void obrisiListu() {
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
      zadnji = nullptr;
      brojEl = 0;
    }
  }
  void inicijaliziraj(const JednostrukaLista &jl) {
    prvi = new Cvor(jl.prvi->element);
    tr = zadnji = prvi;
    Cvor *pok1 = prvi;
    for (Cvor *pok = jl.prvi->veza; pok != nullptr; pok = pok->veza) {
      pok1->veza = new Cvor(pok->element);
      pok1 = pok1->veza;
      if (!pok->veza)
        zadnji = pok1;
      if (pok == jl.tr)
        tr = pok1;
    }
    brojEl = jl.brojEl;
  }

public:
  JednostrukaLista() : prvi{}, zadnji{}, tr{}, brojEl{} {}
  JednostrukaLista(const JednostrukaLista &jl) { inicijaliziraj(jl); }
  JednostrukaLista(JednostrukaLista &&jl) {
    prvi = jl.prvi;
    zadnji = jl.zadnji;
    tr = jl.tr;
    brojEl = jl.brojEl;
    jl.prvi = nullptr;
    jl.tr = nullptr;
    jl.zadnji = nullptr;
    jl.brojEl = 0;
  }
  ~JednostrukaLista() { obrisiListu(); }
  JednostrukaLista &operator=(const JednostrukaLista &jl) {
    if (this->prvi != jl.prvi) {
      obrisiListu();
      inicijaliziraj(jl);
      return *this;
    }
  }
  JednostrukaLista &operator=(JednostrukaLista &&jl) {
    if (this->prvi != jl.prvi) {
      obrisiListu();
      prvi = jl.prvi;
      zadnji = jl.zadnji;
      tr = jl.tr;
      brojEl = jl.brojEl;
      jl.prvi = nullptr;
      jl.tr = nullptr;
      jl.zadnji = nullptr;
      jl.brojEl = 0;
    }
    return *this;
  }
  int brojElemenata() const override { return brojEl; }
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
        for (Cvor *pok = prvi->veza; pok != nullptr; pok = pok->veza) {
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
    else
      tr = zadnji;
  }
  void obrisi() override {
    if (!prvi)
      throw std::range_error("Lista je prazna!");
    else if (!prvi->veza) {
      delete prvi;
      prvi = nullptr;
      tr = nullptr;
      zadnji = nullptr;
    } else if (prvi->veza && tr == prvi) {
      prvi = prvi->veza;
      delete tr;
      tr = prvi;
    } else {
      for (Cvor *pok = prvi; pok != nullptr; pok = pok->veza) {
        if (pok->veza == tr) {
          pok->veza = tr->veza;
          delete tr;
          if (pok->veza)
            tr = pok->veza;
          else {
            tr = zadnji = pok;
          }
          break;
        }
      }
    }
    brojEl--;
  }
  void dodajIspred(const Tip &el) override {
    if (!prvi) {
      prvi = new Cvor(el);
      tr = zadnji = prvi;
    } else if (prvi && tr == prvi) {
      prvi = new Cvor(el, tr);
    } else {
      for (Cvor *pok = prvi; pok != nullptr; pok = pok->veza) {
        if (pok->veza == tr) {
          pok->veza = new Cvor(el, tr);
          break;
        }
      }
    }
    brojEl++;
  }
  void dodajIza(const Tip &el) override {
    if (!prvi) {
      prvi = new Cvor(el);
      tr = zadnji = prvi;
    } else if (prvi && tr == zadnji) {
      zadnji = tr->veza = new Cvor(el, nullptr);
    } else {
      for (Cvor *pok = prvi; pok != nullptr; pok = pok->veza) {
        if (pok == tr) {
          Cvor *pom = pok->veza;
          pok->veza = new Cvor(el, pom);
          break;
        }
      }
    }
    brojEl++;
  }
  Tip &operator[](int i) override {
    if (i < 0 || i >= brojElemenata())
      throw std::range_error("Indeks negativan ili veći od veličine niza!");
    if (i == brojEl - 1)
      return zadnji->element;
    else if (i != 0 && i != brojEl - 1) {
      int index = 1;
      for (Cvor *pok = prvi->veza; pok != nullptr; pok = pok->veza) {
        if (index++ == i)
          return pok->element;
      }
    }
    return prvi->element;
  }
  const Tip &operator[](int i) const override {
    if (i < 0 || i >= brojElemenata())
      throw std::range_error("Indeks negativan ili veći od veličine niza!");
    if (i == brojEl - 1)
      return zadnji->element;
    else if (i != 0 && i != brojEl - 1) {
      int index = 1;
      for (Cvor *pok = prvi->veza; pok != nullptr; pok = pok->veza) {
        if (index++ == i)
          return pok->element;
      }
    }
    return prvi->element;
  }
};

template <typename Tip> class Stek {
  JednostrukaLista<Tip> jl;

public:
  Stek() {}
  void brisi() {
    int br = jl.brojElemenata();
    if (br)
      jl.pocetak();
    for (int i = 0; i < br; i++)
      jl.obrisi();
  }
  void stavi(const Tip &el) {
    jl.dodajIspred(el);
    jl.prethodni();
  }
  Tip skini() {
    if (!jl.brojElemenata())
      throw std::domain_error("Stek je prazan!");
    jl.pocetak();
    Tip x = jl.trenutni();
    jl.obrisi();
    return x;
  }
  Tip &vrh() {
    if (!jl.brojElemenata())
      throw std::domain_error("Stek je prazan!");
    jl.pocetak();
    return jl.trenutni();
  }
  int brojElemenata() { return jl.brojElemenata(); }
};

/*Testovi za klasu Stek*/
bool test_brisi() {
  Stek<int> s;
  for (int i = 0; i < 51; i += 5)
    s.stavi(i);
  s.brisi();
  if (!s.brojElemenata())
    return true;
  return false;
}

bool test_stavi() {
  Stek<int> s;
  for (int i = 0; i < 51; i += 5)
    s.stavi(i);
  if (s.vrh() == 50)
    return true;
  return false;
}

bool test_skini() {
  Stek<int> s;
  for (int i = 0; i < 12; i++)
    s.stavi(i);
  if (s.skini() == 11 && s.vrh() == 10)
    return true;
  return false;
}

bool test_vrh() {
  Stek<int> s;
  for (int i = 0; i < 17; i += 3)
    s.stavi(i);
  s.vrh() = 55;
  if (s.vrh() == 55 && s.skini() == 55)
    return true;
  return false;
}

bool test_brojElemenata() {
  Stek<int> s;
  for (int i = 0; i < 10; i++)
    s.stavi(i);
  if (s.brojElemenata() == 10)
    return true;
  return false;
}

int main() {
  std::cout << "Klasa Stek:\n";
  std::cout << "Test metode brisi: " << std::boolalpha << test_brisi()
            << std::endl;
  std::cout << "Test metode stavi: " << test_stavi() << std::endl;
  std::cout << "Test metode skini: " << test_skini() << std::endl;
  std::cout << "Test metode vrh: " << test_vrh() << std::endl;
  std::cout << "Test metode brojElemenata: " << test_brojElemenata()
            << std::endl;

  return 0;
}
