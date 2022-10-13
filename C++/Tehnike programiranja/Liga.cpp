#include <algorithm>
#include <cstring>
#include <iomanip>
#include <iostream>
#include <memory>
#include <stdexcept>
#include <vector>

using namespace std;

class Tim {
  char ime_tima[20];
  int broj_odigranih, broj_pobjeda, broj_nerijesenih, broj_poraza, broj_datih,
      broj_primljenih, broj_poena;

public:
  Tim(const char ime[])
      : broj_odigranih{}, broj_pobjeda{}, broj_nerijesenih{}, broj_poraza{},
        broj_datih{}, broj_primljenih{}, broj_poena{} {
    if (strlen(ime) > 20)
      throw range_error("Predugacko ime tima");
    strcpy(ime_tima, ime);
  }
  void ObradiUtakmicu(int broj_datih, int broj_primljenih) {
    if (broj_datih < 0 || broj_primljenih < 0)
      throw range_error("Neispravan broj golova");
    broj_odigranih++;
    this->broj_datih += broj_datih;
    this->broj_primljenih += broj_primljenih;
    if (broj_datih > broj_primljenih) {
      broj_pobjeda++;
      broj_poena += 3;
    } else if (broj_datih == broj_primljenih) {
      broj_nerijesenih++;
      broj_poena += 1;
    } else {
      broj_poraza++;
    }
  }
  const char *DajImeTima() const { return ime_tima; }
  int DajBrojPoena() const { return broj_poena; }
  int DajGolRazliku() const { return broj_datih - broj_primljenih; }
  void IspisiPodatke() const {
    cout << setw(20) << left << ime_tima << setw(4) << right << broj_odigranih
         << setw(4) << broj_pobjeda << setw(4) << broj_nerijesenih << setw(4)
         << broj_poraza << setw(4) << broj_datih << setw(4) << broj_primljenih
         << setw(4) << broj_poena << endl;
  }
};

bool Krit(const shared_ptr<Tim> t1, const shared_ptr<Tim> t2) {
  if (t1->DajBrojPoena() > t2->DajBrojPoena())
    return true;
  if (t1->DajBrojPoena() == t2->DajBrojPoena()) {
    if (t1->DajGolRazliku() > t2->DajGolRazliku())
      return true;
    if (t1->DajGolRazliku() == t2->DajGolRazliku()) {
      if (strcmp(t1->DajImeTima(), t2->DajImeTima()) < 0)
        return true;
    }
  }
  return false;
}

class Liga {
  vector<shared_ptr<Tim>> timovi;

public:
  Liga() {}
  explicit Liga(initializer_list<Tim> lista_timova) {
    timovi.resize(lista_timova.size());
    auto it = lista_timova.begin();
    int i = 0;
    while (it != lista_timova.end()) {
      timovi.at(i) = make_shared<Tim>(Tim(*it));
      it++;
      i++;
    }
  }
  ~Liga() {
    for (int i = 0; i < timovi.size(); i++) {
      timovi.at(i) = nullptr;
    }
    timovi.resize(0);
  }
  Liga(const Liga &l) {
    timovi.resize(l.timovi.size());
    for (int i = 0; i < timovi.size(); i++) {
      timovi.at(i) = make_shared<Tim>(*(l.timovi.at(i)));
    }
  }
  Liga(Liga &&l) {
    timovi.resize(l.timovi.size());
    for (int i = 0; i < timovi.size(); i++) {
      timovi.at(i) = make_shared<Tim>(*(l.timovi.at(i)));
      l.timovi.at(i) = nullptr;
    }
    l.timovi.resize(0);
  }
  Liga &operator=(const Liga &l) {
    timovi.resize(l.timovi.size());
    for (int i = 0; i < timovi.size(); i++) {
      timovi.at(i) = make_shared<Tim>(*(l.timovi.at(i)));
    }
    return *this;
  }
  Liga &operator=(Liga &&l) {
    timovi.resize(l.timovi.size());
    for (int i = 0; i < timovi.size(); i++) {
      timovi.at(i) = make_shared<Tim>(*(l.timovi.at(i)));
      l.timovi.at(i) = nullptr;
    }
    l.timovi.resize(0);
    return *this;
  }
  void DodajNoviTim(const char ime_tima[]) {
    if (strlen(ime_tima) > 20)
      throw range_error("Predugacko ime tima");
    for (int i = 0; i < timovi.size(); i++) {
      if (timovi.at(i) == nullptr) {
        timovi.at(i) = make_shared<Tim>(Tim(ime_tima));
        break;
      }
      if (!strcmp(timovi.at(i)->DajImeTima(), ime_tima))
        throw logic_error("Tim vec postoji");
    }
  }
  void RegistrirajUtakmicu(const char tim1[], const char tim2[], int rezultat_1,
                           int rezultat_2) {
    int usl = -2;
    for (int i = 0; i < timovi.size(); i++) {
      if (!strcmp(timovi.at(i)->DajImeTima(), tim1))
        usl++;
      if (!strcmp(timovi.at(i)->DajImeTima(), tim2))
        usl++;
    }
    if (usl != 0)
      throw logic_error("Tim nije nadjen");
    if (rezultat_1 < 0 || rezultat_2 < 0)
      throw range_error("Neispravan broj golova");
    for (int i = 0; i < timovi.size(); i++) {
      if (!strcmp(timovi.at(i)->DajImeTima(), tim1)) {
        timovi.at(i)->ObradiUtakmicu(rezultat_1, rezultat_2);
      }
      if (!strcmp(timovi.at(i)->DajImeTima(), tim2)) {
        timovi.at(i)->ObradiUtakmicu(rezultat_2, rezultat_1);
      }
    }
  }
  void IspisiTabelu() {
    sort(timovi.begin(), timovi.end(), Krit);
    for (int i = 0; i < timovi.size(); i++)
      timovi.at(i)->IspisiPodatke();
  }
};

int main() {
  try {
    Liga l(
        {"Celik", "Borac", "Jedinstvo", "Zeljeznicar", "Zrinjski", "Sarajevo"});
    l.IspisiTabelu();
    cout << endl;
    while (1) {
      cout << "Unesite ime prvog tima (ENTER za kraj): ";
      string x;
      getline(cin, x);
      if (x.size() == 0)
        break;
      cout << "Unesite ime drugog tima: ";
      string y;
      getline(cin, y);
      cout << "Unesite broj postignutih golova za oba tima: ";
      int n, m;
      cin >> n >> m;
      cin.ignore(10000, '\n');
      char ime1[20]{}, ime2[20]{};
      for (int i = 0; i < x.size(); i++) {
        if (i > 19)
          break;
        ime1[i] = x.at(i);
      }
      for (int i = 0; i < y.size(); i++) {
        if (i > 19)
          break;
        ime2[i] = y.at(i);
      }
      try {
        l.RegistrirajUtakmicu(ime1, ime2, n, m);
      } catch (logic_error e) {
        cout << e.what() << endl;
      } catch (range_error e) {
        cout << e.what() << endl;
      }
      l.IspisiTabelu();
      cout << endl;
    }
  } catch (range_error e) {
    cout << e.what() << endl;
  } catch (bad_alloc) {
    cout << "Nedovoljno memorije" << endl;
  }
  return 0;
}
