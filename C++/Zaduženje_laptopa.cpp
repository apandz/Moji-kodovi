#include <iostream>
#include <map>
#include <stdexcept>
#include <string>
#include <vector>

using namespace std;

class Student {
  int brind;
  string gods, ime, ad, brtel;

public:
  Student(int brind1, string gods1, string ime1, string ad1, string brtel1);
  int DajIndeks() const { return brind; }
  string DajGodinuStudija() const {
    if (gods == "1")
      return "1/B";
    if (gods == "2")
      return "2/B";
    if (gods == "3")
      return "3/B";
    return gods;
  }
  string DajImePrezime() const { return ime; }
  string DajAdresu() const { return ad; }
  string DajTelefon() const { return brtel; }
  void Ispisi() const {
    cout << "Broj indeksa: " << brind << endl << "Godina studija: ";
    if (gods == "1")
      cout << "1/B";
    else if (gods == "2")
      cout << "2/B";
    else if (gods == "3")
      cout << "3/B";
    else
      cout << gods;
    cout << "\nIme i prezime: " << ime << endl
         << "Adresa: " << ad << endl
         << "Telefon: " << brtel << endl;
  }
};

Student::Student(int brind1, string gods1, string ime1, string ad1,
                 string brtel1) {
  if (brind1 / 10000 == 0) {
    throw domain_error("Neispravni parametri");
  }
  brind = brind1;
  vector<string> gdst{"1",   "2",   "3",   "1/B", "2/B", "3/B",
                      "1/M", "2/M", "1/D", "2/D", "3/D"};
  int usl = 0;
  for (int i = 0; i < gdst.size(); i++) {
    if (gdst.at(i) == gods1) {
      usl = 1;
      break;
    }
  }
  if (usl == 0) {
    throw domain_error("Neispravni parametri");
  }
  gods = gods1;
  usl = 0;
  for (int i = 0; i < ime1.length(); i++) {
    if (ime1.at(i) != ' ' && usl == 0)
      usl = 1;
    if (ime1.at(i) == ' ' && usl == 0) {
      ime1.erase(i, 1);
      i--;
      continue;
    }
    if (ime1.at(i) == ' ' && usl == 1)
      usl = 0;
  }
  if (ime1.size() == 0) {
    throw domain_error("Neispravni parametri");
  }
  if (ime1.at(ime1.size() - 1) == ' ')
    ime1.erase(ime1.size() - 1, 1);
  ime = ime1;
  usl = 0;
  for (int i = 0; i < ad1.length(); i++) {
    if (ad1.at(i) != ' ' && usl == 0)
      usl = 1;
    if (ad1.at(i) == ' ' && usl == 0) {
      ad1.erase(i, 1);
      i--;
      continue;
    }
    if (ad1.at(i) == ' ' && usl == 1)
      usl = 0;
  }
  if (ad1.size() == 0) {
    throw domain_error("Neispravni parametri");
  }
  if (ad1.at(ad1.size() - 1) == ' ')
    ad1.erase(ad1.size() - 1, 1);
  ad = ad1;
  usl = 0;
  for (int i = 0; i < brtel1.length(); i++) {
    if (brtel1.at(i) > '0' - 1 && brtel1.at(i) < '9' + 1) {
      if (usl == 0)
        usl = 1;
      else if (usl == 2)
        usl = 3;
      else if (usl == 4)
        usl = 5;
    } else if (brtel1.at(i) == '/' && usl == 1)
      usl = 2;
    else if (brtel1.at(i) == '-' && usl == 3)
      usl = 4;
    else {
      usl = -1;
      break;
    }
  }
  if (usl != 5) {
    throw domain_error("Neispravni parametri");
  }
  brtel = brtel1;
}

