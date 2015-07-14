/*********************************************
  E01 TwoSided -> E05 List
 
  Author:
  Susanne Kroemker, IWR - UNIVERSITAET HEIDELBERG
  Im Neuenheimer Feld 368
  D-69120 Heidelberg
 
  phone +49 (0)6221 54 8883
  fax   +49 (0)6221 54 8850
 
*********************************************/

/* Framerate

framerate seems to be higher for precompiled list, 
it falls down when we switch to the normal - direct method calling;
on that basis we can say that displaying precompiled objects is more efficient
*/

#include <stdio.h>
#include <stdlib.h>

#if defined(__APPLE__) || defined(MACOSX)
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

#include "util/arcball.h"

#define KEY_ESC 27
#define PI 3.14159265
#define MOEBIUS 1


int frameCount = 0;
float fps = 0;
int currentTime = 0, previousTime = 0;
float font_emission[]  = { 0.0, 0.0, 0.0, 1.0 };
int mode = 1;
GLfloat rotate_x, rotate_y, rotate_z;

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

void calculateFPS();
void drawFPS();
void renderText(char *text, float x, float y, float z);
void animateObject();

// method prototypes
void draw_sun();
void display();
void reshape( GLint width, GLint height );
void keyboard( GLubyte key, GLint x, GLint y );
void mouse( int button, int state, int x, int y );
void motion( int x, int y );
void init();
int main( int argc, char** argv );

void animateObject()
{
    //  Rotate the object around the y axis
	rotate_y += 0.5;

	if (rotate_y > 360)
		rotate_y = 0;

    //  Set rotation
	glRotatef (rotate_x, 1, 0, 0);
	glRotatef (rotate_y, 0, 1, 0);
	glRotatef (rotate_z, 0, 0, 1);
}

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

void moebius() {
  GLfloat shininess[] = {30.2};
  GLfloat ambientColor[] = {0.19225, 0.19225, 0.19225, 1.0f};
  GLfloat diffuseColor[] = {0.50754, 0.50754, 0.50754, 1.0f};
  GLfloat specularColor[] = {0.508273, 0.508273, 0.508273, 1.0f}; 
  glEnable(GL_COLOR_MATERIAL);
  
  int n;
  float r, a;
  r=-1;
  a=0;
  struct point{
	float x;
	float y;
	float z;
  }p1, p2;

  glNormal3f( 0.0, 0.0, 1 );
  glFrontFace(GL_CW);

  glMaterialfv(GL_FRONT_AND_BACK, GL_SHININESS, shininess);
  glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, ambientColor);
  glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, diffuseColor);
  glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, specularColor);

  glColor3f( 0.0f, 0.2f, 0.0f );
  //glPolygonMode( GL_FRONT_AND_BACK, GL_LINE );
  glPushMatrix(); 
  glTranslatef( 0.0f, 0.0f, -5.0f );
  for (n=10; n>0; n--){
  glBegin(GL_TRIANGLE_STRIP/*GL_POINTS*/);
   for(a = 0; a < 2*PI; a += 0.2)  //0-2pi
	{
 
		for(r = -0.5; r <= 0.5; r += 0.1)  //-0.5 0.5 //-1 1
		{
				p1.x = cos(a)* ( 1+ (r/2 * cos(a/2)) );
				p1.y = sin(a)* ( 1+ (r/2 * cos(a/2)) );
				p1.z = r/2 *sin(a/2);
 
				glVertex3f(p1.x, p1.y, p1.z); 
						
				a+=0.2;
				p1.x = cos(a)* ( 1+ (r/2 * cos(a/2)) );
				p1.y = sin(a)* ( 1+ (r/2 * cos(a/2)) );
				p1.z = r/2 *sin(a/2);
 
				glVertex3f(p1.x, p1.y, p1.z); 
				a-=0.2;
		}
	}
   glEnd();
   glTranslatef( 0.0f, 0.0f, 1.0f );
   }
   glPopMatrix();
}

