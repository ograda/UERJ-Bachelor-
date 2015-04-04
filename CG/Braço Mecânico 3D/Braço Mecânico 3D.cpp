#include <X11/Xlib.h>
#include <GL/gl.h>
#include <GL/glut.h>
#include <iostream>
#include <math.h>
#include <stdio.h>
#define MAX(a,b) ((a) >= (b)) ? (a) : (b)
#define MIN(a,b) ((a) <= (b)) ? (a) : (b)
#define TetaMAX 180
#define TetaMIN 0
#define deltaT 10
 
// MODIFICAÇÕES NO PROGRAMA
#define tAnima 20.0 // tempo de animação
#define LIMITTHETA // limita a variação de teta para evitar giros impossíveis no braço
#define DRAWSOLID // desenha o braço na forma solida (caso contrario desenhara linhas)
#define KEEPALIVE // mantem a tela aberta apos o fim da animação
#define WHITEBG // define o bg branco, caso contrário será preto
#define KEYBOARD // habilita o uso do teclado durante a animação (zoom, pam)
#define LIGHT // habilita os efeitos de luz e sombra
#define MATERIAL // habilita os aspectos de material
//#define HAND // adiciona mão ao braço mecanico
//#define FINGER // adiciona dedos ao braço mecanico
//#define MANUALINPUT // habilita o input manual, em preferencia a linha de comando
 
using namespace std;
 
const GLfloat light_ambient[]  = { 0., 0., 0., 1. };
const GLfloat light_diffuse[]  = { 1., 1., 1., 1. };
const GLfloat light_specular[] = { 1., 1., 1., 1. };
const GLfloat light_position[] = { 2., 5., 5., 0. };
 
const GLfloat mat_ambient[]    = { 0.7, 0.7, 0.7, 1. };
const GLfloat mat_diffuse[]    = { 0.8, 0.8, 0.8, 1. };
const GLfloat mat_specular[]   = { 1., 1., 1., 1. };
const GLfloat high_shininess[] = { 100. };
 
GLfloat camDis = 15, camAng = 0, camCentX = 0, camCentY = 0, camCentZ = 0;
float inicial_tetas[6], final_tetas[6], tetas[6], l, TempoDecorrido;
enum { OMBRO_ROTACAO, OMBRO, COTOVELO, ANTEBRACO, MAO, DEDOS };
 
void draw(void) {
    glLoadIdentity();
    GLfloat x = camDis * sin(camAng);
    GLfloat z = camDis * cos(camAng);
    gluLookAt (x, 0., z, camCentX, camCentY, camCentZ, 0., 1., 0.);
 
    glPushMatrix();
        // esfera que representa a rotação do ombro (eixo y - teta 1)
       glColor4f(0., 1., 0.,0.5);
        glutWireSphere(0.3, 20, 20);
        glRotatef((GLfloat) tetas[OMBRO_ROTACAO], 0., 1., 0.);
        glRotatef((GLfloat) tetas[OMBRO], 0., 0., 1.);
        glTranslatef(0.8, 0., 0.);
        // retangulo representando o movimento do ombro (parte superior do braço)
            glPushMatrix();
                glScalef(1.0, 0.9, 0.6);
                glColor4f(0.0, 0.0, 0.8, 0.5);
                #ifdef DRAWSOLID
                    glutSolidCube(1.);
                #else
                    glutWireCube(1.);
                #endif
            glPopMatrix();
        glTranslatef(0.5, 0., 0.);
        // retangulo representando o movimento do cotovelo (parte media do braço) -- se L é maior que 0
        if (l) {
        glRotatef((GLfloat) tetas[COTOVELO], 0., 0., 1.);
        glTranslatef(l/2, 0., 0.);
            glPushMatrix();
                glScalef(l, 0.9, 0.6);
                glColor4f(0.0, 1.0, 0.0, 0.5);
                #ifdef DRAWSOLID
                    glutSolidCube(1.);
                #else
                    glutWireCube(1.);
                #endif
            glPopMatrix();
        glTranslatef(l/2, 0., 0.);
        }
        // retangulo representando o movimento do antebraço (parte inferior do braço)
        glRotatef((GLfloat) tetas[ANTEBRACO], 0., 0., 1.);
        glTranslatef(1.5, 0., 0.);
            glPushMatrix();
                glScalef(3.0, 0.9, 0.6);
                glColor4f(0.0, 0.0, 0.5, 0.5);
                #ifdef DRAWSOLID
                    glutSolidCube(1.);
                #else
                    glutWireCube(1.);
                #endif
            glPopMatrix();
        // opcionais - desenho da mão e dedos
        #ifdef HAND
        glTranslatef(1., 0., 0.);
        glRotatef((GLfloat) tetas[MAO], 1., 0., 0.);
        glTranslatef(1.25, 0., 0.);
            glPushMatrix();
                glScalef(1.5, 0.6, 1.8);
                glColor4f(0.0, 0.0, 0.8, 0.5);
                #ifdef DRAWSOLID
                    glutSolidCube(1.);
                #else
                    glutWireCube(1.);
                #endif
            glPopMatrix();
            #ifdef FINGER
                glColor4f(0.0, 0.0, 1.0, 0.5);
                glTranslatef(0.75, 0., 0.);
 
                // dedos da mão (1)
                glPushMatrix();
                    glRotatef((GLfloat) tetas[DEDOS], 0., 0., 1.);
                    glTranslatef(0.6, 0., -0.7);
                    glScalef(1.2, 0.2, 0.2);
                    #ifdef DRAWSOLID
                        glutSolidCube(1.);
                    #else
                        glutWireCube(1.);
                    #endif
                glPopMatrix();
 
                // dedos da mão (2)
                glPushMatrix();
                    glRotatef((GLfloat) tetas[DEDOS], 0., 0., 1.);
                    glTranslatef(0.6, 0., -0.23);
                    glScalef(1.2, 0.2, 0.2);
                    #ifdef DRAWSOLID
                        glutSolidCube(1.);
                    #else
                        glutWireCube(1.);
                    #endif
                glPopMatrix();
                // dedos da mão (3)
 
                glPushMatrix();
                    glRotatef((GLfloat) tetas[DEDOS], 0., 0., 1.);
                    glTranslatef(0.6, 0., 0.27);
                    glScalef(1.2, 0.2, 0.2);
                    #ifdef DRAWSOLID
                        glutSolidCube(1.);
                    #else
                        glutWireCube(1.);
                    #endif
                glPopMatrix();
 
                // dedos da mão (4)
                glPushMatrix();
                    glRotatef((GLfloat) tetas[DEDOS], 0., 0., 1.);
                    glTranslatef(0.6, 0., 0.75);
                    glScalef(1.2, 0.2, 0.2);
                    #ifdef DRAWSOLID
                        glutSolidCube(1.);
                    #else
                        glutWireCube(1.);
                    #endif
                glPopMatrix();
                // dedão
                glRotatef( 90., 0., 1., 0.);
                glTranslatef(-1.2, 0.1, -0.5);
                glRotatef((GLfloat) tetas[DEDOS], 1., 0., -1.);
                glPushMatrix();
                    glScalef(0.9, 0.3, 0.3);
                    #ifdef DRAWSOLID
                        glutSolidCube(1.);
                    #else
                        glutWireCube(1.);
                    #endif
                glPopMatrix();
            #endif
        #endif
    glPopMatrix();
}
 
