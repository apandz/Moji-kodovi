#include <algorithm>
#include <cmath>
#include <fstream>
#include <iostream>
#include <vector>
#include <ctime>

using namespace std;

template <typename Tip> void ispisi(Tip *niz, int vel) {
  for (int i = 0; i < vel; i++) {
    cout << niz[i];
    if (i != vel - 1)
      cout << ",";
  }
  cout << endl;
}

template <typename Tip> void bubble_sort(Tip *niz, int vel) {
  while (vel) {
    int i = vel - 1;
    vel = 0;
    for (int j = 1; j <= i; j++) {
      if (niz[j - 1] > niz[j]) {
        Tip temp = niz[j - 1];
        niz[j - 1] = niz[j];
        niz[j] = temp;
        vel = j;
      }
    }
  }
}

void test_bubble_sort_1() {
  string niz[8]{"bah", "xf", "aab", "ggd", "aa", "cd", "cdef", "ab"};
  string niz1[8]{"bah", "xf", "aab", "ggd", "aa", "cd", "cdef", "ab"};
  bubble_sort(niz, 8);
  sort(niz1, niz1 + 8);
  cout << "Test bubble_sort:\nOčekivani output:\n";
  ispisi(niz1, 8);
  cout << "Stvarni output:\n";
  ispisi(niz, 8);
}

void test_bubble_sort_2() {
  double niz[8]{5.5, 17.234, 17.23, 2.03, 1.112, 15.53, 9.4, 9.9};
  double niz1[8]{5.5, 17.234, 17.23, 2.03, 1.112, 15.53, 9.4, 9.9};
  bubble_sort(niz, 8);
  sort(niz1, niz1 + 8);
  cout << "Test bubble_sort:\nOčekivani output:\n";
  ispisi(niz1, 8);
  cout << "Stvarni output:\n";
  ispisi(niz, 8);
}

template <typename Tip> void selection_sort(Tip *niz, int vel) {
  for (int i = 0; i < vel - 1; i++) {
    Tip min = niz[i];
    int mini = i;
    for (int j = i + 1; j < vel; j++) {
      if (niz[j] < min) {
        min = niz[j];
        mini = j;
      }
    }
    niz[mini] = niz[i];
    niz[i] = min;
  }
}

void test_selection_sort_1() {
  string niz[8]{"bah", "xf", "aab", "ggd", "aa", "cd", "cdef", "ab"};
  string niz1[8]{"bah", "xf", "aab", "ggd", "aa", "cd", "cdef", "ab"};
  selection_sort(niz, 8);
  sort(niz1, niz1 + 8);
  cout << "Test selection_sort:\nOčekivani output:\n";
  ispisi(niz1, 8);
  cout << "Stvarni output:\n";
  ispisi(niz, 8);
}

void test_selection_sort_2() {
  double niz[8]{5.5, 17.234, 17.23, 2.03, 1.112, 15.53, 9.4, 9.9};
  double niz1[8]{5.5, 17.234, 17.23, 2.03, 1.112, 15.53, 9.4, 9.9};
  selection_sort(niz, 8);
  sort(niz1, niz1 + 8);
  cout << "Test selection_sort:\nOčekivani output:\n";
  ispisi(niz1, 8);
  cout << "Stvarni output:\n";
  ispisi(niz, 8);
}

template <typename Tip>
int particija(Tip *niz, const int prvi, const int zadnji) {
  Tip pivot = niz[prvi];
  int p = prvi + 1;
  while (p <= zadnji && niz[p] < pivot) {
    p++;
  }
  for (int i = p + 1; i <= zadnji; i++) {
    if (niz[i] < pivot) {
      Tip temp = niz[i];
      niz[i] = niz[p];
      niz[p] = temp;
      p++;
    }
  }
  Tip temp = niz[prvi];
  niz[prvi] = niz[p - 1];
  niz[p - 1] = temp;
  return p - 1;
}

template <typename Tip> void quick_sort(Tip *niz, int vel, const int prvi = 0) {
  vel--;
  if (prvi < vel) {
    int j = particija(niz, prvi, vel);
    quick_sort(niz, j, prvi);
    quick_sort(niz, vel + 1, j + 1);
  }
}

void test_quick_sort_1() {
  string niz[8]{"bah", "xf", "aab", "ggd", "aa", "cd", "cdef", "ab"};
  string niz1[8]{"bah", "xf", "aab", "ggd", "aa", "cd", "cdef", "ab"};
  quick_sort(niz, 8);
  sort(niz1, niz1 + 8);
  cout << "Test quick_sort:\nOčekivani output:\n";
  ispisi(niz1, 8);
  cout << "Stvarni output:\n";
  ispisi(niz, 8);
}

