#include <iostream>
//#include <GL/glut.h>
#include <GL/freeglut.h>
#include <math.h>
#include <SOIL2.h>
#include <string>
#include <filesystem>
#include <direct.h>
#include "table.h"
#include "lamp.h"
#define GetCurrentDir _getcwd

using namespace std;

// vertices for the cube
GLfloat x = 20.0f;
GLfloat y = 10.0f;
GLfloat z = 20.0f;

// variables to move outermost Object Frame (to move all the rendered environment)
GLfloat moveX = 0.0f;
GLfloat moveY = 0.0f;
GLfloat moveZ = 0.0f;

// variables to rotate outermost Object Frame (to move all the rendered environment)
GLfloat rotX = 0.0f;
GLfloat rotY = 0.0f;
GLfloat rotZ = 0.0f;

//variables to move the camera
GLfloat camY = 0.0f;
GLfloat camX = 0.0f;
GLfloat camZ = 0.0f;

//main light
GLfloat   lightPosition[] = { 5, 5, 0, 1.0 };
GLfloat   lightDirection[] = { 0, 0, 10, 0 };
GLfloat   diffuseLight[] = { 0.7, 0.7, 0.7, 1.0 };
GLfloat   ambientLight[] = { 0.4, 0.4, 0.4, 1.0 };

//light 1
GLfloat L1_Ambient[] = { 0.5, 0.5, 0.5, 1.0 };
GLfloat L1_Diffuse[] = { 1, 1, 1, 5.0 };
GLfloat L1_Specular[] = { 1.0, 1.0, 0.0, 10.0 };   //Declaration of the specular component of the light_1
GLfloat L1_postion[] = { 0, -3, 0, 1.0 };
GLfloat L1_direction[] = { 0, -1, 0, 0 };

//light 2 (wall lamp1)
GLfloat L2_Ambient[] = { 0.2, 0.2, 0.2, 1.0 };
GLfloat L2_Diffuse[] = { 0.6, 0.6, 0.6, 1.0 };
GLfloat L2_Specular[] = { 0.3, 0.3, 0.3, 1.0 };   //Declaration of the specular component of the light_2
GLfloat L2_postion[] = { 10, 25, -20, 1.0 };
GLfloat L2_direction[] = { 0, 0, 0 };

GLfloat globalAmbient[] = { 0.8, 0.8, 0.8, 1.0 };

//init light
void initLight() {
    glLightfv(GL_LIGHT0, GL_AMBIENT, ambientLight);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, diffuseLight);
    glLightfv(GL_LIGHT0, GL_POSITION, lightPosition);
    glLightf(GL_LIGHT0, GL_SPOT_CUTOFF, 180);
    glLightf(GL_LIGHT0, GL_SPOT_EXPONENT, 128);
    glLightfv(GL_LIGHT0, GL_SPOT_DIRECTION, lightDirection);

    //glLightfv(GL_LIGHT1, GL_AMBIENT, L1_Ambient);
    glLightfv(GL_LIGHT1, GL_DIFFUSE, L1_Diffuse);
    glLightfv(GL_LIGHT1, GL_SPECULAR, L1_Specular);
    glLightfv(GL_LIGHT1, GL_POSITION, L1_postion);
    //glLightf(GL_LIGHT1, GL_SPOT_CUTOFF, 90);
    glLightf(GL_LIGHT1, GL_SPOT_EXPONENT, 128);
    glLightf(GL_LIGHT2, GL_SHININESS, 128);
    //glLightfv(GL_LIGHT1, GL_SPOT_DIRECTION, L1_direction);

    glLightfv(GL_LIGHT2, GL_DIFFUSE, L2_Diffuse);
    glLightfv(GL_LIGHT2, GL_SPECULAR, L2_Specular);
    glLightfv(GL_LIGHT2, GL_POSITION, L2_postion);
    glLightfv(GL_LIGHT2, GL_SPOT_DIRECTION, L2_direction);
    glLightf(GL_LIGHT2, GL_SPOT_CUTOFF, 45.0);
    glLightf(GL_LIGHT2, GL_SPOT_EXPONENT, 2);
    //glLightf(GL_LIGHT2, GL_CONSTANT_ATTENUATION, 1.5);
    //glLightf(GL_LIGHT2, GL_LINEAR_ATTENUATION, 0.5);
    //glLightf(GL_LIGHT2, GL_QUADRATIC_ATTENUATION, 0.1);

    //Declaration of the ligt reflecting properties for the materials
    GLfloat specularReflectance[] = { 1.0, 1.0, 1.0, 1.0 };
    glMaterialfv(GL_FRONT, GL_SPECULAR, specularReflectance);
    glMateriali(GL_FRONT, GL_SHININESS, 50);
}

