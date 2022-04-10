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
  if(niz){delete[] niz;}
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
  niz=nullptr;
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

template <typename tipk, typename tipv>
void ispisi(const NizMapa<tipk, tipv> &nm, vector<tipk> kljucevi) {
  for (int i = 0; i < kljucevi.size(); i++) {
    cout << kljucevi.at(i) << " " << nm[kljucevi.at(i)] << endl;
  }
}

template <typename tipk, typename tipv>
void ispisi_vector(vector<tipk> v1, vector<tipv> v2) {
  for (int i = 0; i < v1.size(); i++) {
    cout << v1.at(i) << " " << v2.at(i) << endl;
  }
}

void test_kopirajuci_konstruktor() {
  NizMapa<string, int> gradovi;
  vector<string> grad({"Tokio", "New York", "Seoul", "London"});
  vector<int> brst({37730064, 23019036, 22692652, 13377482});
  for (int i = 0; i < grad.size(); i++) {
    gradovi[grad.at(i)] = brst.at(i);
  }
  NizMapa<string, int> gradovi1(gradovi);
  cout << "Test kopirajući konstruktor:\n   Očekivani output:\n";
  ispisi_vector(grad, brst);
  cout << "    Stvarni output:\n";
  ispisi(gradovi1, grad);
}

void test_pomjerajuci_konstruktor() {
    NizMapa<string, int> gradovi;
  vector<string> grad({"Tokio", "New York", "Seoul", "London"});
  vector<int> brst({37730064, 23019036, 22692652, 13377482});
  for (int i = 0; i < grad.size(); i++) {
    gradovi[grad.at(i)] = brst.at(i);
  }
  NizMapa<string, int> gradovi1(move(gradovi));
  cout << "Test pomjerajući konstruktor:\n   Očekivani output:\n";
  ispisi_vector(grad, brst);
  cout << "    Stvarni output:\n";
  ispisi(gradovi1, grad);
}

void test_kopirajuci_operator_dodjele() {
    NizMapa<string, int> gradovi;
  vector<string> grad({"Tokio", "New York", "Seoul", "London"});
  vector<int> brst({37730064, 23019036, 22692652, 13377482});
  for (int i = 0; i < grad.size(); i++) {
    gradovi[grad.at(i)] = brst.at(i);
  }
  NizMapa<string, int> gradovi1=gradovi;
  cout << "Test kopirajući operator dodjele:\n  Očekivani output:\n";
  ispisi_vector(grad, brst);
  cout << "    Stvarni output:\n";
  ispisi(gradovi1, grad);
}

void test_pomjerajuci_operator_dodjele() {
    NizMapa<string, int> gradovi;
  vector<string> grad({"Tokio", "New York", "Seoul", "London"});
  vector<int> brst({37730064, 23019036, 22692652, 13377482});
  for (int i = 0; i < grad.size(); i++) {
    gradovi[grad.at(i)] = brst.at(i);
  }
  NizMapa<string, int> gradovi1=move(gradovi);
  cout << "Test pomjerajući operator dodjele:\n   Očekivani output:\n";
  ispisi_vector(grad, brst);
  cout << "    Stvarni output:\n";
  ispisi(gradovi1, grad);
}

void test_operator_uglaste() {
    NizMapa<string, int> gradovi;
    vector<string> grad({"Tokio", "New York", "Seoul", "London"});
  vector<int> brst({37730064, 23019036, 22692652, 13377482});
    for (int i = 0; i < grad.size(); i++) {
    gradovi[grad.at(i)] = brst.at(i);
  }
  gradovi["Tokio"]=0;
  brst.at(0)=0;
  gradovi["Sarajevo"]=1000000;
  grad.push_back("Sarajevo");
  brst.push_back(1000000);
  cout << "Test operator[]:\n   Očekivani output:\n";
  ispisi_vector(grad, brst);
  cout << "    Stvarni output:\n";
  ispisi(gradovi, grad);
}

