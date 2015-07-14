/* 
  Open-GL program using Cg for using simple Cg shaders
  by Daniel Jungblut, IWR Heidelberg University, February 2008
  revised by Susanne Kroemker, IWR Heidelberg University, April 2014
  based on example code of Cg Tutorial (Addison-Wesley, ISBN
  0321194969) by Randima Fernando and Mark J. Kilgard.
*/
/*
Periodicity

if we applying texture to the objects like sphere, or want to repeat the same texture several times on the same object, 
it's left and right side should be the same, otherwise rendered object wont look nice
in enviremont mapping we use sphere or a cube that represents 'world' to map enviroment texture on it, 
then texture from it is calculated on objects in our scene, hence if we rotate them then they will reflect same part of enviroment texture
*/
#include <math.h>
#include <cstdlib>
#include <stdio.h>

#include <GL/glut.h>
#include <Cg/cg.h>
#include <Cg/cgGL.h>

#include "util/arcball.h"

bool fullscreen = false;
bool mouseDown = false;
 
float xrot = 0.0f;
float yrot = 0.0f;
 
float xdiff = 0.0f;
float ydiff = 0.0f;

int _mode = 0;

// global variables for handling the arcball

// screen size
int screenWidth = 800;
int screenHeight = 600;

// last and current y mouse position for zooming
int last_y = 0;
int current_y = 0;

// arcball activated or not
bool arcball_on = false;

// zoom activated or not
bool zoom_on = false;
float zoomValue = 0.f;

// initialize arcball 
static gl::ArcBall arcball;


  int w = 600;//1600;
  int h = 300;//800;

//GLuint texture_id;
//GLubyte* texture_id;
GLuint texture_int = 0;


GLuint loadPNGTexture (const char *filename);

// Globale Variablen:


GLuint texture_id = 0;

bool wireframe = false;

void reshape( GLint width, GLint height );
void draw_plane(float unit, int n); 
void mouse( int button, int state, int x, int y );
void motion( int x, int y );



void draw_sun()
{
  static const GLfloat light_pos[]        = {4.0f, 4.0f, 4.0f, 1.0f};
  static const GLfloat sun_color[]        = {1.0f, 1.0f, 0.0f};
  static const GLfloat sun_emissive[]     = {1.0f, 1.0f, 0.0f};
  static const GLfloat not_emissive[]     = {0.0f, 0.0f, 0.0f};

  glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, sun_color);
  glMaterialfv(GL_FRONT_AND_BACK, GL_EMISSION, sun_emissive);
  glPushMatrix();
    glLightfv(GL_LIGHT0, GL_POSITION, light_pos);
    glTranslatef(light_pos[0], light_pos[1], light_pos[2]);
    glutSolidSphere( 0.2f, 40, 40 );
    glMaterialfv(GL_FRONT_AND_BACK, GL_EMISSION, not_emissive);
  glPopMatrix();
}



// keyboard callback:
void keyboard(unsigned char key, int x, int y) {
  switch (key) {
  case 27: 		// Escape
  case 'q':

    glDeleteTextures(1, &texture_id);
    exit(0);
  break;
  case ' ':
    wireframe = !wireframe;
    if(wireframe) glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    else glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    glutPostRedisplay();
  break;
  case 't':
    if (_mode==0)
      _mode=1;
    else
      _mode=0;
  break;
  }
}


// display function:
void display() {
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  glLoadIdentity();
  gluLookAt(0, 0, 20 + zoomValue, 0, 0, 0, 0, 1, 0);
 // glMultMatrixf( arcball.get() );
  draw_sun();
  if (_mode==1)
    glEnable(GL_TEXTURE_2D);


  glTexGeni(GL_S, GL_TEXTURE_GEN_MODE, GL_SPHERE_MAP);
  glTexGeni(GL_T, GL_TEXTURE_GEN_MODE, GL_SPHERE_MAP);
  glEnable(GL_TEXTURE_GEN_S);
  glEnable(GL_TEXTURE_GEN_T);
  glBindTexture(GL_TEXTURE_2D, texture_id);



  GLfloat shininess[] = {51.2};
  GLfloat ambientColor[] = {0.19225, 0.19225, 0.19225, 1.0f};
  GLfloat diffuseColor[] = {0.50754, 0.50754, 0.50754, 1.0f};
  GLfloat specularColor[] = {0.508273, 0.508273, 0.508273, 1.0f};

  glMaterialfv(GL_FRONT_AND_BACK, GL_SHININESS, shininess);
  glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, ambientColor);
  glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, diffuseColor);
  glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, specularColor);

 
  glRotatef(xrot, 1.0f, 0.0f, 0.0f);
  glRotatef(yrot, 0.0f, 1.0f, 0.0f);  


  glPushMatrix();
  glTranslatef( -4.0f, 0.0f, 0.0f );
  glutSolidSphere( 3, 40, 40 );
  glTranslatef( 8.0f, 0.0f, 0.0f );
  glutSolidTeapot(3.0f); 
  glPopMatrix();


  glDisable(GL_TEXTURE_2D);

  glutPostRedisplay();
  glutSwapBuffers();
}

