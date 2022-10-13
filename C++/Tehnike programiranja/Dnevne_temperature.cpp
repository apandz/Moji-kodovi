#include <algorithm>
#include <functional>
#include <iostream>
#include <list>
#include <vector>

using namespace std;

class Temperature {
  list<int> mn, mx;
  int min, max;

public:
  Temperature(int min1, int max1) {
    if (max1 < min1) {
      throw range_error("Nekorektne temperature");
    }min = min1;max = max1;
  }
  void RegistrirajTemperature(pair<int, int> p) {
    if (p.first > p.second || p.first < min || p.second > max) {
      throw range_error("Nekorektne temperature");
    }
    mn.push_back(p.first);
    mx.push_back(p.second);
  }
  void BrisiSve() {
    mn.clear();
    mx.clear();
  }
  void BrisiNegativneTemperature();
  int DajBrojRegistriranihTemperatura() const { return mn.size(); }
  int DajMinimalnuTemperaturu() const {
    int x = *min_element(mn.begin(), mn.end());
    if (x == 0) {
      throw logic_error("Nema registriranih temperatura");
    }
    return x;
  }
  int DajMaksimalnuTemperaturu() const {
    int x = *max_element(mx.begin(), mx.end());
    if (x == 0) {
      throw logic_error("Nema registriranih temperatura");
    }
    return x;
  }
  int DajBrojTemperaturaManjihOd(int tm) const;
  int DajBrojTemperaturaVecihOd(int tm) const;
  bool operator!(){if(mn.size()==0) {return true;} return false;}
  friend Temperature& operator++(Temperature& tm);
  friend Temperature operator++(Temperature& tm, int);
  friend Temperature& operator--(Temperature& tm);
  friend Temperature operator--(Temperature& tm, int);
  friend vector<int> operator*(const Temperature& tm);
  friend vector<int> operator-(const Temperature& tm);
  friend vector<int> operator+(const Temperature& tm);
  pair<int,int> operator[](int n);
  friend Temperature operator+(const Temperature& tm, int n);
  friend Temperature operator+(int n, const Temperature& tm);
  friend Temperature operator-(const Temperature& tm, int n);
  friend Temperature operator-(int n, const Temperature& tm);
  Temperature& operator+=(int n);
  Temperature& operator-=(int n);
  friend bool operator==(const Temperature& tm1,const Temperature& tm2);
  friend bool operator!=(const Temperature& tm1,const Temperature& tm2);
  friend ostream &operator<<(ostream &tok, const Temperature &t);
};

void Temperature::BrisiNegativneTemperature() {
  auto it1 = mn.begin();
  auto it2 = mx.begin();
  while (it1 != mn.end()) {
    if (*it1 < 0 && *it2 < 0) {
      mn.erase(it1);
      mx.erase(it2);
      it1 = mn.begin();
      it2 = mx.begin();
    } else {
      it1++;
      it2++;
    }
  }
}

int Temperature::DajBrojTemperaturaManjihOd(int tm) const {
    int x =
        count_if(mn.begin(), mn.end(), bind(less<int>(), placeholders::_1, tm));
    int y =
        count_if(mx.begin(), mx.end(), bind(less<int>(), placeholders::_1, tm));
    if (x + y == 0) {
      throw logic_error("Nema registriranih temperatura");
    }
    return x + y;
  }

int Temperature::DajBrojTemperaturaVecihOd(int tm) const {
    int x = count_if(mn.begin(), mn.end(),
                     bind(greater<int>(), placeholders::_1, tm));
    int y = count_if(mx.begin(), mx.end(),
                     bind(greater<int>(), placeholders::_1, tm));
    if (x + y == 0) {
      throw logic_error("Nema registriranih temperatura");
    }
    return x + y;
}

Temperature& operator++(Temperature& tm){
    auto it=tm.mn.begin();
    while(it!=tm.mn.end()) {
        if((*it)+1>tm.max) {throw logic_error("Ilegalna operacija");}
        (*it)++; 
        it++;
    }
    return tm;
}

Temperature operator++(Temperature& tm, int) {
    Temperature tm1(tm);
    ++tm;
    return tm1;
}

Temperature& operator--(Temperature& tm) {
    auto it=tm.mx.begin();
    while(it!=tm.mx.end()) {
        if((*it)-1<tm.min) {throw logic_error("Ilegalna operacija");}
        (*it)--; 
        it++;
    }
    return tm;
}

Temperature operator--(Temperature& tm, int) {
    Temperature tm1(tm);
    --tm;
    return tm1;
}

vector<int> operator*(const Temperature& tm) {
    vector<int> v;
    auto it1=tm.mn.begin(); auto it2=tm.mx.begin();
    while(it1!=tm.mn.end()) {
        v.push_back((*it2)-(*it1));
        it1++; it2++;
    }
    return v;
}