void test_operator_uglaste_const() {
    NizMapa<string, int> gradovi;
    vector<string> grad({"Tokio", "New York", "Seoul", "London"});
  vector<int> brst({37730064, 23019036, 22692652, 13377482});
    for (int i = 0; i < grad.size(); i++) {
    gradovi[grad.at(i)] = brst.at(i);
  }
  gradovi["Tokio"]=0;
  brst.at(0)=0;
  gradovi["Sarajevo"]=1000000;
  grad.push_back("Sarajevo");
  brst.push_back(1000000);
  const NizMapa<string,int> gradovi1(gradovi);
  gradovi["Banja Luka"]=500000;
  cout << "Test operator[] const:\n Očekivani output:\n";
  ispisi_vector(grad, brst);
  cout << "    Stvarni output:\n";
  ispisi(gradovi1, grad);
}

void test_broj_elemenata() {
    NizMapa<string, int> gradovi;
    vector<string> grad({"Tokio", "New York", "Seoul", "London"});
  vector<int> brst({37730064, 23019036, 22692652, 13377482});
    for (int i = 0; i < grad.size(); i++) {
    gradovi[grad.at(i)] = brst.at(i);
  }
  gradovi["Sarajevo"]=1000000;
  grad.push_back("Sarajevo");
  brst.push_back(1000000);
  cout << "Test broj elemenata:\n   Očekivani output:\n"<<grad.size()<<"\n";
  cout << "    Stvarni output:\n"<<gradovi.brojElemenata()<<"\n";
}

void test_obrisi_sve() {
    NizMapa<string, int> gradovi;
    vector<string> grad({"Tokio", "New York", "Seoul", "London"});
  vector<int> brst({37730064, 23019036, 22692652, 13377482});
    for (int i = 0; i < grad.size(); i++) {
    gradovi[grad.at(i)] = brst.at(i);
  }
  gradovi.obrisi();
  cout << "Test obrisi sve:\n   Očekivani output:\n0\n";
  cout << "    Stvarni output:\n"<<gradovi.brojElemenata()<<"\n";
}

void test_obrisi() {
    NizMapa<string, int> gradovi;
    vector<string> grad({"Tokio", "New York", "Seoul", "London"});
  vector<int> brst({37730064, 23019036, 22692652, 13377482});
    for (int i = 0; i < grad.size(); i++) {
    gradovi[grad.at(i)] = brst.at(i);
  }
  gradovi["Sarajevo"]=1000000;
  grad.push_back("Sarajevo");
  brst.push_back(1000000);
  gradovi.obrisi("New York");
  grad.erase(grad.begin()+1);
  brst.erase(brst.begin()+1);
  cout << "Test obrisi:\n   Očekivani output:\n";
  ispisi_vector(grad, brst);
  cout << "    Stvarni output:\n";
  ispisi(gradovi, grad);
}

void test_kopirajuci_konstruktor1() {
  NizMapa<int,string> pozbr;
  vector<string> kantoni({"Srednjobosanski kanton", "Posavski kanton", "Zeničko-dobojski kanton", "Kanton Sarajevo","Kanton 10"});
  vector<int> br({30, 31, 32,33,34});
  for (int i = 0; i < kantoni.size(); i++) {
    pozbr[br.at(i)] = kantoni.at(i);
  }
  NizMapa<int,string> pozbr1(pozbr);
  cout << "Test kopirajući konstruktor:\n   Očekivani output:\n";
  ispisi_vector(br, kantoni);
  cout << "    Stvarni output:\n";
  ispisi(pozbr, br);
}

void test_pomjerajuci_konstruktor1() {
    NizMapa<int,string> pozbr;
  vector<string> kantoni({"Srednjobosanski kanton", "Posavski kanton", "Zeničko-dobojski kanton", "Kanton Sarajevo","Kanton 10"});
  vector<int> br({30, 31, 32,33,34});
  for (int i = 0; i < kantoni.size(); i++) {
    pozbr[br.at(i)] = kantoni.at(i);
  }
  NizMapa<int,string> pozbr1(move(pozbr));
  cout << "Test pomjerajući konstruktor:\n   Očekivani output:\n";
  ispisi_vector(br,kantoni);
  cout << "    Stvarni output:\n";
  ispisi(pozbr1, br);
}

