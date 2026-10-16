#include <stdio.h>
// #include <math.h>
#define GREETING "Aceasta este o demonstratie a etapelor de compilare."

// Funcția primește doi parametri (a și b), dar folosește doar 'a'.
int calculeaza(int a, int b) {
    return a * 2;
}

int main() {
    int unused;
    printf("Nivelul 1: Salut, GCC si Make!\n");
    printf("Ai idee cat e 2*2?\n");
    printf("%d\n", calculeaza(2,2));
    // CAN YOU SEE ME?
    printf("%s\n", GREETING);
    // double baza, exponent;
    // scanf("%lf %lf", &baza, &exponent);
    // printf("%lf^%lf= %lf\n",baza, exponent, pow(baza,exponent));
    return 0;
}