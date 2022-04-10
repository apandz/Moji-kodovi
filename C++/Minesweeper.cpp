#include <iostream>
#include <vector>

using namespace std;

enum class Polje {
  Prazno,
  Posjeceno,
  Mina,
  BlokiranoPrazno,
  BlokiranoPosjeceno,
  BlokiranoMina
};

typedef vector<vector<Polje>> Tabla;

Tabla KreirajIgru(int n, const vector<vector<int>> &mine) {
  Tabla t(n);
  if (n <= 0)
    throw domain_error("Ilegalna velicina");
  for (int i = 0; i < n; i++) {
    t.at(i).resize(n);
    for (int j = 0; j < n; j++) {
      t.at(i).at(j) = Polje::Prazno;
      for (int k = 0; k < mine.size(); k++) {
        if (mine.at(k).size() != 2)
          throw domain_error("Ilegalan format zadavanja mina");
        if (mine.at(k).at(0) >= n || mine.at(k).at(0) < 0 ||
            mine.at(k).at(1) < 0 || mine.at(k).at(1) >= n)
          throw domain_error("Ilegalne pozicije mina");
        if (mine.at(k).at(0) == i && mine.at(k).at(1) == j) {
          t.at(i).at(j) = Polje::Mina;
          break;
        }
      }
    }
  }
  return t;
}

vector<vector<int>> PrikaziOkolinu(const Tabla &polja, int x, int y) {
  vector<vector<int>> m(3);
  m.at(0).resize(3);
  m.at(1).resize(3);
  m.at(2).resize(3);
  if (x < 0 || x >= polja.size() || y < 0 || y >= polja.size()) {
    cout << "Polje (" << x << "," << y << ") ne postoji";
    throw domain_error("");
  }
  int m1 = 0, m2 = 0;
  int br = 0;
  for (int i = x - 1; i <= x + 1; i++) {
    m2 = 0;
    for (int j = y - 1; j <= y + 1; j++) {
      br = 0;
      for (int k = i - 1; k <= i + 1; k++) {
        for (int l = j - 1; l <= j + 1; l++) {
          if (k == i && l == j) {
            continue;
          }
          if (k > -1 && l > -1 && k < polja.size() && l < polja.size()) {
            if (polja.at(k).at(l) == Polje::Mina)
              br++;
          }
        }
      }
      m.at(m1).at(m2) = br;
      m2++;
    }
    m1++;
  }
  return m;
}

void BlokirajPolje(Tabla &polja, int x, int y) {
  if (x < 0 || x >= polja.size() || y < 0 || y >= polja.size()) {
    cout << "Polje (" << x << "," << y << ") ne postoji";
    throw domain_error("");
  }
  if (polja.at(x).at(y) == Polje::Prazno)
    polja.at(x).at(y) = Polje::BlokiranoPrazno;
  if (polja.at(x).at(y) == Polje::Posjeceno)
    polja.at(x).at(y) = Polje::BlokiranoPosjeceno;
  if (polja.at(x).at(y) == Polje::Mina)
    polja.at(x).at(y) = Polje::BlokiranoMina;
}

void DeblokirajPolje(Tabla &polja, int x, int y) {
  if (x < 0 || x >= polja.size() || y < 0 || y >= polja.size()) {
    cout << "Polje (" << x << "," << y << ") ne postoji";
    throw domain_error("");
  }
  if (polja.at(x).at(y) == Polje::BlokiranoPrazno)
    polja.at(x).at(y) = Polje::Prazno;
  if (polja.at(x).at(y) == Polje::BlokiranoPosjeceno)
    polja.at(x).at(y) = Polje::Posjeceno;
  if (polja.at(x).at(y) == Polje::BlokiranoMina)
    polja.at(x).at(y) = Polje::Mina;
}

enum class Smjerovi {
  GoreLijevo,
  Gore,
  GoreDesno,
  Desno,
  DoljeDesno,
  Dolje,
  DoljeLijevo,
  Lijevo
};

enum class Status { NijeKraj, KrajPoraz, KrajPobjeda };