void display() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    draw();
    glFlush();
    glutSwapBuffers();
}
 
void Timer(int value) {
 
   TempoDecorrido += deltaT/1000.0;
   if (TempoDecorrido > tAnima)
      TempoDecorrido = tAnima;
 
    for (int i = 0; i < value; i++) {
        tetas[i] = (int) (inicial_tetas[i] + TempoDecorrido*(final_tetas[i]/tAnima)) % 360;
        #ifdef LIMITTHETA
            tetas[i] = MAX(tetas[i], TetaMIN);
            tetas[i] = MIN(tetas[i], TetaMAX);
        #endif
    }
   glutPostRedisplay();
 
   if (TempoDecorrido < tAnima)
      glutTimerFunc(deltaT,Timer, value);
   else {
        #ifndef KEEPALIVE
            exit (0);
        #endif
   }
}
 
void reshape(GLsizei width, GLsizei height) {
    glViewport(0, 0, width, height);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glFrustum (-1.0, 1.0, -1.0, 1.0, 1.5, 50.0);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}
 
void specialKeyboard (int key, int x, int y) {
    GLfloat dVar = 0.5, aVar = 3 * M_PI / 180.0;
    switch (key) {
        // gira a camera do observador para direita
        case GLUT_KEY_RIGHT:
            camAng += aVar;
        break;
        // gira a camera do observador para esquerda
        case GLUT_KEY_LEFT:
            camAng -= aVar;
        break;
        // afasta a camera do observador
        case GLUT_KEY_DOWN:
            camDis += dVar;
        break;
        // aproxima a camera do observador
        case GLUT_KEY_UP:
            camDis = max((GLfloat)3.0, camDis - dVar);
        break;
        default: break;
    }
    glutPostRedisplay();
}
 
void keyboard (unsigned char key, int x, int y) {
    GLfloat move = 0.5;
    if(key == 27) exit(0);
    switch (key) {
        //tecla "a" move o viewer na direção negativa do eixo Z
        case 'a': case 'A':
            camCentZ += move;
        //tecla "d" move o viewer na direção negativa do eixo Z
        case 'd': case 'D':
            camCentZ -= move;
        //tecla "s" move o viewer para baixo
        case 's': case 'S':
            camCentY += move;
        break;
        //tecla "w" move o viewer para cima
        case 'w': case 'W':
            camCentY -= move;
        break;
        //seta da esquerda "<" move o viewer para esquerda
        case ',': case '<':
            camCentX += move;
        break;
        //seta da direita ">" move o viewer para direita
        case '.': case '>':
            camCentX -= move;
        break;
        default: break;
    }
    glutPostRedisplay();
}
 