void test_kopirajuci_operator_dodjele1() {
    NizMapa<int,string> pozbr;
  vector<string> kantoni({"Srednjobosanski kanton", "Posavski kanton", "Zeničko-dobojski kanton", "Kanton Sarajevo","Kanton 10"});
  vector<int> br({30, 31, 32,33,34});
  for (int i = 0; i < kantoni.size(); i++) {
    pozbr[br.at(i)] = kantoni.at(i);
  }
  NizMapa<int,string> pozbr1=pozbr;
  cout << "Test kopirajući operator dodjele:\n  Očekivani output:\n";
  ispisi_vector(br,kantoni);
  cout << "    Stvarni output:\n";
  ispisi(pozbr1, br);
}

void test_pomjerajuci_operator_dodjele1() {
    NizMapa<int,string> pozbr;
  vector<string> kantoni({"Srednjobosanski kanton", "Posavski kanton", "Zeničko-dobojski kanton", "Kanton Sarajevo","Kanton 10"});
  vector<int> br({30, 31, 32,33,34});
  for (int i = 0; i < kantoni.size(); i++) {
    pozbr[br.at(i)] = kantoni.at(i);
  }
  NizMapa<int,string> pozbr1=move(pozbr);
  cout << "Test pomjerajući operator dodjele:\n   Očekivani output:\n";
  ispisi_vector(br,kantoni);
  cout << "    Stvarni output:\n";
  ispisi(pozbr1, br);
}

void test_operator_uglaste1() {
    NizMapa<int,string> pozbr;
    vector<string> kantoni({"Srednjobosanski kanton", "Posavski kanton", "Zeničko-dobojski kanton", "Kanton Sarajevo","Kanton 10"});
  vector<int> br({30, 31, 32,33,34});
    for (int i = 0; i < kantoni.size(); i++) {
    pozbr[br.at(i)] = kantoni.at(i);
  }
  pozbr[31]="Orašje";
  kantoni.at(1)="Orašje";
  pozbr[35]="Tuzlanski kanton";
  kantoni.push_back("Tuzlanski kanton");
  br.push_back(35);
  cout << "Test operator[]:\n   Očekivani output:\n";
  ispisi_vector(br,kantoni);
  cout << "    Stvarni output:\n";
  ispisi(pozbr, br);
}

void test_operator_uglaste_const1() {
    NizMapa<int,string> pozbr;
    vector<string> kantoni({"Srednjobosanski kanton", "Posavski kanton", "Zeničko-dobojski kanton", "Kanton Sarajevo","Kanton 10"});
  vector<int> br({30, 31, 32,33,34});
    for (int i = 0; i < kantoni.size(); i++) {
    pozbr[br.at(i)] = kantoni.at(i);
  }
  pozbr[31]="Orašje";
  kantoni.at(1)="Orašje";
  pozbr[35]="Tuzlanski kanton";
  kantoni.push_back("Tuzlanski kanton");
  br.push_back(35);
  const NizMapa<int,string> pozbr1(pozbr);
  pozbr[36]="Hercegovačko-neretvanski kanton";
  cout << "Test operator[] const:\n Očekivani output:\n";
  ispisi_vector(br,kantoni);
  cout << "    Stvarni output:\n";
  ispisi(pozbr1, br);
}

void test_broj_elemenata1() {
    NizMapa<int,string> pozbr;
    vector<string> kantoni({"Srednjobosanski kanton", "Posavski kanton", "Zeničko-dobojski kanton", "Kanton Sarajevo","Kanton 10"});
  vector<int> br({30, 31, 32,33,34});
    for (int i = 0; i < kantoni.size(); i++) {
    pozbr[br.at(i)] = kantoni.at(i);
  }
  pozbr[35]="Tuzlanski kanton";
  kantoni.push_back("Tuzlanski kanton");
  br.push_back(35);
  cout << "Test broj elemenata:\n   Očekivani output:\n"<<kantoni.size()<<"\n";
  cout << "    Stvarni output:\n"<<pozbr.brojElemenata()<<"\n";
}

void test_obrisi_sve1() {
    NizMapa<int,string> pozbr;
    vector<string> kantoni({"Srednjobosanski kanton", "Posavski kanton", "Zeničko-dobojski kanton", "Kanton Sarajevo","Kanton 10"});
  vector<int> br({30, 31, 32,33,34});
    for (int i = 0; i < kantoni.size(); i++) {
    pozbr[br.at(i)] = kantoni.at(i);
  }
  pozbr.obrisi();
  cout << "Test obrisi sve:\n   Očekivani output:\n0\n";
  cout << "    Stvarni output:\n"<<pozbr.brojElemenata()<<"\n";
}

