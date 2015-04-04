#include <iostream>
#include <string>
#include <algorithm>
#include <math.h>
#include "matrix.h"
#include "rectsupport.h"
#include "grayimage.h"
#define MAX(a,b) ((a) >= (b)) ? (a) : (b)
#define MIN(a,b) ((a) <= (b)) ? (a) : (b)
 
using namespace std;
 
struct ponto {
    double x,y;
    ponto(double x,double y) : x(x), y(y) {}
    ponto() {}
};
 
int main() {
         std::string str;
    double pX,pY;
    int i = 0, j;
    ponto P[8];
    Matrix newMatrix (3,3), X(8,1), obs (8,1), coef (8,8);
    GrayImage IMGload, IMGnew (50,50);
 
         cin >> str;
    while (i < 8) P[i++] = (cin >> pX >> pY, ponto (pX,pY));
    IMGload = loadPGM(str);
 
    // preenche as matrizes
    for (i=1; i <= 8; (i+=2)) {
        obs.put(i,1,P[i].x);
        obs.put((i+1),1,P[i].y);
        for (j=0; j <= 1; j++) {
            coef.put(i+j,1+(j*3),P[(i-1)].x);
            coef.put(i+j,2+(j*3),P[(i-1)].y);
            coef.put(i+j,3+(j*3),1);
            coef.put(i+j,7,-(P[(i-1)].x)*(j?(P[i].y):(P[i].x)));
            coef.put(i+j,8,-(P[(i-1)].y)*(j?(P[i].y):(P[i].x)));
        }
    }
 
    Matrix AT, ATA;
    AT = transpose(coef);
    ATA = AT*coef;
    X =  inverse(ATA) *AT*obs;
 
    for (i=0; i<=7 ; i++)
        newMatrix.put(1+(i/3),1+(i%3),X.get((1+i),1));
    newMatrix.put(3,3,1);
   newMatrix.show();
   newMatrix = transpose(newMatrix);
   
   double fy1,fy2, x,y;
        int x1,x2,y1,y2,f11,f12,f21,f22, pxl;
   for (i=0; i < IMGnew.getRows(); i++)
        for (j=0; j < IMGnew.getCols(); j++) {
         x = ((newMatrix.get(1,1)*i+newMatrix.get(2,1)*j+newMatrix.get(3,1))/(newMatrix.get(1,3)*i+newMatrix.get(2,3)*j+ newMatrix.get(3,3)));
         y = ((newMatrix.get(1,2)*i+newMatrix.get(2,2)*j+ newMatrix.get(3,2))/(newMatrix.get(1,3)*i+newMatrix.get(2,3)*j+ newMatrix.get(3,3)));
                        x = MAX(MIN(199,x),0.1);
                        y = MAX(MIN(199,y),0.1);
                        if ( i || j)
                                x1 = ceil(x), x2 = x1-1, y1 = ceil(y), y2 = y1-1;
                   else
                        x1 = floor(x), x2 = x1-1, y1 = floor(y), y2 = y1-1;
 
                        f11 = IMGload.getPixel(x1,y1);
                        f12 = IMGload.getPixel(x1,y2);
                        f21 = IMGload.getPixel(x2,y1);
                        f22 = IMGload.getPixel(x2,y2);
                       
                        pxl = (int) ( ( 1/ ((x2-x1)*(y2-y1)) )*( (f11*(x2-x)*(y2-y)) + (f21*(x-x1)*(y2-y)) + (f12*(x2-x)*(y-y1)) + (f22*(x-x1)*(y-y1)) ) );
                IMGnew.setPixel(i,j, pxl);
        }
    IMGnew.show();
    return 0;
}