void init() {
    #ifdef WHITEBG
        glClearColor(1.0, 1.0, 1.0, 0.0);
    #else
        glClearColor(0.0, 0.0, 0.0, 0.0);
    #endif
    glShadeModel(GL_FLAT);
}
 
int main(int argc, char * argv[]) {
    int args = 10;
    #ifndef MANUALINPUT
        #ifdef HAND
            args += 2;
            #ifdef FINGER
                args += 2;
            #endif
        #endif
        if (argc >= args) {
            cout << "angulos finais e iniciais:";
            for (int i = 0; i < 4; i++) {
                tetas[i] = inicial_tetas[i] =  atof (argv[(i+1)]);
                final_tetas[i] = atof (argv[(i+5)]);
                cout << "theta " << (i+1) << " (" << inicial_tetas[i] << "," << final_tetas[i] << ")";
            }
            l = atof (argv[9]);
            l = ( (l > 2) ? 2.0 : ((l < 0) ? 0: l));
            cout << endl << "comprimento de L definido como: " << l << endl;
 
            #ifdef HAND
                tetas[4] = inicial_tetas[4] =  atof (argv[(10)]);
                final_tetas[4] = atof (argv[11]);
                #ifdef FINGER
                    tetas[5] = inicial_tetas[5] =  atof (argv[(12)]);
                    final_tetas[5] = atof (argv[13]);
                #endif
            #endif
        }
        else {
          cout << "Você enviou " << (argc-1) << " parâmetros, enquanto essa execução necessita de " << (args-1) << "." << endl << endl;
          cout << "O programa necessita de 9 parâmetros para sua execução, são eles:" << endl << "teta 1, 2, 3 e 4 iniciais;" << endl << "teta 1, 2 ,3 e 4 finais;" << endl << "o comprimento de L (0<=L<=2)." << endl;
          cout << endl << "O programa ainda conta com opções adicionais para desenhar a mão e os dedos do braço mecânico, cada uma utilizando mais dois inputs:" << endl << "os angulos iniciais e finais das suas rotações." << endl;
          cout << endl << "Compile usando 'MANUALINPUT' para permitir recepção das variaveis em execução" << endl << endl;
          exit (-1);
       }
   #else
        cout << "digite angulos iniciais:" << endl;
        for (int i = 0; i < 4; i++) {
               (cin >> inicial_tetas[i], tetas[i] = inicial_tetas[i]);
        }
        cout << endl << "digite angulos finais:" << endl;
        for (int i = 0; i < 4; i++) cin >> final_tetas[i];
        cout << endl << "digite o comprimento de l:" << endl;
        cin >> l;
        l = ( (l > 2) ? 2.0 : ((l < 0) ? 0: l));
 
        #ifdef HAND
            cout << "digite os angulos inicial e final da mão: " << endl;
            cin >> inicial_tetas[4] >> final_tetas[4];
            tetas[4] = inicial_tetas[4];
            #ifdef FINGER
                cout << "digite os angulos inicial e final dos dedos: " << endl;
                cin >> inicial_tetas[5] >> final_tetas[5];
                tetas[5] = inicial_tetas[5];
            #endif
        #endif
   #endif
 
    glutInit(&argc, argv);
    Display* disp = XOpenDisplay(NULL);
    Screen*  scrn = DefaultScreenOfDisplay(disp);
 
    glutInitDisplayMode (GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    int height = scrn->height, width  = scrn->width;
    glutInitWindowSize (ceil(width*0.9), ceil(height*0.9));
    glutCreateWindow ("Computacao Grafica - Braco Mecanico");
 
    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    #ifdef KEYBOARD
        glutKeyboardFunc(keyboard);
        glutSpecialFunc(specialKeyboard);
    #endif
 
    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);
 
    #ifdef LIGHT
        glEnable(GL_LIGHT0);
        glEnable(GL_LIGHTING);
        glLightfv(GL_LIGHT0, GL_AMBIENT,  light_ambient);
        glLightfv(GL_LIGHT0, GL_DIFFUSE,  light_diffuse);
        glLightfv(GL_LIGHT0, GL_SPECULAR, light_specular);
        glLightfv(GL_LIGHT0, GL_POSITION, light_position);
    #endif
 
    #ifdef MATERIAL
        glEnable(GL_NORMALIZE);
        glEnable(GL_COLOR_MATERIAL);
        glMaterialfv(GL_FRONT, GL_AMBIENT,   mat_ambient);
        glMaterialfv(GL_FRONT, GL_DIFFUSE,   mat_diffuse);
        glMaterialfv(GL_FRONT, GL_SPECULAR,  mat_specular);
        glMaterialfv(GL_FRONT, GL_SHININESS, high_shininess);
    #endif
 
    args = 4;
    #ifdef HAND
        args++;
        #ifdef FINGER
            args ++;
        #endif
    #endif
    glutTimerFunc(deltaT,Timer, args);
    init();
    glutMainLoop();
}