/*********************************************
  Solution E01 TwoSided
 
  Author:
  Susanne Kroemker, IWR - UNIVERSITAET HEIDELBERG
  Im Neuenheimer Feld 368
  D-69120 Heidelberg
 
  phone +49 (0)6221 54 8883
  fax   +49 (0)6221 54 8850
 
*********************************************/

#include <stdio.h>
#include <stdlib.h>

#if defined(__APPLE__) || defined(MACOSX)
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

#include "util/arcball.h"
#include "load_bmp.h"

#define KEY_ESC 27

GLuint texture;
int imageWidth = 1024;
int imageHeight = 1024;
float terrain[1024][1024][3];
float MAP_SCALE_XZ = 0.01;
float MAP_SCALE_high = 0.01;
GLubyte* _imageData;
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

void translateHights() {
    for (int z = 0; z < imageHeight; z++)
    {
        for (int x = 0; x < imageWidth; x++)
        {
	    terrain[x][z][0] = float(x)*MAP_SCALE_XZ;
            terrain[x][z][1] = (((float)_imageData[(z*imageHeight+x)*3])*MAP_SCALE_high);
            terrain[x][z][2] = -float(z)*MAP_SCALE_XZ;
	    //fprintf("%.2f",(float)_imageData[0]);
	    //float a = (float)_imageData[1];
 	    //printf("%.2f\n", (float)_imageData[(z*imageHeight+x)*3]);
        }
    }
}

void drawTerrain() {
    for (int z = 0; z < imageHeight-1; z++)
    {
        glBegin(GL_TRIANGLE_STRIP);
        for (int x = 0; x < imageWidth-1; x++)
        {
            // draw vertex 0
            glColor3f(terrain[x][z][1]/255.0f/MAP_SCALE_high, 
                      terrain[x][z][1]/255.0f/MAP_SCALE_high, 
                      terrain[x][z][1]/255.0f/MAP_SCALE_high);
            //glTexCoord2f(0.0f, 0.0f);
            glVertex3f(terrain[x][z][0], 
                       terrain[x][z][1], terrain[x][z][2]);

            // draw vertex 1
            //glTexCoord2f(1.0f, 0.0f);
            glColor3f(terrain[x+1][z][1]/255.0f/MAP_SCALE_high, 
                      terrain[x+1][z][1]/255.0f/MAP_SCALE_high, 
                      terrain[x+1][z][1]/255.0f/MAP_SCALE_high);
            glVertex3f(terrain[x+1][z][0], terrain[x+1][z][1], 
                       terrain[x+1][z][2]);

            // draw vertex 2
            //glTexCoord2f(0.0f, 1.0f);
            glColor3f(terrain[x][z+1][1]/255.0f/MAP_SCALE_high, 
                      terrain[x][z+1][1]/255.0f/MAP_SCALE_high, 
                      terrain[x][z+1][1]/255.0f/MAP_SCALE_high);
            glVertex3f(terrain[x][z+1][0], terrain[x][z+1][1], 
                       terrain[x][z+1][2]);

            // draw vertex 3
            glColor3f(terrain[x+1][z+1][1]/255.0f/MAP_SCALE_high, 
                      terrain[x+1][z+1][1]/255.0f/MAP_SCALE_high, 
                      terrain[x+1][z+1][1]/255.0f/MAP_SCALE_high);
            //glTexCoord2f(1.0f, 1.0f);
            glVertex3f(terrain[x+1][z+1][0], 
                       terrain[x+1][z+1][1], 
                       terrain[x+1][z+1][2]);
        }
        glEnd();
    }
}

void display()
{
  float unit = 1.0f;
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
  glLoadIdentity();
  gluLookAt(0, 0, 20 + zoomValue, 0, 0, 0, 0, 1, 0);

  // multiply current matrix with arcball matrix
  glMultMatrixf( arcball.get() );

  GLUquadric *sphere = gluNewQuadric(); 
  glPolygonMode( GL_FRONT_AND_BACK, GL_FILL );
/*********************************************************/
  draw_sun();
  draw_coordinate_system(5.0f * unit);

  // draw a solid teapot and a cone and a torus
  glColor3f( 0.5f, 0.5f, 0.6f );
  
  //glEnable(GL_TEXTURE_2D);
  //glutSolidTeapot(5);
  //glDisable(GL_TEXTURE_2D);
  
  drawTerrain();
   
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
    glLightModeli(GL_LIGHT_MODEL_TWO_SIDE,0);
    
    break;
  case '2':
    glLightModeli(GL_LIGHT_MODEL_TWO_SIDE,1);
    
    break;
  case 'q':
  case 'Q':
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

void idle()
{
	glutPostRedisplay();
}

void init()
{
  glClearColor (0.7f, 0.7f, 1.0f, 0.0f);
    
  // the default light model is TWO_SIDE = 0
  glLightModeli(GL_LIGHT_MODEL_TWO_SIDE,0);
  // enable lighting 
  glEnable(GL_LIGHTING);
  glEnable(GL_LIGHT0);
  glEnable(GL_DEPTH_TEST);

  // enable the manipulation of the diffuse material component by glColor();
  glEnable(GL_COLOR_MATERIAL);
/*****************************************************/
  GLubyte* image;
  image = (GLubyte*) loadBMP("land.bmp", &imageWidth, &imageHeight);
  if(image != NULL) {
      //
      glBindTexture(GL_TEXTURE_2D, texture);
      //
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
      //
      glTexImage2D(GL_TEXTURE_2D, 0, 3, imageWidth, imageHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
      _imageData = image;
      //
      //free(image);
  } else {
      printf("Cannot load texture!\n");
  }
/********************************************************/
  translateHights();

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
  
  glutIdleFunc( idle );
  glutDisplayFunc( display );
  
  glutKeyboardFunc( keyboard );
  glutMouseFunc( mouse );
  glutMotionFunc( motion );
  
  glutMainLoop();
  return 0;
}