class Laptop {
  int ev_broj;
  string naziv, karakteristike;
  Student *kod_koga_je;

public:
  Laptop(int evbr, string naz, string kar)
      : ev_broj(evbr), naziv(naz), karakteristike(kar), kod_koga_je{} {
    if (evbr <= 0)
      throw domain_error("Neispravni parametri");
  }
  int DajEvidencijskiBroj() const { return ev_broj; }
  string DajNaziv() const { return naziv; }
  string DajKarakteristike() const { return karakteristike; }
  void Zaduzi(Student &s) {
    if (kod_koga_je != nullptr)
      throw domain_error("Laptop vec zaduzen");
    kod_koga_je = &s;
  }
  void Razduzi() { kod_koga_je = nullptr; }
  bool DaLiJeZaduzen() const {
    if (kod_koga_je == nullptr)
      return false;
    return true;
  }
  Student &DajKodKogaJe() const {
    if (kod_koga_je == nullptr)
      throw domain_error("Laptop nije zaduzen");
    return *kod_koga_je;
  }
  Student *DajPokKodKogaJe() const { return kod_koga_je; }
  void Ispisi() const {
    cout << "Evidencijski broj: " << ev_broj << endl
         << "Naziv: " << naziv << endl
         << "Karakteristike: " << karakteristike << endl;
  }
};

class Administracija {
  map<int, Student *> s;
  map<int, Laptop *> l;

public:
  Administracija() {}
  Administracija(const Administracija &adm);
  Administracija(Administracija &&adm);
  ~Administracija();
  Administracija &operator=(const Administracija &adm);
  Administracija &operator=(Administracija &&adm);
  void RegistrirajNovogStudenta(int brind1, string gods1, string ime1,
                                string ad1, string brtel1);
  void RegistrirajNoviLaptop(int evbr, string naz, string kar);
  Student &NadjiStudenta(int brind1);
  Student NadjiStudenta(int brind1) const;
  Laptop &NadjiLaptop(int evbr);
  Laptop NadjiLaptop(int evbr) const;
  void IzlistajStudente() const;
  void IzlistajLaptope() const;
  void ZaduziLaptop(int brind1, int evbr1);
  int NadjiSlobodniLaptop() const;
  void RazduziLaptop(int evbr1);
  void PrikaziZaduzenja() const;
};

Administracija::Administracija(const Administracija &adm) {
  auto it2 = adm.s.begin();
  while (it2 != adm.s.end()) {
    s.insert(
        {it2->first,
         new Student(it2->second->DajIndeks(), it2->second->DajGodinuStudija(),
                     it2->second->DajImePrezime(), it2->second->DajAdresu(),
                     it2->second->DajTelefon())});
    it2++;
  }
  auto it3 = adm.l.begin();
  while (it3 != adm.l.end()) {
    l.insert({it3->first, new Laptop(it3->second->DajEvidencijskiBroj(),
                                     it3->second->DajNaziv(),
                                     it3->second->DajKarakteristike())});
    it3++;
  }
}

Administracija::Administracija(Administracija &&adm) {
  s = adm.s;
  l = adm.l;
  adm.s.clear();
  adm.l.clear();
}

Administracija::~Administracija() {
  auto it = s.begin();
  while (it != s.end()) {
    delete it->second;
    it++;
  }
  s.clear();
  auto it1 = l.begin();
  while (it1 != l.end()) {
    delete it1->second;
    it1++;
  }
  l.clear();
}

Administracija &Administracija::operator=(const Administracija &adm) {
  auto it = s.begin();
  while (it != s.end()) {
    delete it->second;
    it++;
  }
  s.clear();
  auto it1 = l.begin();
  while (it1 != l.end()) {
    delete it1->second;
    it1++;
  }
  l.clear();
  auto it2 = adm.s.begin();
  while (it2 != adm.s.end()) {
    s.insert(
        {it2->first,
         new Student(it2->second->DajIndeks(), it2->second->DajGodinuStudija(),
                     it2->second->DajImePrezime(), it2->second->DajAdresu(),
                     it2->second->DajTelefon())});
    it2++;
  }
  auto it3 = adm.l.begin();
  while (it3 != adm.l.end()) {
    l.insert({it3->first, new Laptop(it3->second->DajEvidencijskiBroj(),
                                     it3->second->DajNaziv(),
                                     it3->second->DajKarakteristike())});
    it3++;
  }
  return *this;
}

