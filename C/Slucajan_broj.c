#include <stdio.h>
#include <math.h>

int slucajni (int a) {
    double x=a;
    x=x/pow(10,8);
    x=sqrt(x);
    x*=10000;
    x=x-(int)x;
    a=x*pow(10,8);
    return a;
}

int main() {
  int a;
  printf("Unesite broj a: ");
  scanf("%d", &a);
  printf("\nSljedeci slucajni broj je: %d", slucajni(a));
  return 0;
}