void drawAxes() {

    glBegin(GL_LINES);

    glLineWidth(1.5);

    glColor3f(1.0, 0.0, 0.0);
    glVertex3f(-20, 0, 0);
    glVertex3f(20, 0, 0);

    glColor3f(0.0, 1.0, 0.0);
    glVertex3f(0, -20, 0);
    glVertex3f(0, 20, 0);

    glColor3f(0.0, 0.0, 1.0);
    glVertex3f(0, 0, -20);
    glVertex3f(0, 0, 20);

    glEnd();
}
void DrawGrid() {
    GLfloat ext = 20.0f;
    GLfloat step = 1.0f;
    GLfloat yGrid = -0.4f;
    GLint line;

    glBegin(GL_LINES);
    for (line = -ext; line <= ext; line += step) {
        glVertex3f(line, yGrid, ext);
        glVertex3f(line, yGrid, -ext);

        glVertex3f(ext, yGrid, line);
        glVertex3f(-ext, yGrid, line);
    }
    glEnd();
}

//texture image files
const char* image_files[8] = {
    "C:/work/CS308/Project/ProjectLivingRooom/Textures/bricks.jpg",
    "C:/work/CS308/Project/ProjectLivingRooom/Textures/ceiling.jpg",
    "C:/work/CS308/Project/ProjectLivingRooom/Textures/floor.jpg",
    "C:/work/CS308/Project/ProjectLivingRooom/Textures/wall_white.jpg",
    "C:/work/CS308/Project/ProjectLivingRooom/Textures/wall_grey.jpg",
    "C:/work/CS308/Project/ProjectLivingRooom/Textures/wall_white_brick.png",
    "C:/work/CS308/Project/ProjectLivingRooom/Textures/wall_matt_butter.jpg",
    "C:/work/CS308/Project/ProjectLivingRooom/Textures/wall_lamp.jpg"
};

//texture func 1
GLuint texture[8];

void loadTexture() {
    for (int i = 0; i < 8; i++) {
        texture[i] = SOIL_load_OGL_texture(image_files[i], SOIL_LOAD_RGBA, SOIL_CREATE_NEW_ID, 0);
        //SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_MIPMAPS | SOIL_FLAG_INVERT_Y | SOIL_FLAG_NTSC_SAFE_RGB | SOIL_FLAG_COMPRESS_TO_DXT

        glBindTexture(GL_TEXTURE_2D, texture[i]);

        glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    }
}

//texture func 2
/*std::string get_current_dir() {
    char buff[FILENAME_MAX]; //create string buffer to hold path
    GetCurrentDir(buff, FILENAME_MAX);
    string current_working_dir(buff);
    return current_working_dir;
}

void loadTexture() {

    for (int i = 0; i < 7; i++) {

        int width, height;
        
        unsigned char* image = SOIL_load_image(image_files[i], &width, &height, 0, SOIL_LOAD_RGB);

        if (!image) {
            std::cout << "Failed to load texture: " << sizeof(image) << std::endl;
        }
        else {
            std::cout << i << " " << &image << std::endl;
        }

        unsigned char data[] =
        {
            128, 128, 128, 255,
            255, 0, 0, 255,
            0, 255, 0, 255,
            0, 0, 255, 255
        };

        glGenTextures(1, &texture[i]);
        glBindTexture(GL_TEXTURE_2D, texture[i]);


        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);

        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
    }  

}*/