void test_quick_sort_2() {
  double niz[8]{5.5, 17.234, 17.23, 2.03, 1.112, 15.53, 9.4, 9.9};
  double niz1[8]{5.5, 17.234, 17.23, 2.03, 1.112, 15.53, 9.4, 9.9};
  quick_sort(niz, 8);
  sort(niz1, niz1 + 8);
  cout << "Test quick_sort:\nOčekivani output:\n";
  ispisi(niz1, 8);
  cout << "Stvarni output:\n";
  ispisi(niz, 8);
}

template <typename Tip>
void merge(Tip *niz, const int l, const int p, const int q, const int u) {
  Tip *b = new Tip[u - l + 1];
  int i = 0;
  int j = q - l;
  int k = l;
  for (int m = 0; m <= u - l; m++) {
    b[m] = niz[l + m];
  }
  while (i <= p - l && j <= u - l) {
    if (b[i] < b[j]) {
      niz[k] = b[i++];
    } else {
      niz[k] = b[j++];
    }
    k++;
  }
  while (i <= p - l) {
    niz[k++] = b[i++];
  }
  while (j <= u - l) {
    niz[k++] = b[j++];
  }
  delete[] b;
}

template <typename Tip> void merge_sort(Tip *niz, int vel, const int l = 0) {
  if (vel - 1 > l) {
    int p = floor((l + vel - 2) / 2);
    int q = p + 1;
    merge_sort(niz, p + 1, l);
    merge_sort(niz, vel, q);
    merge(niz, l, p, q, vel - 1);
  }
}

void test_merge_sort_1() {
  string niz[8]{"bah", "xf", "aab", "ggd", "aa", "cd", "cdef", "ab"};
  string niz1[8]{"bah", "xf", "aab", "ggd", "aa", "cd", "cdef", "ab"};
  merge_sort(niz, 8);
  sort(niz1, niz1 + 8);
  cout << "Test merge_sort:\nOčekivani output:\n";
  ispisi(niz1, 8);
  cout << "Stvarni output:\n";
  ispisi(niz, 8);
}

void test_merge_sort_2() {
  double niz[8]{5.5, 17.234, 17.23, 2.03, 1.112, 15.53, 9.4, 9.9};
  double niz1[8]{5.5, 17.234, 17.23, 2.03, 1.112, 15.53, 9.4, 9.9};
  merge_sort(niz, 8);
  sort(niz1, niz1 + 8);
  cout << "Test merge_sort:\nOčekivani output:\n";
  ispisi(niz1, 8);
  cout << "Stvarni output:\n";
  ispisi(niz, 8);
}

void ucitaj(string filename, int *&niz, int &vel) {
  ifstream ulazni_tok(filename);
  if (!ulazni_tok)
    cout << "Otvaranje datoteke nije uspjelo!" << endl;
  else {
      vector<int> v;
      int broj;
    while (ulazni_tok >> broj) {
        v.push_back(broj);
    }
    vel=v.size();
    niz=new int[vel]();
    for(int i=0; i<vel; i++) {
        niz[i]=v.at(i);
    }
  }
}

void generisi(string filename, int vel) {
  ofstream izlazni_tok(filename);
  if (!izlazni_tok)
    cout << "Otvaranje datoteke nije uspjelo!" << endl;
  else {
    for (int i = 0; i < vel; i++) {
      izlazni_tok << rand() << " ";
    }
  }
}

void test_ucitaj_1() {
  ofstream izlazni_tok("brojevi.txt");
  generisi("brojevi.txt", 8);
  int *niz;
  int vel = 0;
  izlazni_tok.close();
  ucitaj("brojevi.txt", niz, vel);
  cout << "Test ucitaj:\n";
  ispisi(niz, vel);
  if (vel == 8)
    cout << "Tačno" << endl;
  else
    cout << "Netačno" << endl;
  delete[] niz;
}

void test_ucitaj_2() {
  ofstream izlazni_tok("brojevi.txt");
  int niz1[8] = {5, 6, 2, 4, 6, 1, 11, 34};
  for (int i = 0; i < 8; i++) {
    izlazni_tok << niz1[i] << " ";
  }
  int *niz;
  int vel = 0;
  izlazni_tok.close();
  ucitaj("brojevi.txt", niz, vel);
  cout << "Test ucitaj:\nOčekivani output:\n";
  ispisi(niz1, 8);
  cout << "Stvarni output:\n";
  ispisi(niz, vel);
  delete[] niz;
}