void reshape( GLint width, GLint height )
{
  // set new window size for arcball when reshaping
  arcball.set_win_size( width, height );

  glMatrixMode( GL_PROJECTION );
  glLoadIdentity();
  gluPerspective( 45, 1.0 * width / height, 1, 1000 );
   
  glViewport( 0, 0, width, height );

  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();
  gluLookAt( 0, 0, 100, 0, 0, 0, 0, 1, 0 );
}

void mouse( int button, int state, int x, int y )
{
  // if the left mouse button is pressed
  if( button == GLUT_LEFT_BUTTON && state == GLUT_DOWN ) {
    // set use of arcball to true 
    arcball_on = true;
    // set use of zooming to false
    zoom_on = false;
    // store current mouse position
    arcball.set_cur( x, y );
    // and begin drag of ball
    arcball.begin_drag();
  }
  // else if right button is pressed
  else if((button == GLUT_RIGHT_BUTTON) && (state == GLUT_DOWN) && (zoom_on == false)) {
    // set use of zooming to true
    zoom_on = true;
    // store y values for zooming
    last_y = y;
  }
  // else set all to false and end drag of ball
  else {
    zoom_on = false;
    // set use of arcball to false 
    arcball_on = false;
    // and end drag of ball
    arcball.end_drag();
  }
}

void motion( int x, int y )
{
  // id arcball is activated
  if( arcball_on ) {
    // store current x and y mouse positions
    arcball.set_cur( x, y );
  }

  // if zooming is activated
  if( zoom_on ) {
    // store current y mouse position for zooming
    current_y = y;
    // prevent from zooming too close and thereby inverting the space
      if ((zoomValue > -17.0f) && (zoomValue < 80.0f))
      zoomValue = zoomValue + 0.1f * (last_y - current_y);
    // the eye position is fixed at (0,0,3), see gluLookAt( 0,0,20+zoomValue, ...) 
      else if (zoomValue <= -17.0f)
        zoomValue = -16.9999f;
    // the eye position is fixed at (0,0,100), see gluLookAt( 0,0,20+zoomValue, ...) 
      else 
        zoomValue = 79.9999f;
    last_y = current_y;
  }
}

void init()
{
  glClearColor (0.7f, 0.7f, 1.0f, 0.0f);
  glLightModeli(GL_LIGHT_MODEL_TWO_SIDE,0);
  glEnable(GL_LIGHTING);
  glEnable(GL_LIGHT0);
  glEnable(GL_DEPTH_TEST);
  // enable the manipulation of the diffuse material component by glColor();
  glEnable(GL_COLOR_MATERIAL);
}

void idle() {
  if (!mouseDown) {
    xrot += 0.3f;
    yrot += 0.4f;
  }
}

void mouse2(int button, int state, int x, int y)
{
  if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {
    mouseDown = true;
 
    xdiff = x - yrot;
    ydiff = -y + xrot;
  }
  else
    mouseDown = false;
}
 
void motion2(int x, int y) {
  if (mouseDown) {
    yrot = x - xdiff;
    xrot = y + ydiff;
 
    glutPostRedisplay();
  }
}


int main(int argc, char **argv) {

  glutInitWindowSize(800, 600);
  glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH);
  glutInit(&argc, argv);

  glutCreateWindow("Shaders and textures");
  init();
  glutDisplayFunc(display);
  glutKeyboardFunc(keyboard);

  glClearColor(0.1, 0.2, 0.8, 1.0); 

  //texture_int = (GLubyte*) loadBMP("sand.bmp", &w, &h);
  texture_id = loadPNGTexture("Planet_7_d.png");

  glutReshapeFunc( reshape );
  glutMouseFunc( mouse2 );
  glutMotionFunc( motion2 );
  

  glutMainLoop();
  return 0;
}
