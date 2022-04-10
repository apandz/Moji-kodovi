#include <math.h>
#include <stdio.h>
#include <string.h>
#define eps 0.0001

struct Student {
  char ime[20], prezime[20];
  int ocjene[1000];
  int br_ocjena;
};

int prosjeci(struct Student niz[], int n) {
  double prosj[1000] = {0}, a;
  int i, j, k, br=0, max;
  for (i = 0; i < n; i++) {
    for (j = 0; j < niz[i].br_ocjena; j++) {
      prosj[i] += niz[i].ocjene[j];
    }
    prosj[i] = prosj[i] / niz[i].br_ocjena;
    prosj[i] = round(prosj[i] * 100) / 100;
  }
  for (i = 0; i < n; i++) {
    a = prosj[i];
    br=0;
    for (j = i + 1; j < n; j++) {
      if (fabs(prosj[j] - a) < eps) {
        if (br == 0) {
              for (k = i; k < n - 1; k++) {
                prosj[k] = prosj[k + 1];
                niz[k]=niz[k+1];
              }
              n--;
              i--;
              j--;
              br++;
          }
          for (k = j; k < n - 1; k++) {
                prosj[k] = prosj[k + 1];
                niz[k]=niz[k+1];
              }
              n--;
              j--;
        }
      }
    }
    if (n>0) {
    max=0;
    for(i=1; i<n; i++) {
        if (prosj[i]-prosj[max]>eps)
        max=i;
    }
    printf("%s %s\n", niz[max].prezime, niz[max].ime);
    }
    return n;
}

int main() {
  printf("Zadatak 3");
  return 0;
}