void drawWalls() {
    //glTranslatef(0.0, 1.6, 0.0);

    //texture param

    // BACK
    //texture
    glBindTexture(GL_TEXTURE_2D, texture[5]);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

    glBegin(GL_QUADS);
    glTexCoord3f(x, y, z);
        glVertex3f(x, y, z);
    glTexCoord3f(x, -y, z);
        glVertex3f(x, -y, z);
    glTexCoord3f(-x, -y, z);
        glVertex3f(-x, -y, z);
    glTexCoord3f(-x, y, z);
        glVertex3f(-x, y, z);
    glEnd();
    // END BACK

    // FRONT
    //texture
    glBindTexture(GL_TEXTURE_2D, texture[3]);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

    glBegin(GL_QUADS);
    glTexCoord3f(x, y, -z);
        glVertex3f(x, y, -z);
    glTexCoord3f(-x, y, -z);
        glVertex3f(-x, y, -z);
    glTexCoord3f(-x, -y, -z);
        glVertex3f(-x, -y, -z);
    glTexCoord3f(x, -y, -z);
        glVertex3f(x, -y, -z);
    glEnd();
    // END FRONT

    // LEFT
    //texture
    //glBindTexture(GL_TEXTURE_2D, tex_array[3]);
    //glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    //glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

    glBegin(GL_QUADS);
    glTexCoord3f(-x, -y, -z);
        glVertex3f(-x, -y, -z);
    glTexCoord3f(-x, y, -z);
        glVertex3f(-x, y, -z);
    glTexCoord3f(-x, y, z);
        glVertex3f(-x, y, z);
    glTexCoord3f(-x, -y, z);
        glVertex3f(-x, -y, z);
    glEnd();
    // LEFT END

    // RIGHT
    //texture
    //glBindTexture(GL_TEXTURE_2D, tex_array[3]);
    //glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    //glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

    glBegin(GL_QUADS);
    glTexCoord3f(x, y, z);
        glVertex3f(x, y, z);
    glTexCoord3f(x, -y, z);
        glVertex3f(x, -y, z);
    glTexCoord3f(x, -y, -z);
        glVertex3f(x, -y, -z);
    glTexCoord3f(x, y, -z);
        glVertex3f(x, y, -z);
    glEnd();
    // RIGHT END

    // TOP
    //texture
    glBindTexture(GL_TEXTURE_2D, texture[1]);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

    glBegin(GL_QUADS);
    glTexCoord3f(x, y, -z);
        glVertex3f(x, y, -z);
    glTexCoord3f(x, y, z);
        glVertex3f(x, y, z);
    glTexCoord3f(-x, y, z);
        glVertex3f(-x, y, z);
    glTexCoord3f(-x, y, -z);
        glVertex3f(-x, y, -z);
    glEnd();
    // END TOP

    //BOTTOM
    //texture
    glBindTexture(GL_TEXTURE_2D, texture[4]);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

    glBegin(GL_QUADS);
    glTexCoord3f(x, -y, z);
        glVertex3f(x, -y, z);
    glTexCoord3f(x, -y, -z);
        glVertex3f(x, -y, -z);
    glTexCoord3f(-x, -y, -z);
        glVertex3f(-x, -y, -z);
    glTexCoord3f(-x, -y, z);
        glVertex3f(-x, -y, z);
    glEnd();
    // END BOTTOM
}

void drawTable() {
    Table mainTable;
    glPushMatrix();
    glTranslatef(2, -4, -4);
    glScalef(2.0f, 2.0f, 2.0f);
    //glRotatef(-30.0, 0.0, 1.0, 0.0);
    mainTable.drawTable();
    glPopMatrix();
}

void drawTableLamp() {
    Lamp tableLamp;
    glPushMatrix();
    glTranslatef(0, -3, -4);
    glScalef(3.0f, 3.0f, 3.0f);
    //glRotatef(-30.0, 0.0, 1.0, 0.0);
    tableLamp.drawTableLamp();
    glPopMatrix();
}

void drawWallLamp1() {
    Lamp wallLamp;
    glPushMatrix();
    glTranslatef(15, 6, -19);
    glScalef(1.7f, 1.7f, 1.7f);
    //glRotatef(-30.0, 0.0, 1.0, 0.0);
    wallLamp.drawWallLamp(texture[7]);
    glPopMatrix();
}

void drawWallLamp2() {
    Lamp wallLamp;
    glPushMatrix();
    glTranslatef(-15, 6, -19);
    glScalef(1.7f, 1.7f, 1.7f);
    //glRotatef(-30.0, 0.0, 1.0, 0.0);
    wallLamp.drawWallLamp(texture[7]);
    glPopMatrix();
}

