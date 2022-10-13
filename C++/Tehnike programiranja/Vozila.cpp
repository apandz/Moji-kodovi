#include <iostream>
#include <vector>
#include <fstream>
#include <algorithm>

using namespace std;

class ApstraktnoVozilo {
protected:
  int t;

public:
  ApstraktnoVozilo(int t1) : t(t1) {}
  virtual ~ApstraktnoVozilo(){}
  int DajTezinu() const { return t; }
  virtual int DajUkupnuTezinu() const = 0;
  virtual ApstraktnoVozilo *DajKopiju() const = 0;
  virtual void IspisiPodatke() const = 0;
};

class Automobil : public ApstraktnoVozilo {
  vector<int> tp;

public:
  Automobil(int t1, vector<int> tp1) : ApstraktnoVozilo(t1), tp(tp1) {}
  int DajUkupnuTezinu() const override {
    int ut = t;
    for (int i : tp) {
      ut += i;
    }
    return ut;
  }
  ApstraktnoVozilo *DajKopiju() const override { return new Automobil(t, tp); }
  void IspisiPodatke() const override {
    cout << "Vrsta vozila: Automobil" << endl
         << "Vlastita tezina: " << t << " kg" << endl;
    cout << "Tezine putnika: ";
    for (int i = 0; i < tp.size(); i++) {
      cout << tp.at(i) << " kg";
      if (i != tp.size() - 1)
        cout << ", ";
    }
    cout << endl << "Ukupna tezina: " << DajUkupnuTezinu() << " kg" << endl;
  }
};

class Kamion : public ApstraktnoVozilo {
  int tt;

public:
  Kamion(int t1, int tt1) : ApstraktnoVozilo(t1), tt(tt1) {}
  int DajUkupnuTezinu() const override { return t + tt; }
  ApstraktnoVozilo *DajKopiju() const override { return new Kamion(t, tt); }
  void IspisiPodatke() const override {
    cout << "Vrsta vozila: Kamion" << endl
         << "Vlastita tezina: " << t << " kg" << endl
         << "Tezina tereta: " << tt << " kg" << endl
         << "Ukupna tezina: " << t+tt << " kg" << endl;
  }
};

class Autobus : public ApstraktnoVozilo {
  int brp,prtp;

public:
  Autobus(int t1, int brp1, int prtp1) : ApstraktnoVozilo(t1), brp(brp1), prtp(prtp1) {}
  int DajUkupnuTezinu() const override { return t + brp*prtp; }
  ApstraktnoVozilo *DajKopiju() const override { return new Autobus(t, brp, prtp); }
  void IspisiPodatke() const override {
    cout << "Vrsta vozila: Autobus" << endl
         << "Vlastita tezina: " << t << " kg" << endl
         << "Broj putnika: "<<brp<<endl
         << "Prosjecna tezina putnika: " << prtp << " kg" << endl
         << "Ukupna tezina: " << t+brp*prtp << " kg" << endl;
  }
};

class Vozilo {
    ApstraktnoVozilo* v;
    void Test() const {if(!v) throw logic_error("Nespecificirano vozilo");}
    public:
    Vozilo(): v(nullptr){}
    ~Vozilo() {delete v;}
    Vozilo(const ApstraktnoVozilo& v1): v(v1.DajKopiju()){}
    Vozilo(const Vozilo& v1) {if(!v1.v){v=nullptr;} else {v=v1.v->DajKopiju();}}
    Vozilo(Vozilo&& v1){v=v1.v; v1.v=nullptr;}
    Vozilo& operator=(const Vozilo& v1) {delete v; if(!v1.v) {v=nullptr;} else {v=v1.v->DajKopiju();} return *this;}
    Vozilo& operator=(Vozilo&& v1){delete v; v=v1.v; v1.v=nullptr; return *this;}
    int DajTezinu() const { Test(); return v->DajTezinu(); }
    int DajUkupnuTezinu() const { Test(); return v->DajUkupnuTezinu();}
    ApstraktnoVozilo *DajKopiju() const {Test(); return v->DajKopiju();}
    void IspisiPodatke() const {Test(); v->IspisiPodatke();}
};

int main() { 
    ifstream voz("VOZILA.TXT");
    if(!voz) cout<<"Otvaranje datoteke nije uspjelo!\n";
    else {
        vector<Vozilo> v;
        for(;;) {
            char z;
            voz>>z;
            if(voz.peek()==EOF || z==EOF || voz.eof()) {break;}
            if(z=='A') {
                vector<int> tp;
                int t,brp;
                voz>>t>>brp;
                if(!voz) {cout<<"Problem pri citanju datoteke"; break;}
                for(int i=0; i<brp; i++) {
                    int tjp;
                    voz>>tjp;
                    tp.push_back(tjp);
                }
                if(!voz) {cout<<"Problem pri citanju datoteke"; break;}
                v.push_back(Vozilo(Automobil(t,tp)));
            } 
            else if(z=='B') {
                int t,brp,pr;
                voz>>t>>brp>>pr;
                if(!voz) {cout<<"Problem pri citanju datoteke"; break;}
                v.push_back(Vozilo(Autobus(t,brp,pr)));
            }
            else if(z=='K') {
                int t, tt;
                voz>>t>>tt;
                if(!voz) {cout<<"Problem pri citanju datoteke"; break;}
                v.push_back(Vozilo(Kamion(t,tt)));
            } else {cout<<"Problem pri citanju datoteke"; break;}
        }
        sort(v.begin(),v.end(),[](const Vozilo& v1, const Vozilo& v2) {return v1.DajUkupnuTezinu()<v2.DajUkupnuTezinu();});
        for_each(v.begin(),v.end(),[](const Vozilo& v1){cout<<v1.DajUkupnuTezinu()<<endl;});
    }
    return 0; 
    }