void test_obrisi1() {
    NizMapa<int,string> pozbr;
    vector<string> kantoni({"Srednjobosanski kanton", "Posavski kanton", "Zeničko-dobojski kanton", "Kanton Sarajevo","Kanton 10"});
  vector<int> br({30, 31, 32,33,34});
    for (int i = 0; i < kantoni.size(); i++) {
    pozbr[br.at(i)] = kantoni.at(i);
  }
  pozbr[35]="Tuzlanski kanton";
  kantoni.push_back("Tuzlanski kanton");
  br.push_back(35);
  pozbr.obrisi(32);
  kantoni.erase(kantoni.begin()+2);
  br.erase(br.begin()+2);
  cout << "Test obrisi:\n   Očekivani output:\n";
  ispisi_vector(br,kantoni);
  cout << "    Stvarni output:\n";
  ispisi(pozbr, br);
}

void test_kopirajuci_konstruktor2() {
  NizMapa<int, double> br;
  vector<double> br2;
  vector<int> br1({3, 4, 7, 12});
  for (int i = 0; i < br1.size(); i++) {
      br2.push_back(br1.at(i)*br1.at(i)/3.);
    br[br1.at(i)] = br2.at(i);
  }
  NizMapa<int, double> brr(br);
  cout << "Test kopirajući konstruktor:\n   Očekivani output:\n";
  ispisi_vector(br1,br2);
  cout << "    Stvarni output:\n";
  ispisi(brr, br1);
}

void test_pomjerajuci_konstruktor2() {
    NizMapa<int, double> br;
  vector<double> br2;
  vector<int> br1({3, 4, 7, 12});
  for (int i = 0; i < br1.size(); i++) {
      br2.push_back(br1.at(i)*br1.at(i)/3.);
    br[br1.at(i)] = br2.at(i);
  }
  NizMapa<int, double> brr(move(br));
  cout << "Test pomjerajući konstruktor:\n   Očekivani output:\n";
  ispisi_vector(br1,br2);
  cout << "    Stvarni output:\n";
  ispisi(brr, br1);
}

void test_kopirajuci_operator_dodjele2() {
    NizMapa<int, double> br;
  vector<double> br2;
  vector<int> br1({3, 4, 7, 12});
  for (int i = 0; i < br1.size(); i++) {
      br2.push_back(br1.at(i)*br1.at(i)/3.);
    br[br1.at(i)] = br2.at(i);
  }
  NizMapa<int, double> brr=br;
  cout << "Test kopirajući operator dodjele:\n  Očekivani output:\n";
  ispisi_vector(br1,br2);
  cout << "    Stvarni output:\n";
  ispisi(brr, br1);
}

void test_pomjerajuci_operator_dodjele2() {
    NizMapa<int, double> br;
  vector<double> br2;
  vector<int> br1({3, 4, 7, 12});
  for (int i = 0; i < br1.size(); i++) {
      br2.push_back(br1.at(i)*br1.at(i)/3.);
    br[br1.at(i)] = br2.at(i);
  }
  NizMapa<int, double> brr=move(br);
  cout << "Test pomjerajući operator dodjele:\n   Očekivani output:\n";
  ispisi_vector(br1,br2);
  cout << "    Stvarni output:\n";
  ispisi(brr, br1);
}

void test_operator_uglaste2() {
    NizMapa<int, double> br;
  vector<double> br2;
  vector<int> br1({3, 4, 7, 12});
  for (int i = 0; i < br1.size(); i++) {
      br2.push_back(br1.at(i)*br1.at(i)/3.);
    br[br1.at(i)] = br2.at(i);
  }
  br[4]=br[4]+2;
  br2.at(1)=br[4];
  br[15]=15*15/3.;
  br1.push_back(15);
  br2.push_back(15*15/3.);
  cout << "Test operator[]:\n   Očekivani output:\n";
  ispisi_vector(br1,br2);
  cout << "    Stvarni output:\n";
  ispisi(br, br1);
}