Administracija &Administracija::operator=(Administracija &&adm) {
  auto it = s.begin();
  while (it != s.end()) {
    delete it->second;
    it++;
  }
  s.clear();
  auto it1 = l.begin();
  while (it1 != l.end()) {
    delete it1->second;
    it1++;
  }
  l.clear();
  s = adm.s;
  l = adm.l;
  adm.s.clear();
  adm.l.clear();
  return *this;
}

void Administracija::RegistrirajNovogStudenta(int brind1, string gods1,
                                              string ime1, string ad1,
                                              string brtel1) {
  auto it = s.begin();
  while (it != s.end()) {
    if (it->first == brind1) {
      throw domain_error("Student s tim indeksom vec postoji");
    }
    it++;
  }
  s.insert({brind1, new Student(brind1, gods1, ime1, ad1, brtel1)});
}

void Administracija::RegistrirajNoviLaptop(int evbr, string naz, string kar) {
  auto it = l.begin();
  while (it != l.end()) {
    if (it->first == evbr) {
      throw domain_error("Laptop s tim evidencijskim brojem vec postoji");
    }
    it++;
  }
  l.insert({evbr, new Laptop(evbr, naz, kar)});
}

Student &Administracija::NadjiStudenta(int brind1) {
  auto it = s.begin();
  while (it != s.end()) {
    if (it->first == brind1) {
      return *it->second;
    }
    it++;
  }
  throw domain_error("Student nije nadjen");
  return *it->second;
}

Student Administracija::NadjiStudenta(int brind1) const {
  auto it = s.begin();
  while (it != s.end()) {
    if (it->first == brind1) {
      return Student(it->first, it->second->DajGodinuStudija(),
                     it->second->DajImePrezime(), it->second->DajAdresu(),
                     it->second->DajTelefon());
    }
    it++;
  }
  throw domain_error("Student nije nadjen");
  return *it->second;
}

Laptop &Administracija::NadjiLaptop(int evbr) {
  auto it = l.begin();
  while (it != l.end()) {
    if (it->first == evbr) {
      return *it->second;
    }
    it++;
  }
  throw domain_error("Laptop nije nadjen");
  return *it->second;
}

Laptop Administracija::NadjiLaptop(int evbr) const {
  auto it = l.begin();
  while (it != l.end()) {
    if (it->first == evbr) {
      return Laptop(it->first, it->second->DajNaziv(),
                    it->second->DajKarakteristike());
    }
    it++;
  }
  throw domain_error("Laptop nije nadjen");
  return *it->second;
}

void Administracija::IzlistajStudente() const {
  auto it = s.begin();
  while (it != s.end()) {
    it->second->Ispisi();
    cout << endl;
    it++;
  }
}

void Administracija::IzlistajLaptope() const {
  auto it = l.begin();
  while (it != l.end()) {
    it->second->Ispisi();
    if (it->second->DaLiJeZaduzen()) {
      cout << "Zaduzio(la): " << it->second->DajPokKodKogaJe()->DajImePrezime()
           << " (" << it->second->DajPokKodKogaJe()->DajIndeks() << ")" << endl;
    }
    cout << endl;
    it++;
  }
}

void Administracija::ZaduziLaptop(int brind1, int evbr1) {
  try {
    Student &stud = NadjiStudenta(brind1);
    auto it1 = l.begin();
    while (it1 != l.end()) {
      if (it1->second->DaLiJeZaduzen()) {
        if (it1->second->DajPokKodKogaJe()->DajIndeks() == brind1)
          break;
      }
      it1++;
    }
    if (it1 != l.end())
      throw domain_error("Student je vec zaduzio laptop");
    Laptop &lapt = NadjiLaptop(evbr1);
    if (lapt.DaLiJeZaduzen())
      throw domain_error("Laptop vec zaduzen");
    lapt.Zaduzi(stud);
  } catch (...) {
    throw;
  }
}

int Administracija::NadjiSlobodniLaptop() const {
  auto it = l.begin();
  while (it != l.end()) {
    if (!it->second->DaLiJeZaduzen()) {
      return it->first;
    }
    it++;
  }
  if (it == l.end())
    throw domain_error("Nema slobodnih laptopa");
  return 0;
}