Status Idi(Tabla &polja, int &x, int &y, Smjerovi smjer) {
  int a = 0, b = 0;
  if (smjer == Smjerovi::GoreLijevo) {
    a = x - 1;
    b = y - 1;
  }
  if (smjer == Smjerovi::Gore) {
    a = x - 1;
    b = y;
  }
  if (smjer == Smjerovi::GoreDesno) {
    a = x - 1;
    b = y + 1;
  }
  if (smjer == Smjerovi::Desno) {
    a = x;
    b = y + 1;
  }
  if (smjer == Smjerovi::DoljeDesno) {
    a = x + 1;
    b = y + 1;
  }
  if (smjer == Smjerovi::Dolje) {
    a = x + 1;
    b = y;
  }
  if (smjer == Smjerovi::DoljeLijevo) {
    a = x + 1;
    b = y - 1;
  }
  if (smjer == Smjerovi::Lijevo) {
    a = x;
    b = y - 1;
  }
  if (a < 0 || a >= polja.size() || b < 0 || b >= polja.size())
    throw out_of_range("Izlazak van igrace table");
  if (polja.at(a).at(b) == Polje::BlokiranoPosjeceno ||
      polja.at(a).at(b) == Polje::BlokiranoPrazno ||
      polja.at(a).at(b) == Polje::BlokiranoMina)
    throw logic_error("Blokirano polje");
  polja.at(x).at(y) = Polje::Posjeceno;
  x = a;
  y = b;
  if (polja.at(x).at(y) == Polje::Mina) {
    for (int i = 0; i < polja.size(); i++) {
      for (int j = 0; j < polja.size(); j++) {
        polja.at(i).at(j) = Polje::Prazno;
      }
    }
    return Status::KrajPoraz;
  }
  int usl = 1;
  for (int i = 0; i < polja.size(); i++) {
    for (int j = 0; j < polja.size(); j++) {
      if (i == x && j == y)
        continue;
      if (polja.at(i).at(j) == Polje::Prazno)
        usl = 0;
    }
  }
  if (usl == 1)
    return Status::KrajPobjeda;
  return Status::NijeKraj;
}

Status Idi(Tabla &polja, int &x, int &y, int novi_x, int novi_y) {
  if (novi_x < 0 || novi_x >= polja.size() || novi_y < 0 ||
      novi_y >= polja.size())
    throw out_of_range("Izlazak van igrace table");
  if (polja.at(novi_x).at(novi_y) == Polje::BlokiranoPosjeceno ||
      polja.at(novi_x).at(novi_y) == Polje::BlokiranoPrazno ||
      polja.at(novi_x).at(novi_y) == Polje::BlokiranoMina)
    throw logic_error("Blokirano polje");
  polja.at(x).at(y) = Polje::Posjeceno;
  x = novi_x;
  y = novi_y;
  if (polja.at(x).at(y) == Polje::Mina) {
    for (int i = 0; i < polja.size(); i++) {
      for (int j = 0; j < polja.size(); j++) {
        polja.at(i).at(j) = Polje::Prazno;
      }
    }
    return Status::KrajPoraz;
  }
  int usl = 1;
  for (int i = 0; i < polja.size(); i++) {
    for (int j = 0; j < polja.size(); j++) {
      if (i == x && j == y)
        continue;
      if (polja.at(i).at(j) == Polje::Prazno)
        usl = 0;
    }
  }
  if (usl == 1)
    return Status::KrajPobjeda;
  return Status::NijeKraj;
}

enum class KodoviGresaka {
  PogresnaKomanda,
  NedostajeParametar,
  SuvisanParametar,
  NeispravanParametar
};

void PrijaviGresku(KodoviGresaka k) {
  if (k == KodoviGresaka::PogresnaKomanda)
    cout << "Nerazumljiva komanda!" << endl;
  if (k == KodoviGresaka::NedostajeParametar)
    cout << "Komanda trazi parametar koji nije naveden!" << endl;
  if (k == KodoviGresaka::SuvisanParametar)
    cout << "Zadan je suvisan parametar nakon komande!" << endl;
  if (k == KodoviGresaka::NeispravanParametar)
    cout << "Parametar komande nije ispravan!" << endl;
}

enum class Komande {
  PomjeriJednoMjesto,
  PomjeriDalje,
  Blokiraj,
  Deblokiraj,
  PrikaziOkolinu,
  ZavrsiIgru,
  KreirajIgru
};

