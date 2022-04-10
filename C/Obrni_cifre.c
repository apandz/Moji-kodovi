#include <stdio.h>
#include <stdlib.h>

void obrni_cifre(int niz[], int br) {
    int* p, i, j, z, a, c;
    p=niz;
    for (i=0; i<br; i++) {
        z=1;
        a=0;
        if (*(p+i)<0) {
        z=-1;
        }
        do {
            c=abs(*(p+i)%10);
            a=a*10+c;
            *(p+i)=*(p+i)/10;
        } while (*(p+i)!=0);
        niz[i]=a*z;
    }
}

int main() {
    int niz[100], i, br;
    printf("Unesite broj elemenata niza: ");
    scanf("%d", &br);
    printf("Unesite elemente niza: ");
    for (i=0; i<br; i++) {
    scanf("%d", &niz[i]);
    }
    obrni_cifre(niz, br);
    printf("Niz s obrnutim ciframa je:\n");
    for (i=0; i<br; i++) {
        printf("%d ", niz[i]);
    }
	return 0;
}
