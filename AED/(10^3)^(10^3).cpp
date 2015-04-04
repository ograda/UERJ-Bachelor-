#include <stdio.h>
#include <stdlib.h>
#include <math.h>

void inserir(int*, int);
void eleva(int*, int, int);
void realocar(int*);
int numEnd = 0;

// P^N
int main () {
int N,P, B[5000];
    scanf ("%d %d",&P, &N);
    inserir(B,P);
    eleva(B,N,P);

    printf ("\n\nresultado:\n");
    numEnd--;
    while (numEnd!=-1) printf ("%d",B[numEnd--]);
    return 0;
}

void inserir(int Vet[], int K) {
    int a, b=0;

    for (a=0; floor(K) != 0 ; a++) {
        Vet[a] += K%10;
        K /= 10;
        b++;
    }

    if (b > numEnd) numEnd = b;
}

void realocar(int A[]) {
    int a;

    for (a=0; a < numEnd ; a++)
        if (A[a]<10) continue;
        else {
            if ( numEnd == (a+1) )
                numEnd = (a+2);
            A[(a+1)] += A[a]/10;
            A[a] = A[a]%10;
        }
}

void eleva(int B[],int N, int P){
    int a,b=0;

    for (a=1; a<N; a++) {
        for (b=0; b < numEnd; b++) B[b] *= P;
        realocar(B);
    }
}
