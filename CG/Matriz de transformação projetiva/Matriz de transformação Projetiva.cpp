#include <iostream>
#include <string>
#include <algorithm>
#include <math.h>
#include "matrix.h"
#include "rectsupport.h"
#define MAX(a,b) ((a) >= (b)) ? (a) : (b)
 
using namespace std;
 
struct ponto ;
 
int main() {
    double hdl, x, y;
    int i = 0, j;
    ponto P[8];
    Matrix newMatrix (3,3), X(8,1), obs (8,1), coef (8,8);
 
    while (i < 8) P[i++] = (cin >> x >> y, ponto (x,y));
 
    // preenche parte de 'coeficientes'
    for (i=1; i <= 8; (i+=2))
        for (j=0; j <= 1; j++) {
            coef.put(i+j,1+(j*3),P[(i-1)].x);
             coef.put(i+j,2+(j*3),P[(i-1)].y);
             coef.put(i+j,3+(j*3),1);
             coef.put(i+j,7,-(P[(i-1)].x));
             coef.put(i+j,8,-(P[(i-1)].y));
         }
 
     // preenche matriz de 'Observações' e parte de 'coeficientes'
     for (i=1; i <= 8; (i+=2)) {
         obs.put(i,1,P[i].x);
         obs.put((i+1),1,P[i].y);
         hdl = coef.get(i,7);
         coef.put(i,7,hdl*P[i].x);
         hdl = coef.get(i,8);
         coef.put(i,8,hdl*P[i].x);
         hdl = coef.get((i+1),7);
         coef.put((i+1),7,hdl*P[i].y);
         hdl = coef.get((i+1),8);
         coef.put((i+1),8,hdl*P[i].y);
     }
 
     Matrix AT, ATA;
     AT = transpose(coef);
     ATA = AT*coef;
     X =  inverse(ATA) *AT*obs;
 
     for (i=0; i<=7 ; i++)
         newMatrix.put(1+(i/3),1+(i%3),X.get((1+i),1));
     newMatrix.put(3,3,1);
 
     newMatrix.show();
     return 0;
 }