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

template <typename Tip> class DvostrukaLista : public Lista<Tip> {
  struct Cvor {
    Tip element;
    Cvor *prethodni{}, *sljedeci{};
    Cvor(const Tip &el, Cvor *prethodni = nullptr, Cvor *sljedeci = nullptr)
        : element(el), prethodni(prethodni), sljedeci(sljedeci) {}
  };
  Cvor *prvi, *zadnji, *tr;
  int brojEl;
  void ObrisiListu() {
    if (prvi) {
      if (prvi->sljedeci) {
        tr = prvi->sljedeci;
        delete prvi;
        prvi = nullptr;
        while (tr->sljedeci) {
          prvi = tr->sljedeci;
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
      brojEl=0;
    }
  }
  void Postavi(const DvostrukaLista &dl) {
    prvi = new Cvor(dl.prvi->element, nullptr, nullptr);
    tr = zadnji = prvi;
    Cvor *pok1 = prvi;
    for (Cvor *pok = dl.prvi->sljedeci; pok != nullptr; pok = pok->sljedeci) {
      pok1->sljedeci = new Cvor(pok->element, pok1, nullptr);
      pok1 = pok1->sljedeci;
      if (!pok->sljedeci)
        zadnji = pok1;
      if (pok == dl.tr)
        tr = pok1;
    }
    brojEl=dl.brojEl;
  }
  void PraznaLista() const {
    if (!brojEl)
      throw std::range_error("Lista je prazna!");
  }

public:
  DvostrukaLista() : prvi{}, zadnji{}, tr{},brojEl{} {}
  DvostrukaLista(const DvostrukaLista &dl) { Postavi(dl); }
  DvostrukaLista(DvostrukaLista &&dl) {
    prvi = dl.prvi;
    zadnji = dl.zadnji;
    tr = dl.tr;
    brojEl=dl.brojEl;
    dl.prvi = nullptr;
    dl.zadnji = nullptr;
    dl.tr = nullptr;
    dl.brojEl=0;
  }
  ~DvostrukaLista() { ObrisiListu(); }
  DvostrukaLista &operator=(const DvostrukaLista &dl) {
    if (this->prvi != dl.prvi) {
      ObrisiListu();
      Postavi(dl);
    }
    return *this;
  }
  DvostrukaLista &operator=(DvostrukaLista &&dl) {
    if (this->prvi != dl.prvi) {
      ObrisiListu();
      prvi = dl.prvi;
      zadnji = dl.zadnji;
      tr = dl.tr;
      brojEl=dl.brojEl;
      dl.prvi = nullptr;
      dl.zadnji = nullptr;
      dl.tr = nullptr;
      dl.brojEl=0;
    }
    return *this;
  }
  int brojElemenata() const override {
    return brojEl;
  }
  Tip &trenutni() override {
    PraznaLista();
    return tr->element;
  }
  const Tip &trenutni() const override {
    PraznaLista();
    return tr->element;
  }
  bool prethodni() override {
    PraznaLista();
    if (tr == prvi)
      return false;
    else
      tr = tr->prethodni;
    return true;
  }
  bool sljedeci() override {
    PraznaLista();
    if (tr == zadnji)
      return false;
    else
      tr = tr->sljedeci;
    return true;
  }
  void pocetak() override {
    PraznaLista();
    tr = prvi;
  }
  void kraj() override {
    PraznaLista();
    tr = zadnji;
  }
  void obrisi() override {
    PraznaLista();
    if (!prvi->sljedeci) {
      delete prvi;
      prvi = nullptr;
      zadnji = nullptr;
      tr = nullptr;
    } else if (prvi->sljedeci && tr == prvi ) {
      prvi = prvi->sljedeci;
      prvi->prethodni = nullptr;
      delete tr;
      tr = prvi;
    } else if(prvi->sljedeci && tr==zadnji){
        zadnji=tr->prethodni;
        zadnji->sljedeci=nullptr;
        delete tr;
        tr=zadnji;
    } else {
      Cvor *x = tr->prethodni;
      x->sljedeci = tr->sljedeci;
        tr->sljedeci->prethodni = x;
        delete tr;
        tr = x->sljedeci;
    }
    brojEl--;
  }
  void dodajIspred(const Tip &el) override {
    if (!prvi) {
      prvi = new Cvor(el, nullptr, nullptr);
      tr = zadnji = prvi;
    } else if (prvi && tr == prvi) {
      prvi = new Cvor(el, nullptr, tr);
      tr->prethodni = prvi;
    } else {
      Cvor *x = tr->prethodni;
      x->sljedeci = new Cvor(el, x, tr);
      tr->prethodni = x->sljedeci;
    }
    brojEl++;
  }
  void dodajIza(const Tip &el) override {
    if (!prvi) {
      prvi = new Cvor(el, nullptr, nullptr);
      tr = zadnji = prvi;
    } else if (prvi && !tr->sljedeci) {
      zadnji = tr->sljedeci = new Cvor(el, tr, nullptr);
    } else {
      Cvor *x = tr->sljedeci;
      x->prethodni = tr->sljedeci = new Cvor(el, tr, x);
    }
    brojEl++;
  }
  Tip &operator[](int i) override {
    if (i < 0 || i >= brojElemenata())
      throw std::range_error("Indeks negativan ili veći od veličine niza!");
    if(i==brojEl-1) return zadnji->element;
    else if (i!=0 && i!=brojEl-1) {
      int index = 1;
      for (Cvor *pok = prvi->sljedeci; pok != nullptr; pok = pok->sljedeci) {
        if (index++ == i)
          return pok->element;
      }
    }
    return prvi->element;
  }
  const Tip &operator[](int i) const override {
    if (i < 0 || i >= brojElemenata())
      throw std::range_error("Indeks negativan ili veći od veličine niza!");
    if(i==brojEl-1) return zadnji->element;
    else if (i!=0 && i!=brojEl-1) {
      int index = 1;
      for (Cvor *pok = prvi->sljedeci; pok != nullptr; pok = pok->sljedeci) {
        if (index++ == i)
          return pok->element;
      }
    }
    return prvi->element;
  }
};

template<typename Tip>
class Red {
    DvostrukaLista<Tip> dl;
    public:
    Red(){}
    void brisi() {
        int n=dl.brojElemenata();
        if(n) dl.pocetak();
        for(int i=0; i<n; i++) 
            dl.obrisi();
    }
    void stavi(const Tip& el) {
        if(dl.brojElemenata()) dl.kraj();
        dl.dodajIza(el);
    }
    Tip skini() {
        if(!dl.brojElemenata()) throw std::domain_error("Red je prazan!");
        dl.pocetak();
        Tip x=dl.trenutni();
        dl.obrisi();
        return x;
    }
    Tip& celo() {
        if(!dl.brojElemenata()) throw std::domain_error("Red je prazan!");
        dl.pocetak();
        return dl.trenutni();
    }
    int brojElemenata() {
        return dl.brojElemenata();
    }
};

/*Testovi za klasu Red*/
bool test_brisi() {
    Red<int> r;
    for(int i=0; i<51; i+=5)
        r.stavi(i);
    r.brisi();
    if(!r.brojElemenata()) return true;
    return false;
}

bool test_stavi() {
    Red<int> r;
    for(int i=0; i<51; i+=5)
        r.stavi(i);
    if(r.celo()==0) return true;
    return false;
}

bool test_skini() {
    Red<int> r;
    for(int i=12; i>0; i--)
        r.stavi(i);
    if(r.skini()==12 && r.celo()==11) return true;
    return false;
}

bool test_celo() {
    Red<int> r;
    for(int i=0; i<17; i+=3)
        r.stavi(i);
    r.celo()=55;
    if(r.celo()==55 && r.skini()==55) return true;
    return false;
}

bool test_brojElemenata() {
    Red<int> r;
    for(int i=0; i<10; i++)
        r.stavi(i);
    if(r.brojElemenata()==10) return true;
    return false;
}
int main() {
    std::cout << "Klasa Red:\n";
    std::cout<<"Test metode brisi: "<<std::boolalpha<<test_brisi()<<std::endl;
    std::cout<<"Test metode stavi: "<<test_stavi()<<std::endl;
    std::cout<<"Test metode skini: "<<test_skini()<<std::endl;
    std::cout<<"Test metode celo: "<<test_celo()<<std::endl;
    std::cout<<"Test metode brojElemenata: "<<test_brojElemenata()<<std::endl;
    return 0;
}