string ucitajSortirajZapisiNiz(string filename) {
    int* niz=nullptr, vel=0;
    ucitaj(filename,niz,vel);
    cout<<"Odaberite algoritam za sortiranje (1 - bubble sort, 2 - selection_sort, 3 - quick sort, 4 - merge_sort): ";
    int broj;
    cin>>broj;
    while(broj<1 || broj>4) {
        cin>>broj;
    }
    if(broj==1) {
        clock_t vrijeme1 = clock();
        bubble_sort(niz,vel);
        clock_t vrijeme2 = clock();
        int ukvrijeme = (vrijeme2 - vrijeme1) / (CLOCKS_PER_SEC / 1000);
        cout << "Vrijeme izvrsenja: " << ukvrijeme << " ms." << endl;
    } else if(broj==2) {
        clock_t vrijeme1 = clock();
        selection_sort(niz,vel);
        clock_t vrijeme2 = clock();
        int ukvrijeme = (vrijeme2 - vrijeme1) / (CLOCKS_PER_SEC / 1000);
        cout << "Vrijeme izvrsenja: " << ukvrijeme << " ms." << endl;
    } else if(broj==3) {
        clock_t vrijeme1 = clock();
        quick_sort(niz,vel);
        clock_t vrijeme2 = clock();
        int ukvrijeme = (vrijeme2 - vrijeme1) / (CLOCKS_PER_SEC / 1000);
        cout << "Vrijeme izvrsenja: " << ukvrijeme << " ms." << endl;
        
    } else {
        clock_t vrijeme1 = clock();
        merge_sort(niz,vel);
        clock_t vrijeme2 = clock();
        int ukvrijeme = (vrijeme2 - vrijeme1) / (CLOCKS_PER_SEC / 1000);
        cout << "Vrijeme izvrsenja: " << ukvrijeme << " ms." << endl;
    }
    bool sortiran=true;
    for(int i=0; i<vel-1; i++) {
        if(niz[i]>niz[i+1]) {
            sortiran=false;
        }
    }
    if(sortiran) {
        cout<<"Niz je sortiran!"<<endl;
    } else {
        cout<<"Niz nije sortiran!"<<endl;
    }
    string naziv="SortiranNiz.txt";
    ofstream izlazni_tok(naziv);
    if (!izlazni_tok)
    cout << "Otvaranje datoteke nije uspjelo!" << endl;
    else {
        for(int i=0; i<vel; i++) {
            izlazni_tok<<niz[i]<<" ";
        }
    }
    return naziv;
}

void test_ucitajSortirajZapisiNiz_1() {
  ofstream izlazni_tok("brojevi.txt");
  generisi("brojevi.txt", 10);
  izlazni_tok.close();
  string naziv=ucitajSortirajZapisiNiz("brojevi.txt");
  int *niz;
  int vel = 0;
  ucitaj(naziv, niz, vel);
  cout << "Niz:\n";
  ispisi(niz, vel);
  delete[] niz;
}

void test_ucitajSortirajZapisiNiz_2() {
  ofstream izlazni_tok("brojevi.txt");
  int niz1[8] = {5, 6, 2, 4, 6, 1, 11, 34};
  for (int i = 0; i < 8; i++) {
    izlazni_tok << niz1[i] << " ";
  }
  izlazni_tok.close();
  string naziv=ucitajSortirajZapisiNiz("brojevi.txt");
  int *niz;
  int vel = 0;
  ucitaj(naziv, niz, vel);
  cout << "Očekivani output:\n";
  sort(niz1,niz1+8);
  ispisi(niz1, 8);
  cout << "Stvarni output:\n";
  ispisi(niz, vel);
  delete[] niz;
}

int main() {
  test_bubble_sort_1();
  cout << endl;
  test_bubble_sort_2();
  cout << endl;
  test_selection_sort_1();
  cout << endl;
  test_selection_sort_2();
  cout << endl;
  test_quick_sort_1();
  cout << endl;
  test_quick_sort_2();
  cout << endl;
  test_merge_sort_1();
  cout << endl;
  test_merge_sort_2();
  cout << endl;
  test_ucitaj_1();
  cout << endl;
  test_ucitaj_2();
  cout<<endl;
  test_ucitajSortirajZapisiNiz_1();
  cout<<endl;
  test_ucitajSortirajZapisiNiz_2();
  return 0;
}
