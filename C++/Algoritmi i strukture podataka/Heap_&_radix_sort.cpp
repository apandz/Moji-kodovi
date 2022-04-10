#include <algorithm>
#include <iostream>
#include <vector>

using namespace std;

void radixSort(vector<int> &a) {
  int max = 0;
  for (int i = 0; i < a.size(); i++) {
    if (a.at(i) > a.at(max)) {
      max = i;
    }
  }
  max = a.at(max);
  vector<int> redovi[10];
  for (int j = 1; max / j > 0; j *= 10) {
    for (int i = 0; i < a.size(); i++) {
      redovi[(a.at(i) / j) % 10].push_back(a.at(i));
    }
    int k = 0;
    for (int i = 0; i < 10; i++) {
      for (int j = 0; j < redovi[i].size(); j++) {
        a.at(k++) = redovi[i].at(j);
      }
      redovi[i].clear();
    }
  }
}

void ispisi(const vector<int> &a) {
  for (int i = 0; i < a.size(); i++) {
    cout << a.at(i);
    if (i == a.size() - 1)
      cout << endl;
    else
      cout << " ";
  }
}

void test_radixSort() {
  vector<int> a({248, 820, 788, 643, 347, 329, 673, 174, 293, 455, 271, 625}),
      b = a;
  radixSort(a);
  sort(b.begin(), b.end());
  cout << "Test radixSort:\n    Očekivan output:\n";
  ispisi(b);
  cout << "    Stvaran output:\n";
  ispisi(a);
}

void popraviDolje(vector<int> &a, int i, int vel) {
  while (!(i >= vel / 2 && i < vel)) {
    int veci = 2 * i + 1;
    int dd = 2 * i + 2;
    if (dd < vel && a.at(dd) > a.at(veci)) {
      veci = dd;
    }
    if (a.at(i) > a.at(veci)) {
      return;
    }
    swap(a.at(i), a.at(veci));
    i = veci;
  }
}

void stvoriGomilu(vector<int> &a) {
  int vel = a.size();
  for (int i = vel / 2; i >= 0; i--) {
    popraviDolje(a, i, vel);
  }
}

void popraviGore(vector<int> &a, int i) {
  while (i != 0 && a.at(i) > a.at((i - 1) / 2)) {
    swap(a.at(i), a.at((i - 1) / 2));
    i = (i - 1) / 2;
  }
}

void umetniUGomilu(vector<int> &a, int umetnuti, int& velicina) {
  a.push_back(umetnuti);
  velicina++;
  popraviGore(a, velicina - 1);
}

int izbaciPrvi(vector<int> &a, int &velicina) {
  if (velicina == 0) {
    throw domain_error("Gomila je prazna");
  }
  velicina--;
  swap(a.at(0), a.at(velicina));
  if (velicina != 0) {
    popraviDolje(a, 0, velicina);
  }
  return a.at(velicina);
}

void gomilaSort(vector<int> &a) {
  stvoriGomilu(a);
  int vel = a.size();
  for (int i = 0; i < a.size() - 1; i++) {
    izbaciPrvi(a, vel);
  }
}

void test_stvoriGomilu() {
    vector<int> a({2,15,27,6,11,3,4,9,22,29,31,1,14}),a1(a);
    make_heap(a1.begin(),a1.end());
    stvoriGomilu(a);
    cout << "Test stvoriGomilu:\n    Očekivan output:\n";
    ispisi(a);
    cout << "    Stvaran output:\n";
    ispisi(a1);
}

void test_umetniUGomilu() {
    vector<int> a({2,15,27,6,11,3,4,9,22,29,31,1,14}),a1(a);
    make_heap(a1.begin(),a1.end());
    a1.push_back(16);
    push_heap(a1.begin(),a1.end());
    stvoriGomilu(a);
    int vel=a.size();
    umetniUGomilu(a,16,vel);
    cout << "Test umetniUGomilu:\n    Očekivan output:\n";
    ispisi(a);
    cout << "    Stvaran output:\n";
    ispisi(a1);
}

void test_izbaciPrvi() {
    vector<int> a({2,15,27,6,11,3,4,9,22,29,31,1,14}),a1(a);
    make_heap(a1.begin(),a1.end());
    pop_heap(a1.begin(),a1.end());
    stvoriGomilu(a);
    int vel=a.size();
    izbaciPrvi(a,vel);
    cout << "Test izbaciPrvi:\n    Očekivan output:\n";
    ispisi(a);
    cout << "    Stvaran output:\n";
    ispisi(a1);
}

void test_gomilaSort() {
  vector<int> a({248, 820, 788, 643, 347, 329, 673, 174, 293, 455, 271, 625}),
      b = a;
  gomilaSort(a);
  sort(b.begin(), b.end());
  cout << "Test gomilaSort:\n    Očekivan output:\n";
  ispisi(b);
  cout << "    Stvaran output:\n";
  ispisi(a);
}

int main() {
  test_radixSort();
  cout<<endl;
  test_stvoriGomilu();
  cout<<endl;
  test_umetniUGomilu();
  cout<<endl;
  test_izbaciPrvi();
  cout<<endl;
  test_gomilaSort();
  return 0;
}
