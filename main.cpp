#include <GL/glut.h>
#include <GL/GL.h>
#include <GL/GLU.h>
#include <GLFW/glfw3.h>
#include <Windows.h>
#include <cmath>
#include <iostream>
#include "STB_image.h"

using namespace std;

const float M_PI = 3.14159265358979323846f;
// Ângulo de rotação do planeta
GLfloat angle_planet = 0.0f;
// Ângulo de rotação da lua 1
GLfloat angle_moon = 0.0f;
// Ângulo de rotação da lua 2
GLfloat angle_moon2 = 0.0f;

// Velocidade de translação do planeta
GLfloat SPEED_PLANET = 0.5f;
// Velocidade de translação da lua 1
GLfloat SPEED_MOON = 1.0f;
// Velocidade de translação da lua 2
GLfloat SPEED_MOON2 = 3.0f;

//raio do sol
GLdouble sol = 0.2f;
//raio do planeta
GLdouble planeta = 0.1f;
//raio da lua 1
GLdouble lua1 = 0.05f;
//raio da lua 2
GLdouble lua2 = 0.03f;

GLuint NumText;


void carregaTextura(GLuint tex_id, string filePath)
{
    unsigned char* imagem;
    int largura, altura, canais;

    stbi_set_flip_vertically_on_load(true);
    imagem = stbi_load(filePath.c_str(), &largura, &altura, &canais, 4);
    if (imagem)
    {
        glBindTexture(GL_TEXTURE_2D, tex_id);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, largura, altura, 0, GL_RGBA, GL_UNSIGNED_BYTE, imagem);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

        stbi_image_free(imagem);
    }
    else {
        std::cout << "ERRO! Nao foi possivel carregar a imagem " << filePath.c_str() << std::endl;
    }

}


void init() {
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
}

void Esfera(int raio, int longitude, int latitude)
{
 GLUquadricObj* q = gluNewQuadric();
 gluQuadricDrawStyle(q, GLU_FILL);
 gluQuadricNormals(q, GLU_SMOOTH);
 gluQuadricTexture(q, GL_TRUE);
 gluSphere(q, raio, longitude, latitude);
 gluDeleteQuadric(q);
}

void desenha() {
    glClear(GL_COLOR_BUFFER_BIT);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    // Desenha o sol    
    glPushMatrix();
    glRotatef(angle_planet, 1.0f, 1.0f, 0.0f);
    glColor3f(1.0f, 1.0f, 0.0f);
    glutSolidSphere(sol, 20, 20);
    glPopMatrix();
    
    glPushMatrix();
    glEnable(GL_TEXTURE_2D);
    glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
    glGenTextures(1, &NumText);
    carregaTextura(NumText, "imagens/sol.png");
    glBindTexture(GL_TEXTURE_2D, NumText);
    Esfera(sol, 20, 20);
        glBindTexture(GL_TEXTURE_2D, 0);
    glDisable(GL_TEXTURE_2D);
    glRotatef(angle_planet, 1.0f, 1.0f, 0.0f);
    glPopMatrix();

    // Desenha o planeta
    glPushMatrix();
    // Translada para a posição correta em torno do sol
    glTranslatef(0.6f * cos(angle_planet * M_PI / 180.0f), 0.6f * sin(angle_planet * M_PI / 180.0f), 0.0f);
    // Rotaciona em torno do sol
    glRotatef(angle_planet, 0.0f, 0.0f, 1.0f);
    glColor3f(0.0f, 0.0f, 1.0f);
    glutSolidSphere(planeta, 20, 20);
    glEnable(GL_TEXTURE_2D);
    glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
    glGenTextures(1, &NumText);
    carregaTextura(NumText, "imagens/Terra2.png");
    glBindTexture(GL_TEXTURE_2D, NumText);
    Esfera(planeta, 20, 20);
    glBindTexture(GL_TEXTURE_2D, 1);
    glDisable(GL_TEXTURE_2D);
    glRotatef(angle_planet, 0.0f, 0.0f, 1.0f);

    // Desenha a lua 1
    glPushMatrix();
    // Translada para a posição correta em torno do planeta
    glTranslatef(0.2f * cos(angle_moon * M_PI / 180.0f), 0.2f * sin(angle_moon * M_PI / 180.0f), 0.0f);
    // Rotaciona em torno do planeta
    glRotatef(angle_moon, 1.0f, 1.0f, 0.0f);
    glColor3f(0.5f, 0.5f, 0.5f);
    glutSolidSphere(lua1, 20, 20);
    glPopMatrix();

    // Desenha a lua 2
    glPushMatrix();
    // Translada para a posição correta em torno do planeta
    glTranslatef(0.3f * cos(angle_moon2 * M_PI / 180.0f), 0.3f * sin(angle_moon2 * M_PI / 180.0f), 0.0f);
    // Rotaciona em torno do planeta
    glRotatef(angle_moon2, 1.0f, 1.0f, 0.0f);
    glColor3f(0.5f, 0.5f, 0.5f);
    glutSolidSphere(lua2, 20, 20);
    glPopMatrix();

    glPopMatrix();

    // Atualiza os ângulos de rotação
    angle_planet += SPEED_PLANET;
    angle_moon += SPEED_MOON;
    angle_moon2 += SPEED_MOON2;

    glutSwapBuffers();
}