void moebius_list() {

  GLfloat shininess[] = {30.2};
  GLfloat ambientColor[] = {0.19225, 0.19225, 0.19225, 1.0f};
  GLfloat diffuseColor[] = {0.50754, 0.50754, 0.50754, 1.0f};
  GLfloat specularColor[] = {0.508273, 0.508273, 0.508273, 1.0f}; 
  
    
  float r, a;
  int n = 5;
  r=-1;
  a=0;
  struct point{
	float x;
	float y;
	float z;
  }p1, p2;

 glNewList(MOEBIUS, GL_COMPILE);

  glMaterialfv(GL_FRONT_AND_BACK, GL_SHININESS, shininess);
  glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, ambientColor);
  glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, diffuseColor);
  glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, specularColor);
  glNormal3f( 0.0, 0.0, 1 );
  glFrontFace(GL_CW);
  glEnable(GL_COLOR_MATERIAL);
  glColor3f( 0.8f, 0.8f, 0.8f );

  //glPolygonMode( GL_FRONT_AND_BACK, GL_LINE );
  glPushMatrix(); 
  glTranslatef( 0.0f, 0.0f, -5.0f );
  for (n=10; n>0; n--){
  glBegin(GL_TRIANGLE_STRIP/*GL_POINTS*/);
   for(a = 0; a < 2*PI; a += 0.2)  //0-2pi
	{
		for(r = -0.5; r <= 0.5; r += 0.1)  //-0.5 0.5 //-1 1
		{
				p1.x = cos(a)* ( 1+ (r/2 * cos(a/2)) );
				p1.y = sin(a)* ( 1+ (r/2 * cos(a/2)) );
				p1.z = r/2 *sin(a/2);
 
				glVertex3f(p1.x, p1.y, p1.z); 
						
				a+=0.2;
				p1.x = cos(a)* ( 1+ (r/2 * cos(a/2)) );
				p1.y = sin(a)* ( 1+ (r/2 * cos(a/2)) );
				p1.z = r/2 *sin(a/2);
 
				glVertex3f(p1.x, p1.y, p1.z); 
				a-=0.2;
		}
	}
   glEnd();
   glTranslatef( 0.0f, 0.0f, 1.0f );
   }
   glPopMatrix();
 glEndList();
}

void display()
{

  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
  glLoadIdentity();
  gluLookAt(0, 0, 20 + zoomValue, 0, 0, 0, 0, 1, 0);

  // multiply current matrix with arcball matrix
  glMultMatrixf( arcball.get() );

  GLUquadric *sphere = gluNewQuadric(); 
  glPolygonMode( GL_FRONT_AND_BACK, GL_FILL );

  //draw_sun();
  animateObject();
    if (mode==0) {
      moebius();
      glutPostRedisplay();
    }
    else {
      glCallList(MOEBIUS);
      glutPostRedisplay();
    }
  animateObject();  
  calculateFPS();
  drawFPS();
  
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

void keyboard( GLubyte key, GLint x, GLint y )
{
  switch(key)
  {
  case 'q':
  case 'Q':
  case KEY_ESC:
    exit(0);
  case 'l':
    if (mode==0) {
      mode=1;
    }
    else {
      mode=0;
    }
    break;
  }
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
    
  // set a light model to TWO_SIDE
  //glLightModeli(GL_LIGHT_MODEL_TWO_SIDE,1);
  // the default light model is TWO_SIDE = 0
  glLightModeli(GL_LIGHT_MODEL_TWO_SIDE,0);
  // enable lighting 
  glEnable(GL_LIGHTING);
  glEnable(GL_LIGHT0);
  glEnable(GL_DEPTH_TEST);

  // enable the manipulation of the diffuse material component by glColor();
  glEnable(GL_COLOR_MATERIAL);

  moebius_list();

}


void drawFPS()
{
	char buf[20];	
	glLoadIdentity ();
	snprintf(buf, 20, "FPS: %f", fps);
        renderText(buf, -4.7,3.3,-12.0);
}

void idle (void)
{
    glutPostRedisplay ();

}

void calculateFPS()
{
    //  Increase frame count
    frameCount++;
 
    //  Get the number of milliseconds since glutInit called
    //  (or first call to glutGet(GLUT ELAPSED TIME)).
    currentTime = glutGet(GLUT_ELAPSED_TIME);
 
    //  Calculate time passed
    int timeInterval = currentTime - previousTime;
 
    if(timeInterval > 1000)
    {
        //  calculate the number of frames per second
        fps = frameCount / (timeInterval / 1000.0f);
 
        //  Set time
        previousTime = currentTime;
 
        //  Reset frame count
        frameCount = 0;
    }
}

void renderText(char *text, float x, float y, float z)
{
  char *c;
  glRasterPos3f(x, y, z);
  glColorMaterial(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE);
  glEnable(GL_COLOR_MATERIAL);
  glMaterialfv(GL_FRONT, GL_EMISSION, font_emission); 
  glColor3f(0.0,0.0,0.0);
  
  for (c=text; *c != '\0'; c++) {	
    glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, *c);	
  }
  glDisable(GL_COLOR_MATERIAL);
}


int main( int argc,char** argv )
{
  glutInit( &argc, argv );
  glutInitDisplayMode( GLUT_DOUBLE | GLUT_DEPTH | GLUT_RGBA );
  glutInitWindowSize ( screenWidth, screenHeight );
  glutInitWindowPosition( 0, 0 );
  glutCreateWindow( "One or Two" );

  init();
  //glutReshapeFunc( reshape );
  
  //glutIdleFunc( display );
  //glutDisplayFunc( display );
  
  glutKeyboardFunc( keyboard );
  //glutMouseFunc( mouse );
  //glutMotionFunc( motion );
  
   glutDisplayFunc (display);
   glutReshapeFunc  (reshape);
  //glutIdleFunc (idle);

  glutMainLoop();
  return 0;
}
