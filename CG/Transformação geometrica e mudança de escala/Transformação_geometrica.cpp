#include <iostream>
#include <string>
#include <algorithm>
#include <math.h>
#include "grayimage.h"
#include "matrix.h"
#include "rectsupport.h"
#define MAX(a,b) ((a) >= (b)) ? (a) : (b)
 
using namespace std;
int main() {
        double escala, dist, distHDL;
        std::string str, strout;
        GrayImage IMGload;
     cin >> str >> escala;
 
     IMGload = loadPGM(str);
     escala = MAX(0.0,escala);
     GrayImage IMGnew(ceil(escala*IMGload.getRows()),ceil(escala*IMGload.getCols()));
 
     if (escala == 1)
         IMGnew = IMGload;
     else {
         double x = IMGload.getCols() / (double) IMGnew.getCols() ;
         double y = IMGload.getRows() / (double) IMGnew.getRows() ;
 
             for (int i=0; i < IMGnew.getRows(); i++)
                for (int j=0; j < IMGnew.getCols(); j++)
                     IMGnew.setPixel(i,j, IMGload.getPixel(floor(i*x),floor(j*y)));
     }
 
     IMGnew.show();
     return 0;
 }