void reshape(int w, int h) {
    glViewport(0, 0, w, h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    if (w <= h)
        glOrtho(-1.0, 1.0, -1.0 * (GLfloat)h / (GLfloat)w, 1.0 * (GLfloat)h / (GLfloat)w, -1.0, 1.0);
    else
        glOrtho(-1.0 * (GLfloat)w / (GLfloat)h, 1.0 * (GLfloat)w / (GLfloat)h, -1.0, 1.0, -1.0, 1.0);
    glMatrixMode(GL_MODELVIEW);
}

void timer(int) {
    glutPostRedisplay();
    glutTimerFunc(16, timer, 0);
}


void menuVelocidade(int op) {
    switch (op){
    case 0: //mais rápido
        SPEED_PLANET += 0.5f;        
        SPEED_MOON += 1.0f;        
        SPEED_MOON2 += 3.0f;
        break;
    case 1: //parar
        SPEED_PLANET = 0.0f;
        SPEED_MOON = 0.0f;
        SPEED_MOON2 = 0.0f;
        break;
    case 2: //rodar
        SPEED_PLANET = 0.5f;
        SPEED_MOON = 1.0f;
        SPEED_MOON2 = 3.0f;
        break;
    default:
        break;
    }
}

void menuZoom(int op) {
    switch (op) {
    case 0: //diminui
        sol /= 0.2;
        planeta /= 0.2;
        lua1 /= 0.2;
        lua2 /= 0.2;
        break;
    case 1: //amplia
        sol *= 0.2;
        planeta *= 0.2;
        lua1 *= 0.2;
        lua2 *= 0.2;
        break;
    default:
        break;
    }
}

void menuPrincipal(int op) {} //menu principal vazio, para gerenciar os submenus


void criaMenu() {
    int menu, submenu1, submenu2, submenu3;

    submenu1 = glutCreateMenu(menuVelocidade);
    glutAddMenuEntry("Aumentar a velocidade", 0);
    glutAddMenuEntry("Parar animação", 1);
    glutAddMenuEntry("Velocidade inicial", 2);

    submenu2 = glutCreateMenu(menuZoom);
    glutAddMenuEntry("Ampliar", 0);
    glutAddMenuEntry("Diminuir", 1);
        
    menu = glutCreateMenu(menuPrincipal);
    glutAddSubMenu("Velocidade", submenu1);
    glutAddSubMenu("Zoom", submenu2);   
    glutAttachMenu(GLUT_RIGHT_BUTTON);
}

void gerenciaMouse(int button, int state, int x, int y) {
    if (button == GLUT_RIGHT_BUTTON) {
        if (state == GLUT_DOWN) {
            criaMenu();
        }
    }
    glutPostRedisplay();
}

void teclado(int key, int x, int y) {
    switch (key) {
    case GLUT_KEY_UP:
        SPEED_PLANET += 0.5f;
        SPEED_MOON += 1.0f;
        SPEED_MOON2 += 3.0f;
        break;
    case GLUT_KEY_DOWN:
        SPEED_PLANET *= 0.5;
        SPEED_MOON *= 0.5;
        SPEED_MOON2 *= 0.5;
        break;
    case GLUT_KEY_LEFT:
        SPEED_PLANET = 0.0f;
        SPEED_MOON = 0.0f;
        SPEED_MOON2 = 0.0f;
        break;
    case GLUT_KEY_RIGHT:
        SPEED_PLANET = 0.5f;
        SPEED_MOON = 1.0f;
        SPEED_MOON2 = 3.0f;
        break;
    default:
        break;
    }
}


int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
    glutInitWindowSize(800, 600);
    glutInitWindowPosition(500, 200);
    glutCreateWindow("Sistema Solar");
    init();
    glutDisplayFunc(desenha);
    glutReshapeFunc(reshape);
    glutMouseFunc(gerenciaMouse);
    glutSpecialFunc(teclado);
    glutTimerFunc(16, timer, 0);
    glutMainLoop();
    return 0;
}