bool UnosKomande(Komande &komanda, Smjerovi &smjer, int &x, int &y,
                 KodoviGresaka &greska) {
  string s;
  getline(cin, s);
  int kg = 0;
  int par1 = -1, par2 = -1, usl = 0;
  if (s.find("P1") != -1) {
    if (s.find("GL") != -1) {
      smjer = Smjerovi::GoreLijevo;
      for (int i = 0; i < s.length(); i++) {
        if (s.at(i) != 'P' && s.at(i) != '1' && s.at(i) != ' ' &&
            s.at(i) != 'G' && s.at(i) != 'L')
          kg = -3;
      }
    } else if (s.find("G") != -1 && s.find("D") == -1 && s.find("L") == -1) {
      smjer = Smjerovi::Gore;
      for (int i = 0; i < s.length(); i++) {
        if (s.at(i) != 'P' && s.at(i) != '1' && s.at(i) != ' ' &&
            s.at(i) != 'G')
          kg = -3;
      }
    } else if (s.find("GD") != -1) {
      smjer = Smjerovi::GoreDesno;
      for (int i = 0; i < s.length(); i++) {
        if (s.at(i) != 'P' && s.at(i) != '1' && s.at(i) != ' ' &&
            s.at(i) != 'G' && s.at(i) != 'D')
          kg = -3;
      }
    } else if (s.find("D") != -1 && s.find("G") == -1 && s.find("o") == -1 &&
               s.find("oD") == -1 && s.find("oL") == -1) {
      smjer = Smjerovi::Desno;
      for (int i = 0; i < s.length(); i++) {
        if (s.at(i) != 'P' && s.at(i) != '1' && s.at(i) != ' ' &&
            s.at(i) != 'D')
          kg = -3;
      }
    } else if (s.find("DoD") != -1) {
      smjer = Smjerovi::DoljeDesno;
      for (int i = 0; i < s.length(); i++) {
        if (s.at(i) != 'P' && s.at(i) != '1' && s.at(i) != ' ' &&
            s.at(i) != 'D' && s.at(i) != 'o')
          kg = -3;
      }
    } else if (s.find("Do") != -1 && s.find("oD") == -1 && s.find("oL") == -1) {
      smjer = Smjerovi::Dolje;
      for (int i = 0; i < s.length(); i++) {
        if (s.at(i) != 'P' && s.at(i) != '1' && s.at(i) != ' ' &&
            s.at(i) != 'D' && s.at(i) != 'o')
          kg = -3;
      }
    } else if (s.find("DoL") != -1) {
      smjer = Smjerovi::DoljeLijevo;
      for (int i = 0; i < s.length(); i++) {
        if (s.at(i) != 'P' && s.at(i) != '1' && s.at(i) != ' ' &&
            s.at(i) != 'D' && s.at(i) != 'o' && s.at(i) != 'L')
          kg = -3;
      }
    } else if (s.find("L") != -1 && s.find("G") == -1 && s.find("Do") == -1) {
      smjer = Smjerovi::Lijevo;
      for (int i = 0; i < s.length(); i++) {
        if (s.at(i) != 'P' && s.at(i) != '1' && s.at(i) != ' ' &&
            s.at(i) != 'L')
          kg = -3;
      }
    } else {
      kg = -1;
      for (int i = 0; i < s.length(); i++) {
        if (s.at(i) != 'P' && s.at(i) != '1' && s.at(i) != ' ')
          kg = -2;
      }
    }
    if (kg == -3)
      greska = KodoviGresaka::SuvisanParametar;
    if (kg == -2)
      greska = KodoviGresaka::NeispravanParametar;
    if (kg == -1)
      greska = KodoviGresaka::NedostajeParametar;
    if (kg == 0) {
      komanda = Komande::PomjeriJednoMjesto;
      return true;
    }
  } else if (s.find("P>") != -1) {
    for (int i = s.length() - 1; i >= s.find("P>") + 2; i--) {
      if (s.at(i) > '0' - 1 && s.at(i) < '9' + 1) {
        if (usl == 0)
          usl = 1;
        if (usl == -1)
          usl = 2;
      }
      if ((s.at(i) < '0' || s.at(i) > '9') && s.at(i) != ' ') {
        break;
      }
      if (s.at(i) == ' ' && usl == 1) {
        usl = -1;
        par2 = stoi(s.substr(i));
      }
      if ((s.at(i) == ' ' || i == s.find("P>") + 1) && usl == 2) {
        usl = 3;
        par1 = stoi(s.substr(i));
      }
    }
    if (usl != -2 && (par1 == -1 || par2 == -1)) {
      for (int i = 0; i < s.length(); i++) {
        if (s.at(i) != 'P' && s.at(i) != '>' && s.at(i) != ' ' &&
            (s.at(i) < '0' || s.at(i) > '9')) {
          usl = -5;
          break;
        }
      }
      if (usl == -5)
        greska = KodoviGresaka::NeispravanParametar;
      else
        greska = KodoviGresaka::NedostajeParametar;
    }
    if (usl != -2 && par1 != -1 && par2 != -1) {
      x = par1;
      y = par2;
      komanda = Komande::PomjeriDalje;
      return true;
    }
  } else if (s.find("B") != -1) {
    for (int i = s.length() - 1; i >= s.find("B") + 1; i--) {
      if (s.at(i) > '0' - 1 && s.at(i) < '9' + 1) {
        if (usl == 0)
          usl = 1;
        if (usl == -1)
          usl = 2;
      }
      if ((s.at(i) < '0' || s.at(i) > '9') && s.at(i) != ' ') {
        break;
      }
      if (s.at(i) == ' ' && usl == 1) {
        usl = -1;
        par2 = stoi(s.substr(i));
      }
      if ((s.at(i) == ' ' || i == s.find("B")) && usl == 2) {
        usl = 3;
        par1 = stoi(s.substr(i));
      }
    }
    if (usl != -2 && (par1 == -1 || par2 == -1)) {
      for (int i = 0; i < s.length(); i++) {
        if (s.at(i) != 'B' && s.at(i) != ' ' &&
            (s.at(i) < '0' || s.at(i) > '9')) {
          usl = -5;
          break;
        }
      }
      if (usl == -5)
        greska = KodoviGresaka::NeispravanParametar;
      else
        greska = KodoviGresaka::NedostajeParametar;
    }
    if (usl != -2 && par1 != -1 && par2 != -1) {
      x = par1;
      y = par2;
      komanda = Komande::Blokiraj;
      return true;
    }
  } else if (s.find("D") != -1) {
    for (int i = s.length() - 1; i >= s.find("D") + 1; i--) {
      if (s.at(i) > '0' - 1 && s.at(i) < '9' + 1) {
        if (usl == 0)
          usl = 1;
        if (usl == -1)
          usl = 2;
      }
      if ((s.at(i) < '0' || s.at(i) > '9') && s.at(i) != ' ') {
        break;
      }
      if (s.at(i) == ' ' && usl == 1) {
        usl = -1;
        par2 = stoi(s.substr(i));
      }
      if ((s.at(i) == ' ' || i == s.find("D")) && usl == 2) {
        usl = 3;
        par1 = stoi(s.substr(i));
      }
    }
    if (usl != -2 && (par1 == -1 || par2 == -1)) {
      for (int i = 0; i < s.length(); i++) {
        if (s.at(i) != 'D' && s.at(i) != ' ' &&
            (s.at(i) < '0' || s.at(i) > '9')) {
          usl = -5;
          break;
        }
      }
      if (usl == -5)
        greska = KodoviGresaka::NeispravanParametar;
      else
        greska = KodoviGresaka::NedostajeParametar;
    }
    if (usl != -2 && par1 != -1 && par2 != -1) {
      x = par1;
      y = par2;
      komanda = Komande::Deblokiraj;
      return true;
    }
  } else if (s.find("PO") != -1) {
    {
      for (int i = 0; i < s.length(); i++) {
        if (s.at(i) != 'P' && s.at(i) != 'O' && s.at(i) != ' ') {
          usl = -5;
          break;
        }
      }
      if (usl == -5)
        greska = KodoviGresaka::SuvisanParametar;
    }
    if (usl != -5) {
      komanda = Komande::PrikaziOkolinu;
      return true;
    }
  } else if (s.find("Z") != -1) {
    {
      for (int i = 0; i < s.length(); i++) {
        if (s.at(i) != 'Z' && s.at(i) != ' ') {
          usl = -5;
          break;
        }
      }
      if (usl == -5)
        greska = KodoviGresaka::SuvisanParametar;
    }
    if (usl != -5) {
      komanda = Komande::ZavrsiIgru;
      return true;
    }
  } else if (s.find("K") != -1) {
    {
      for (int i = 0; i < s.length(); i++) {
        if (s.at(i) != 'K' && s.at(i) != ' ') {
          usl = -5;
          break;
        }
      }
      if (usl == -5)
        greska = KodoviGresaka::SuvisanParametar;
    }
    if (usl != -5) {
      komanda = Komande::KreirajIgru;
      return true;
    }
  } else {
    greska = KodoviGresaka::PogresnaKomanda;
  }
  return false;
}

