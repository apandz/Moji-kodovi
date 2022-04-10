#include <algorithm>
#include <cmath>
#include <iostream>
#include <vector>

using namespace std;

class ApstraktniStudent {
protected:
  string ime, prez;
  int ind, brpol;
  double prs;

public:
  ApstraktniStudent(string ime1, string prez1, int ind1) : brpol{}, prs(5) {
    ime = ime1;
    prez = prez1;
    ind = ind1;
  }
  virtual ~ApstraktniStudent(){};
  string DajIme() const { return ime; }
  string DajPrezime() const { return prez; }
  int DajBrojIndeksa() const { return ind; }
  int DajBrojPolozenih() const { return brpol; }
  double DajProsjek() const { return prs; }
  void RegistrirajIspit(int ocj) {
    if (ocj == 5) {
      return;
    }
    if (ocj < 5 || ocj > 10) {
      throw domain_error("Neispravna ocjena");
    } else {
      prs *= brpol;
      brpol++;
      prs = (prs + ocj) / brpol;
    }
  }
  void PonistiOcjene() {
    brpol = 0;
    prs = 5;
  }
  virtual void IspisiPodatke() const = 0;
  virtual ApstraktniStudent *DajKopiju() const = 0;
};

class StudentBachelor : public ApstraktniStudent {
public:
  StudentBachelor(string ime1, string prez1, int ind1)
      : ApstraktniStudent(ime1, prez1, ind1) {}
  void IspisiPodatke() const override {
    cout << "Student bachelor studija " << ime << " " << prez
         << ", sa brojem indeksa " << ind << ",\nima prosjek " << prs << "."
         << endl;
  }
  ApstraktniStudent *DajKopiju() const override {
    return new StudentBachelor(*this);
  }
};

class StudentMaster : public ApstraktniStudent {
  int god;

public:
  StudentMaster(string ime1, string prez1, int ind1, int god1)
      : ApstraktniStudent(ime1, prez1, ind1), god(god1) {}
  void IspisiPodatke() const override {
    cout << "Student master studija " << ime << " " << prez
         << ", sa brojem indeksa " << ind
         << ",\nzavrsio bachelor studij godine " << god << ",ima prosjek "
         << prs << "." << endl;
  }
  ApstraktniStudent *DajKopiju() const override {
    return new StudentMaster(*this);
  }
};

class Fakultet {
  vector<ApstraktniStudent *> v;

public:
  Fakultet() {}
  ~Fakultet() {
    for (int i = 0; i < v.size(); i++) {
      if (v.at(i) != nullptr) {
        delete v.at(i);
      }
    }
  }
  Fakultet(const Fakultet &f) {
    v.resize(f.v.size());
    try {
      for (int i = 0; i < f.v.size(); i++) {
        v.at(i) = f.v.at(i)->DajKopiju();
      }
    } catch (...) {
      for (int i = 0; i < v.size(); i++) {
        if (v.at(i) != nullptr)
          delete v.at(i);
      }
      throw;
    }
  }
  Fakultet(Fakultet &&f) {
    v.resize(f.v.size());
    for (int i = 0; i < f.v.size(); i++) {
      v.at(i) = f.v.at(i);
    }
    f.v.resize(0);
  }
  Fakultet &operator=(const Fakultet &f) {
    for (int i = 0; i < v.size(); i++) {
      delete v.at(i);
    }
    v.resize(f.v.size());
    try {
      for (int i = 0; i < f.v.size(); i++) {
        v.at(i) = f.v.at(i)->DajKopiju();
      }
    } catch (...) {
      for (int i = 0; i < v.size(); i++) {
        if (v.at(i) != nullptr)
          delete v.at(i);
      }
      throw;
    }
    return *this;
  }
  Fakultet &operator=(Fakultet &&f) {
    for (int i = 0; i < v.size(); i++) {
      delete v.at(i);
    }
    v.resize(f.v.size());
    for (int i = 0; i < f.v.size(); i++) {
      v.at(i) = f.v.at(i);
    }
    f.v.resize(0);
    return *this;
  }
  void UpisiStudenta(string ime1, string prez1, int ind1) {
    for (int i = 0; i < v.size(); i++) {
      if (v.at(i)->DajBrojIndeksa() == ind1) {
        throw logic_error("Student sa zadanim brojem indeksa vec postoji");
      }
    }
    v.push_back(new StudentBachelor(ime1, prez1, ind1));
  }
  void UpisiStudenta(string ime1, string prez1, int ind1, int god1) {
    for (int i = 0; i < v.size(); i++) {
      if (v.at(i)->DajBrojIndeksa() == ind1) {
        throw logic_error("Student sa zadanim brojem indeksa vec postoji");
      }
    }
    v.push_back(new StudentMaster(ime1, prez1, ind1, god1));
  }
  void ObrisiStudenta(int ind1) {
    auto it = v.begin();
    for (int i = 0; i < v.size(); i++) {
      if (v.at(i)->DajBrojIndeksa() == ind1) {
        delete v.at(i);
        break;
      }
      it++;
    }
    if (it == v.end()) {
      throw logic_error("Student sa zadanim brojem indeksa ne postoji");
    } else {
      v.erase(it);
    }
  }
  ApstraktniStudent &operator[](int ind1) {
    auto it = v.begin();
    for (int i = 0; i < v.size(); i++) {
      if (v.at(i)->DajBrojIndeksa() == ind1) {
        break;
      }
      it++;
    }
    if (it == v.end()) {
      throw logic_error("Student sa zadanim brojem indeksa ne postoji");
    }
    return *(v.at(it - v.begin()));
  }
  const ApstraktniStudent &operator[](int ind1) const {
    auto it = v.begin();
    for (int i = 0; i < v.size(); i++) {
      if (v.at(i)->DajBrojIndeksa() == ind1) {
        break;
      }
      it++;
    }
    if (it == v.end()) {
      throw logic_error("Student sa zadanim brojem indeksa ne postoji");
    }
    return *(v.at(it - v.begin()));
  }
  void IspisiSveStudente() const {
    auto f = *this;
    sort(f.v.begin(), f.v.end(),
         [](ApstraktniStudent *s1, ApstraktniStudent *s2) {
           const double eps = 1e-10;
           if (fabs(s1->DajProsjek() - s2->DajProsjek()) <=
               eps * (fabs(s1->DajProsjek()) + fabs(s2->DajProsjek()))) {
             return s1->DajBrojIndeksa() < s2->DajBrojIndeksa();
           }
           return !(s1->DajProsjek() - s2->DajProsjek() <
                    eps * (s1->DajProsjek() + s2->DajProsjek()));
         });
    for (int i = 0; i < f.v.size(); i++) {
      f.v.at(i)->IspisiPodatke();
    }
  }
};

int main() {
  Fakultet f1;
  f1.UpisiStudenta("Selena", "Gomez", 5521);
  f1.UpisiStudenta("Amy", "Beckett", 5122, 2016);
  f1.UpisiStudenta("Chris", "Evans", 5550);
  f1[5521].RegistrirajIspit(7);
  f1[5521].RegistrirajIspit(9);
  f1[5122].RegistrirajIspit(9);
  f1[5122].RegistrirajIspit(10);
  f1[5550].RegistrirajIspit(8);
  f1.IspisiSveStudente();
  f1.ObrisiStudenta(5521);
  f1.IspisiSveStudente();
  return 0;
}