void Administracija::RazduziLaptop(int evbr1) {
  try {
    Laptop &lapt = NadjiLaptop(evbr1);
    if (!lapt.DaLiJeZaduzen())
      throw domain_error("Laptop nije zaduzen");
    lapt.Razduzi();
  } catch (...) {
    throw;
  }
}

void Administracija::PrikaziZaduzenja() const {
  auto it = l.begin();
  int usl = 0;
  while (it != l.end()) {
    if (it->second->DaLiJeZaduzen()) {
      usl = 1;
      cout << "Student " << it->second->DajPokKodKogaJe()->DajImePrezime()
           << " (" << it->second->DajPokKodKogaJe()->DajIndeks()
           << ") zaduzio/la laptop broj " << it->first << endl;
    }
    cout << endl;
    it++;
  }
  if (usl == 0)
    cout << "Nema zaduzenja" << endl;
}

int main() {
  Administracija adm;
  while (1) {
    cout << "Odaberite jednu od  sljedecih opcija: \n1 - "
            "RegistrirajNovogStudenta\n2 - RegistrirajNoviLaptop\n3 - "
            "IzlistajStudente\n4 - IzlistajLaptope\n5 - NadjiSlobodniLaptop\n6 "
            "- ZaduziLaptop\n7 - RazduziLaptop\n8 - PrikaziZaduzenja\nK - Kraj "
            "programa\n";
    int n = 0;
    cin >> n;
    if (!cin)
      break;
    if (n == 1) {
      int brind = 0;
      cout << "Unesite broj indeksa: ";
      cin >> brind;
      cin.ignore(10000, '\n');
      string gdst;
      cout << "Unesite godinu studija (formata A/X, gdje je A godina studija, "
              "a X prima vrijednosti B,M,D, zavisno od studija): ";
      getline(cin, gdst);
      string ime;
      cout << "Unesite ime i prezime studenta: ";
      getline(cin, ime);
      string adr;
      cout << "Unesite adresu studenta: ";
      getline(cin, adr);
      string brtel;
      cout << "Unesite broj telefona (formata x/x-x): ";
      getline(cin, brtel);
      try {
        adm.RegistrirajNovogStudenta(brind, gdst, ime, adr, brtel);
        cout << "Student uspjesno registrovan!" << endl;
      } catch (domain_error e) {
        cout << "Izuzetak: " << e.what() << "!" << endl;
      }
    }
    if (n == 2) {
      int evbr = 0;
      cout << "Unesite evidencijski broj laptopa ";
      cin >> evbr;
      cin.ignore(10000, '\n');
      string naz;
      cout << "Unesite naziv laptopa ";
      getline(cin, naz);
      string kar;
      cout << "Unesite karakteristike laptopa: ";
      getline(cin, kar);
      try {
        adm.RegistrirajNoviLaptop(evbr, naz, kar);
        cout << "Laptop uspjesno registrovan!" << endl;
      } catch (domain_error e) {
        cout << "Izuzetak: " << e.what() << "!" << endl;
      }
    }
    if (n == 3) {
      adm.IzlistajStudente();
    }
    if (n == 4) {
      adm.IzlistajLaptope();
    }
    if (n == 5) {
      try {
        cout << adm.NadjiSlobodniLaptop() << endl;
      } catch (domain_error e) {
        cout << "Izuzetak: " << e.what() << "!" << endl;
      }
    }
    if (n == 6) {
      int ind = 0;
      cout << "Unesite indeks studenta: ";
      cin >> ind;
      int ev = 0;
      cout << "Unesite evidencijski broj laptopa: ";
      cin >> ev;
      try {
        adm.ZaduziLaptop(ind, ev);
      } catch (domain_error e) {
        cout << "Izuzetak: " << e.what() << "!" << endl;
      }
    }
    if (n == 7) {
      int ev = 0;
      cout << "Unesite evidencijski broj laptopa: ";
      cin >> ev;
      try {
        adm.RazduziLaptop(ev);
      } catch (domain_error e) {
        cout << "Izuzetak: " << e.what() << "!" << endl;
      }
    }
    if (n == 8) {
      adm.PrikaziZaduzenja();
    }
  }
  return 0;
}