void IzvrsiKomandu(Komande komanda, Tabla &polja, int &x, int &y,
                   Smjerovi p_smjer = Smjerovi::Gore, int p_x = 0,
                   int p_y = 0) {
  Status st = Status::NijeKraj;
  try {
    if (komanda == Komande::PomjeriJednoMjesto)
      st = Idi(polja, x, y, p_smjer);
    if (komanda == Komande::PomjeriDalje) {
      st = Idi(polja, x, y, p_x, p_y);
    }
    if (komanda == Komande::Blokiraj)
      BlokirajPolje(polja, p_x, p_y);
    if (komanda == Komande::Deblokiraj)
      DeblokirajPolje(polja, p_x, p_y);
    vector<vector<int>> m;
    if (komanda == Komande::PrikaziOkolinu) {
      m = PrikaziOkolinu(polja, x, y);
      for (vector<int> v : m) {
        for (int vv : v) {
          cout << vv << " ";
        }
        cout << endl;
      }
    }
    if (komanda == Komande::ZavrsiIgru) {
      for (int i = 0; i < polja.size(); i++) {
        for (int j = 0; j < polja.size(); j++) {
          polja.at(i).at(j) = Polje::Prazno;
        }
      }
      throw runtime_error("Igra zavrsena");
    }
    if (komanda == Komande::KreirajIgru) {
      x = 0;
      y = 0;
      int br = 0;
      cout << "Unesite broj polja: ";
      while (1) {
        cin >> br;
        if (cin.peek() == '\n' && br > 0)
          break;
        cout << "Greska, unesite ponovo!" << endl;
        cin.clear();
        cin.ignore(10000, '\n');
      }
      cin.clear();
      cin.ignore(10000, '\n');
      cout << "Unesite pozicije mina: ";
      vector<vector<int>> v;
      string min;
      while (1) {
        getline(cin, min);
        int usl = 0, br1 = 0, br2 = 0;
        for (int i = 0; i < min.length(); i++) {
          if (usl == 2 && min.at(i) > '0' - 1 && min.at(i) < '9' + 1) {
            br1 = stoi(min.substr(i));
          } else if (usl == 1 && min.at(i) > '0' - 1 && min.at(i) < '9' + 1) {
            usl = 2;
            br1 = stoi(min.substr(i));
          } else if (usl == 4 && min.at(i) > '0' - 1 && min.at(i) < '9' + 1) {
            br2 = stoi(min.substr(i));
          } else if (usl == 3 && min.at(i) > '0' - 1 && min.at(i) < '9' + 1) {
            usl = 4;
            br2 = stoi(min.substr(i));
          } else if (min.at(i) == ',')
            usl = 3;
          else if (min.at(i) == '(' && usl == 0)
            usl = 1;
          else if (min.at(i) == ')' && usl == 4)
            usl = -2;
          else if (min.at(i) == ' ')
            continue;
          else if (min.at(i) == '.' && usl == 0) {
            usl = -3;
            break;
          } else {
            usl = -1;
            break;
          }
        }
        if (usl == -2) {
          if (br1 < 0 || br1 >= br || br2 < 0 || br2 >= br) {
            usl = -1;
          } else {
            v.push_back({br1, br2});
          }
        }
        if (usl == -3)
          break;
        if (usl == -1)
          cout << "Greska, unesite ponovo!" << endl;
        min.clear();
      }
      polja = KreirajIgru(br, v);
    }
    if (komanda == Komande::PomjeriJednoMjesto ||
        komanda == Komande::PomjeriDalje)
      cout << "Tekuca pozicija igraca je (" << x << "," << y << ")" << endl;
    if (st == Status::KrajPobjeda) {
      cout << "Bravo, obisli ste sva sigurna polja" << endl;
      for (int i = 0; i < polja.size(); i++) {
        for (int j = 0; j < polja.size(); j++) {
          polja.at(i).at(j) = Polje::Prazno;
        }
      }
      throw runtime_error("Igra zavrsena");
    }
    if (st == Status::KrajPoraz) {
      cout << "Nagazili ste na minu" << endl;
      for (int i = 0; i < polja.size(); i++) {
        for (int j = 0; j < polja.size(); j++) {
          polja.at(i).at(j) = Polje::Prazno;
        }
      }
      throw runtime_error("Igra zavrsena");
    }

  } catch (domain_error e) {
    cout << e.what() << endl;
  } catch (logic_error e) {
    cout << e.what() << endl;
  } catch (runtime_error e) {
    throw;
  }
}

int main() {
  try {
    Komande komanda;
    int x = 0, y = 0, x1 = 0, y1 = 0, pom = 0;
    Tabla t;
    Smjerovi sm;
    KodoviGresaka kg;
    while (1) {
      cout << "Unesite komandu: ";
      if (UnosKomande(komanda, sm, x, y, kg)) {
        if (x != x1 || y != y1) {
          pom = x1;
          x1 = x;
          x = pom;
          pom = y1;
          y1 = y;
          y = pom;
        }
        IzvrsiKomandu(komanda, t, x, y, sm, x1, y1);
      } else {
        PrijaviGresku(kg);
      }
      x1 = x;
      y1 = y;
    }

  } catch (...) {
    cout << "Dovidjenja!";
  }
  return 0;
}