void test_operator_uglaste_const2() {
    NizMapa<int, double> br;
  vector<double> br2;
  vector<int> br1({3, 4, 7, 12});
  for (int i = 0; i < br1.size(); i++) {
      br2.push_back(br1.at(i)*br1.at(i)/3.);
    br[br1.at(i)] = br2.at(i);
  }
  const NizMapa<int,double> brr(br);
  br[15]=15*15/3.;
  cout << "Test operator[] const:\n Očekivani output:\n";
  ispisi_vector(br1,br2);
  cout << "    Stvarni output:\n";
  ispisi(brr, br1);
}

void test_broj_elemenata2() {
    NizMapa<int, double> br;
  vector<double> br2;
  vector<int> br1({3, 4, 7, 12});
  for (int i = 0; i < br1.size(); i++) {
      br2.push_back(br1.at(i)*br1.at(i)/3.);
    br[br1.at(i)] = br2.at(i);
  }
  br[15]=15*15/3.;
  br1.push_back(15);
  br2.push_back(15*15/3.);
  cout << "Test broj elemenata:\n   Očekivani output:\n"<<br1.size()<<"\n";
  cout << "    Stvarni output:\n"<<br.brojElemenata()<<"\n";
}

void test_obrisi_sve2() {
    NizMapa<int, double> br;
  vector<double> br2;
  vector<int> br1({3, 4, 7, 12});
  for (int i = 0; i < br1.size(); i++) {
      br2.push_back(br1.at(i)*br1.at(i)/3.);
    br[br1.at(i)] = br2.at(i);
  }
  br.obrisi();
  cout << "Test obrisi sve:\n   Očekivani output:\n0\n";
  cout << "    Stvarni output:\n"<<br.brojElemenata()<<"\n";
}

void test_obrisi2() {
   NizMapa<int, double> br;
  vector<double> br2;
  vector<int> br1({3, 4, 7, 12});
  for (int i = 0; i < br1.size(); i++) {
      br2.push_back(br1.at(i)*br1.at(i)/3.);
    br[br1.at(i)] = br2.at(i);
  }
  br[15]=15*15/3.;
  br1.push_back(15);
  br2.push_back(15*15/3.);
  br.obrisi(7);
  br1.erase(br1.begin()+2);
  br2.erase(br2.begin()+2);
  cout << "Test obrisi:\n   Očekivani output:\n";
  ispisi_vector(br1,br2);
  cout << "    Stvarni output:\n";
  ispisi(br, br1);
}

int main() {
    cout<<"Testovi za NizMapa<string,int>:"<<endl<<"-------------------------\n";
  test_kopirajuci_konstruktor();
  cout<<endl;
  test_pomjerajuci_konstruktor();
  cout<<endl;
  test_kopirajuci_operator_dodjele();
  cout<<endl;
  test_pomjerajuci_operator_dodjele();
  cout<<endl;
  test_operator_uglaste();
  cout<<endl;
  test_operator_uglaste_const();
  cout<<endl;
  test_broj_elemenata();
  cout<<endl;
  test_obrisi_sve();
  cout<<endl;
  test_obrisi();
  cout<<endl<<"Testovi za NizMapa<int,string>:"<<endl<<"-------------------------\n";
  test_kopirajuci_konstruktor1();
  cout<<endl;
  test_pomjerajuci_konstruktor1();
  cout<<endl;
  test_kopirajuci_operator_dodjele1();
  cout<<endl;
  test_pomjerajuci_operator_dodjele1();
  cout<<endl;
  test_operator_uglaste1();
  cout<<endl;
  test_operator_uglaste_const1();
  cout<<endl;
  test_broj_elemenata1();
  cout<<endl;
  test_obrisi_sve1();
  cout<<endl;
  test_obrisi1();
  cout<<endl<<"Testovi za NizMapa<int,double>:"<<endl<<"-------------------------\n";
  test_kopirajuci_konstruktor2();
  cout<<endl;
  test_pomjerajuci_konstruktor2();
  cout<<endl;
  test_kopirajuci_operator_dodjele2();
  cout<<endl;
  test_pomjerajuci_operator_dodjele2();
  cout<<endl;
  test_operator_uglaste2();
  cout<<endl;
  test_operator_uglaste_const2();
  cout<<endl;
  test_broj_elemenata2();
  cout<<endl;
  test_obrisi_sve2();
  cout<<endl;
  test_obrisi2();
  return 0;
}
