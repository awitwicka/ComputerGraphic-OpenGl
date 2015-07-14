/*********************************************
  E01 TwoSided
 
  Author:
  Susanne Kroemker, IWR - UNIVERSITAET HEIDELBERG
  Im Neuenheimer Feld 368
  D-69120 Heidelberg
 
  phone +49 (0)6221 54 8883
  fax   +49 (0)6221 54 8850
 
*********************************************/

#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#if defined(__APPLE__) || defined(MACOSX)
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

#include "util/arcball.h"

#define KEY_ESC 27

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
int _cutoff = 10;

// initialize arcball 
static gl::ArcBall arcball;

// method prototypes
void draw_sun();
void draw_coordinate_system( float unit );
void display();
void reshape( GLint width, GLint height );
void keyboard( GLubyte key, GLint x, GLint y );
void mouse( int button, int state, int x, int y );
void motion( int x, int y );
void init();
int main( int argc, char** argv );


void draw_sun()
{
  static const GLfloat light_pos[]        = {4.0f, 4.0f, 8.0f, 1.0f};
  static const GLfloat sun_color[]        = {1.0f, 1.0f, 0.0f};
  static const GLfloat sun_emissive[]     = {1.0f, 1.0f, 0.0f};
  static const GLfloat not_emissive[]     = {0.0f, 0.0f, 0.0f};

  GLfloat lmodel_ambient[] = { 0.25, 0.25, 0.35, 1.0 };
  GLfloat light0_diff[4] = {1.0,1.0,1.0,1.0};
  GLfloat light0_spec[4] = {1.0,1.0,1.0,1.0};
  GLfloat spot_direction[] = { 0.0f, 0.0f, -1.0f};

  //glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, sun_color);
  glLightModelfv(GL_LIGHT_MODEL_AMBIENT, lmodel_ambient);
  glMaterialfv(GL_FRONT_AND_BACK, GL_EMISSION, sun_emissive);
  glPushMatrix();
    //my new light	
    glLightfv(GL_LIGHT0, GL_SPOT_DIRECTION, spot_direction);
    glLightfv(GL_LIGHT0, GL_POSITION, light_pos);
    glTranslatef(light_pos[0], light_pos[1], light_pos[2]);
    glutSolidSphere( 0.2f, 40, 40 );
    glMaterialfv(GL_FRONT_AND_BACK, GL_EMISSION, not_emissive);
  glPopMatrix();

  glShadeModel(GL_SMOOTH);
  glLightfv(GL_LIGHT0, GL_DIFFUSE, light0_diff);
  glLightfv(GL_LIGHT0, GL_SPECULAR, light0_spec);
  glLightf(GL_LIGHT0, GL_CONSTANT_ATTENUATION, 0.1);
  glLightf(GL_LIGHT0, GL_LINEAR_ATTENUATION, 0.001);
  glLightf(GL_LIGHT0, GL_QUADRATIC_ATTENUATION, 0.0001);
  glLightf(GL_LIGHT0, GL_SPOT_CUTOFF, _cutoff); 
  glLightf(GL_LIGHT0, GL_SPOT_EXPONENT, 40);
}

void draw_plane(float unit, int n) 
{
  int i=0;
  int j=0;


  //silver
  GLfloat shininess[] = {51.2};
  GLfloat ambientColor[] = {0.19225, 0.19225, 0.19225, 1.0f};
  GLfloat diffuseColor[] = {0.50754, 0.50754, 0.50754, 1.0f};
  GLfloat specularColor[] = {0.508273, 0.508273, 0.508273, 1.0f};

  glMaterialfv(GL_FRONT_AND_BACK, GL_SHININESS, shininess);
  glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, ambientColor);
  glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, diffuseColor);
  glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, specularColor);
  glEnable(GL_COLOR_MATERIAL);

  //glColor3f( 0.8f, 0.8f, 0.8f );
  
  glBegin(GL_TRIANGLE_STRIP);
	for(j; j<(pow(2,(n-1))); j=j+2) {
		i=0;
		for(i; i<(pow(2,(n-1))+1); i++) {
			glVertex3f(1.0f*i*unit,(0.0f+j)*unit,0.0f); //bottom    
			glVertex3f(1.0f*i*unit,(1.0f+j)*unit,0.0f); //top
		}
		//i=3
		i--;
		for(i; i>0; i--) {
		    glVertex3f(1.0f*i*unit,(1.0f+1+j)*unit,0.0f); //top
			glVertex3f(1.0f*(i-1)*unit,(0.0f+1+j)*unit,0.0f); //bottom    
		}
		glVertex3f(1.0f*i*unit,(1.0f+1+j)*unit,0.0f);
    }
  glEnd();
  
}
  