void init() {
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glEnable(GL_DEPTH_TEST);

    //enable lighing
    glEnable(GL_LIGHTING);
    //glEnable(GL_LIGHT0);
    glShadeModel(GL_SMOOTH);
    initLight();

    (GL_FRONT, GL_AMBIENT_AND_DIFFUSE);

    // Enabling the color tracking of each faces of the materials. this enables the color visibility of the materials
    glColorMaterial(GL_FRONT, GL_AMBIENT_AND_DIFFUSE);
    glEnable(GL_COLOR_MATERIAL);

    glEnable(GL_NORMALIZE);

    //glLightModelfv(GL_LIGHT_MODEL_AMBIENT, globalAmbient);

    //texture
    glEnable(GL_TEXTURE_2D);
    loadTexture();

}

void display() {

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


    glPushMatrix();

    // camera orientation (eyeX, eyeY, eyeZ, centerX, centerY, centerZ, upX, upY, upZ)
    gluLookAt(0.0 + camX, 8.0 + camY, 8.0 + camZ, 0, 5, 0, 0, 1.0, 0);
    //initLight();

    // move the object frame using keyboard keys
    glTranslatef(moveX, moveY, moveZ);
    glRotatef(rotX, 1.0f, 0.0f, 0.0f);
    glRotatef(rotY, 0.0f, 1.0f, 0.0f);
    glRotatef(rotZ, 0.0f, 0.0f, 1.0f);


    glColor3f(1.0, 1.0, 1.0);

    DrawGrid();

    //walls, floor and ceiling
    glPushMatrix();
    glTranslatef(0.0, 10.0, 0.0);

    //glEnable(GL_TEXTURE_2D);
    //glBindTexture(GL_TEXTURE_2D, tex_array[1]);
    drawWalls();
    //glDisable(GL_TEXTURE_2D);
    drawTable();
    drawTableLamp();

    drawWallLamp1();
    drawWallLamp2();

    glPopMatrix();

    drawAxes();
    glPopMatrix();

    glutSwapBuffers();

}

void keyboardSpecial(int key, int x, int y) {
    if (key == GLUT_KEY_UP)
        moveZ += 1;

    if (key == GLUT_KEY_DOWN)
        moveZ -= 1;

    if (key == GLUT_KEY_LEFT)
        rotY -= 5.0;

    if (key == GLUT_KEY_RIGHT)
        rotY += 1.0;

    glutPostRedisplay();
}

void keyboard(unsigned char key, int x, int y) {
    if (key == 'w')
        camY += 0.5;
    if (key == 's')
        camY -= 0.5;

    if (key == 'd')
        camX += 0.5;
    if (key == 'a')
        camX -= 0.5;

    if (key == 'c')
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    if (key == 'C')
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

    if (key == ')')
        glDisable(GL_LIGHT0);
    if (key == '0')
        glEnable(GL_LIGHT0);

    if (key == '!')
        glDisable(GL_LIGHT1);
    if (key == '1')
        glEnable(GL_LIGHT1);

    if (key == '@')
        glDisable(GL_LIGHT2);
    if (key == '2')
        glEnable(GL_LIGHT2);

    glutPostRedisplay();

}

void Timer(int x) {
    //animateRotation += animateRotation >= 360.0 ? -animateRotation : 5;
    glutPostRedisplay();

    glutTimerFunc(60, Timer, 1);
}

void changeSize(GLsizei w, GLsizei h) {
    glViewport(0, 0, w, h);
    GLfloat aspect_ratio = h == 0 ? w / 1 : (GLfloat)w / (GLfloat)h;

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    //Define the Perspective projection frustum 
    // (FOV_in_vertical, aspect_ratio, z-distance to the near plane from the camera position, z-distance to far plane from the camera position)
    gluPerspective(120.0, aspect_ratio, 1.0, 100.0);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();


}

int main(int argc, char** argv) {

    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_DEPTH | GLUT_RGBA);
    glutInitWindowSize(800, 600);
    glutInitWindowPosition(250, 250);
    glutCreateWindow("Living Room");
    glutDisplayFunc(display);
    glutReshapeFunc(changeSize);

    // keyboard function activation
    glutKeyboardFunc(keyboard);
    glutSpecialFunc(keyboardSpecial);

    //glutTimerFunc(60.0, Timer, 1);
    init();
    glutMainLoop();


    return 0;
}