vector<int> operator-(const Temperature& tm) {
    vector<int> v;
    auto it1=tm.mn.begin();
    while(it1!=tm.mn.end()) {
        v.push_back((*it1)-tm.min);
        it1++;
    }
    return v;
}

vector<int> operator+(const Temperature& tm) {
    vector<int> v;
    auto it2=tm.mx.begin();
    while(it2!=tm.mx.end()) {
        v.push_back((*it2)-tm.max);
        it2++;
    }
    return v;
}

pair<int,int> Temperature::operator[](int n) {
    if(n<=0 || n>mn.size()) {throw range_error("Neispravan indeks");}
    auto it1=mn.begin(); auto it2=mx.begin();
    int i=1;
    while(it1!=mn.end()) {
        if(i==n) break;
        it1++; it2++; i++;
    }
    return {*it1,*it2};
}

Temperature operator+(const Temperature& tm, int n) {
    Temperature tm1(tm);
    auto it1=tm1.mn.begin(); auto it2=tm1.mx.begin();
    while(it1!=tm1.mn.end()) {
        (*it1)+=n; (*it2)+=n;
        if((*it1)>tm1.max || (*it2)>tm1.max) {throw logic_error("Prekoracen dozvoljeni opseg temperatura");}
        it1++; it2++;
    }
    return tm1;
}

Temperature operator+(int n, const Temperature& tm) {
    Temperature tm1(tm);
    auto it1=tm1.mn.begin(); auto it2=tm1.mx.begin();
    while(it1!=tm1.mn.end()) {
        (*it1)+=n; (*it2)+=n;
        if((*it1)>tm1.max || (*it2)>tm1.max) {throw logic_error("Prekoracen dozvoljeni opseg temperatura");}
        it1++; it2++;
    }
    return tm1;
}

Temperature operator-(const Temperature& tm, int n) {
    Temperature tm1(tm);
    auto it1=tm1.mn.begin(); auto it2=tm1.mx.begin();
    while(it1!=tm1.mn.end()) {
        (*it1)-=n; (*it2)-=n;
        if((*it1)<tm1.min || (*it2)<tm1.min) {throw logic_error("Prekoracen dozvoljeni opseg temperatura");}
        it1++; it2++;
    }
    return tm1;
}

Temperature operator-(int n, const Temperature& tm) {
    Temperature tm1(tm);
    auto it1=tm1.mn.begin(); auto it2=tm1.mx.begin();
    while(it1!=tm1.mn.end()) {
        (*it1)=n-(*it1); (*it2)=n-(*it2);
        if((*it1)<tm1.min || (*it2)<tm1.min) {throw logic_error("Prekoracen dozvoljeni opseg temperatura");}
        it1++; it2++;
    }
    return tm1;
}

Temperature& Temperature::operator+=(int n) {
    auto it1=mn.begin(); auto it2=mx.begin();
    while(it1!=mn.end()) {
        (*it1)+=n; (*it2)+=n;
        if((*it1)>max || (*it2)>max) {throw logic_error("Prekoracen dozvoljeni opseg temperatura");}
        it1++; it2++;
    }
    return *this;
}

Temperature& Temperature::operator-=(int n) {
    auto it1=mn.begin(); auto it2=mx.begin();
    while(it1!=mn.end()) {
        (*it1)-=n; (*it2)-=n;
        if((*it1)<min || (*it2)<min) {throw logic_error("Prekoracen dozvoljeni opseg temperatura");}
        it1++; it2++;
    }
    return *this;
}

bool operator==(const Temperature& tm1,const Temperature& tm2) {
    if(tm1.mn.size()!=tm2.mn.size() ||tm1.min!=tm2.min || tm1.max!=tm2.max) {return false;}
    if(is_permutation(tm1.mn.begin(),tm1.mn.end(),tm2.mn.begin()) && is_permutation(tm1.mx.begin(),tm1.mx.end(),tm2.mx.begin())) {return true;}
    return false;
}

bool operator!=(const Temperature& tm1,const Temperature& tm2) {
    if(tm1==tm2) return false;
    return true;
}

ostream &operator<<(ostream &tok, const Temperature &t) {
    for_each(t.mn.begin(),t.mn.end(),[&tok](int x) {tok<<x<<" ";});
    tok<<endl;
    for_each(t.mx.begin(),t.mx.end(),[&tok](int x) {tok<<x<<" ";});
  return tok;
}

int main() {
  Temperature t(-10, 10);
  t.RegistrirajTemperature({-2, -2});
  t.RegistrirajTemperature({-3, -3});
  t.RegistrirajTemperature({3, 3});
  t.RegistrirajTemperature({-3, 3});
  cout << t.DajMinimalnuTemperaturu() << endl;
  cout << t.DajBrojTemperaturaManjihOd(3) << endl;
  t.BrisiNegativneTemperature();
  cout << t;
  return 0;
}