void draw_coordinate_system( float unit )
{
  GLUquadricObj* circle = NULL;
  circle=gluNewQuadric();

  glEnable(GL_COLOR_MATERIAL);
  glColor3f( 0.2f, 0.2f, 0.2f );
  //glNormal3f( 0.57735f, 0.57735f, 0.57735f );

  glLineWidth(2);
  for (int i=0; i < unit; i++) {
    glBegin(GL_LINES);
      glVertex3f( 1.0f * i, 0.1f, 0.0f );
      glVertex3f( 1.0f * i, 0.0f, 0.0f );
      glVertex3f( 0.0f, 1.0f * i, 0.1f );
      glVertex3f( 0.0f, 1.0f * i, 0.0f );
      glVertex3f( 0.1f, 0.0f, 1.0f * i );
      glVertex3f( 0.0f, 0.0f, 1.0f * i );
    glEnd();
  }
  glBegin(GL_LINES);
    glVertex3f( 0.0f, 0.0f, 0.0f );
    glVertex3f( 1.0f * unit, 0.0f, 0.0f );
    glVertex3f( 0.0f, 0.0f, 0.0f );
    glVertex3f( 0.0f, 1.0f * unit, 0.0f );
    glVertex3f( 0.0f, 0.0f, 0.0f );
    glVertex3f( 0.0f, 0.0f, 1.0f * unit );
  glEnd();
  glLineWidth(1);

  glColor3f( 0.7f, 0.7f, 0.3f );
  glPushMatrix();
    glRotatef( 90.0f, 0.0f, 1.0f, 0.0f );
    glTranslatef( 0.0f, 0.0f, 1.0f * unit );
    glutSolidCone( 0.02f * unit, 0.10f * unit, 20, 20 );
    gluDisk(circle, 0.0f, 0.02f * unit, 30, 30);
  glPopMatrix();
  glPushMatrix();
    glRotatef( 270.0f, 1.0f, 0.0f, 0.0f );
    glTranslatef( 0.0f, 0.0f, 1.0f * unit );
    glutSolidCone( 0.02f * unit, 0.10f * unit, 20, 20 );
    gluDisk(circle, 0.0f, 0.02f * unit, 30, 30);
  glPopMatrix();
  glPushMatrix();
    glTranslatef( 0.0f, 0.0f, 1.0f * unit );
    glutSolidCone( 0.02f * unit, 0.10f * unit, 20, 20 );
    gluDisk(circle, 0.0f, 0.02f * unit, 30, 30);
  glPopMatrix();
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

  draw_sun();
  draw_coordinate_system(5.0f);
  glPushMatrix();
  glTranslatef( 0.0f, 0.0f, 0.0f );
  draw_plane(0.05f, 9); 
  glPopMatrix();

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
  case '1':
    _cutoff = _cutoff + 1;
    glLightf(GL_LIGHT0, GL_SPOT_CUTOFF, _cutoff);
    glutPostRedisplay();
    break;
  case '2':
    _cutoff = _cutoff - 1;
    glLightf(GL_LIGHT0, GL_SPOT_CUTOFF, _cutoff);
    glutPostRedisplay();
    break;
  case 'q':
  case 'Q':
  case '+':
    _cutoff = _cutoff + 1;
    glLightf(GL_LIGHT0, GL_SPOT_CUTOFF, _cutoff);
    glutPostRedisplay();
  case '-':
    _cutoff = _cutoff - 1;
    glLightf(GL_LIGHT0, GL_SPOT_CUTOFF, _cutoff);
    glutPostRedisplay();
  case KEY_ESC:
    exit(0);
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
    
  glLightModeli(GL_LIGHT_MODEL_TWO_SIDE,0);
  // enable lighting 
  glEnable(GL_LIGHTING);
  glEnable(GL_LIGHT0);
  glEnable(GL_DEPTH_TEST);

  // enable the manipulation of the diffuse material component by glColor();
  glEnable(GL_COLOR_MATERIAL);
}



int main( int argc,char** argv )
{
  glutInit( &argc, argv );
  glutInitDisplayMode( GLUT_DOUBLE | GLUT_DEPTH | GLUT_RGBA );
  glutInitWindowSize ( screenWidth, screenHeight );
  glutInitWindowPosition( 0, 0 );
  glutCreateWindow( "One or Two" );

  init();
  glutReshapeFunc( reshape );
  
  glutIdleFunc( display );
  glutDisplayFunc( display );
  
  glutKeyboardFunc( keyboard );
  glutMouseFunc( mouse );
  glutMotionFunc( motion );
  
  glutMainLoop();
  return 0;
}
