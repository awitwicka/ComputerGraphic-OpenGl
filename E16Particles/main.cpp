/* 
   Open-GL program using Cg for using simple Cg vertex-shaders
   by Daniel Jungblut, IWR Heidelberg, February 2008
   based on example code of Cg Tutorial (Addison-Wesley, ISBN
   0321194969) by Randima Fernando and Mark J. Kilgard
   revised by Susanne Kroemker, IWR Heidelberg, April 2014
 */

/*THIS ONE IS NOT REALLY DONE, IT'S RATHER BEGGINING*/
#include <cstdlib>
#include <stdio.h>
#include <math.h>

#ifndef __INCLUDES__
#define __INCLUDES__

#if defined(__APPLE__) || defined(MACOSX)
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

#endif

#include <Cg/cg.h>    
#include <Cg/cgGL.h>
#include "util/arcball.h"
#include <time.h>


// screen size
int screenWidth = 800;
int screenHeight = 800;
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

float fXOffset=0;
float fYOffset=0;
GLuint texture = 0;

float rotation = 1.509;

static CGcontext   cg_context;
static CGprofile   cg_vertex_profile;
static CGprogram   cg_vertex_program;

static CGprofile   cg_fragment_profile;
static CGprogram   cg_fragment_program;

static CGparameter cg_parameter_vertex_scale_factor;
static CGparameter cg_parameter_vertex_rotation;

static CGparameter cg_parameter_fragment_color;

//functions
GLuint loadPNGTexture (const char *filename);
void reshape( GLint width, GLint height );
void mouse( int button, int state, int x, int y );
void motion( int x, int y );
void display();
void init();
int main( int argc, char** argv );
void ComputePositionOffsets(float &fXOffset, float &fYOffset);

// Error checking routine for Cg:
/*static void checkForCgError(const char *situation) {
  CGerror error;
  const char *string = cgGetLastErrorString(&error);

  if (error != CG_NO_ERROR) {
    printf("%s: %s\n", situation, string);
    if (error == CG_COMPILER_ERROR) {
      printf("%s\n", cgGetLastListing(cg_context));
    }
    exit(1);
  }
}*/

void keyboard(unsigned char key, int x, int y) {
  switch (key) {
    case 27:     // Escape key is define as 27
    case 'q':
      cgDestroyProgram(cg_vertex_program);
      cgDestroyProgram(cg_fragment_program);
      cgDestroyContext(cg_context);
      exit(0);
      break;

    // Rotation
    case '+':
      rotation += 1.0/M_PI;
      break;
    case '-':
      rotation -= 1.0/M_PI;
      break;
  }

  rotation  = fmodf(rotation, 2*M_PI);
  glutPostRedisplay();
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

void display() {
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  glLoadIdentity();
  gluLookAt(0, 0, 20 + zoomValue, 0, 0, 0, 0, 1, 0);
  glMultMatrixf( arcball.get() );

  /*cgGLBindProgram(cg_vertex_program);
  checkForCgError("binding vertex program");

  cgGLEnableProfile(cg_vertex_profile);
  checkForCgError("enabling vertex profile");

  // Set value of uniform parameter "scale_factor" in vertex shader
  cgGLSetParameter1f(cg_parameter_vertex_scale_factor, 0.7);

  // Set rotation parameter
  cgGLSetParameter1f(cg_parameter_vertex_rotation, rotation);

  cgGLBindProgram(cg_fragment_program);
  checkForCgError("binding fragment program");

  cgGLEnableProfile(cg_fragment_profile);
  checkForCgError("enabling fragment profile");

  GLfloat color[] = {0.2, 0.7, 0.3};
  cgGLSetParameter3fv(cg_parameter_fragment_color, color);*/

  glEnable(GL_POINT_SPRITE_ARB);
  glTexEnvi(GL_POINT_SPRITE_ARB, GL_COORD_REPLACE_ARB, GL_TRUE);
  glEnable(GL_VERTEX_PROGRAM_POINT_SIZE);
  glPointSize(15.0f);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE);
  glEnable(GL_BLEND);
  glDepthMask(GL_FALSE);
  glEnable(GL_TEXTURE_2D);
  glBindTexture(GL_TEXTURE_2D, texture);


  //glEnable(GL_POINT_SPRITE);
 // glTexEnvi(GL_POINT_SPRITE, GL_COORD_REPLACE, GL_TRUE);


  ComputePositionOffsets(fXOffset, fYOffset);
  glBegin(GL_POINTS);
    for(int r = 1; r < 10; ++r) {	
        glPointSize(1+rand()%100);
	glVertex2i(fXOffset, fYOffset);
    }
  glEnd();
 /*
  cgGLDisableProfile(cg_fragment_profile);
  checkForCgError("disabling fragment profile");

  cgGLDisableProfile(cg_vertex_profile);
  checkForCgError("disabling vertex profile");
*/
  glDisable(GL_TEXTURE_2D);

  glutPostRedisplay(); 
  glutSwapBuffers();
} 

void ComputePositionOffsets(float &fXOffset, float &fYOffset)
{
    float fElapsedTime = glutGet(GLUT_ELAPSED_TIME) / 1000.0f;
    float v = rand()%4;
    fXOffset += 0.001;//fElapsedTime*v;
    fYOffset += 0.001;//(fElapsedTime*fElapsedTime*9.8)/2;
}

int main(int argc, char **argv) {

  glutInitWindowSize(800, 800);
  glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE);
  glutInit(&argc, argv);

  glutCreateWindow("Vertex and fragment shaders");
  glutDisplayFunc(display);
  glutKeyboardFunc(keyboard);

  glClearColor(0.1, 0.2, 0.8, 1.0); 

  texture = loadPNGTexture("images.png");
  srand(time(0));

  /*cg_context = cgCreateContext();
  checkForCgError("creating context");

  cg_vertex_profile = cgGLGetLatestProfile(CG_GL_VERTEX);
  cgGLSetOptimalOptions(cg_vertex_profile);
  checkForCgError("selecting vertex profile");

  cg_vertex_program = cgCreateProgramFromFile(cg_context, CG_SOURCE, "E06_vertex.cg", 
  cg_vertex_profile, "more_complex_vertex_shader", NULL);                 

  checkForCgError("creating vertex program from file");
  cgGLLoadProgram(cg_vertex_program);
  checkForCgError("loading vertex program");

  // Connect local variables with shader variables

  cg_parameter_vertex_scale_factor = cgGetNamedParameter(cg_vertex_program, "scale_factor");
  checkForCgError("getting scale_factor parameter");

  cg_parameter_vertex_rotation = cgGetNamedParameter(cg_vertex_program, "rotation");


  cg_fragment_profile = cgGLGetLatestProfile(CG_GL_FRAGMENT);
  checkForCgError("selecting fragment profile");

  cg_fragment_program = cgCreateProgramFromFile(cg_context, CG_SOURCE, "E06_fragment.cg",
      cg_fragment_profile, "simple_fragment_shader", NULL);
  checkForCgError("creating fragment program from file");
  cgGLLoadProgram(cg_fragment_program);
  checkForCgError("loading fragment program");

  cg_parameter_fragment_color = cgGetNamedParameter(cg_fragment_program, "color");
  checkForCgError("getting fragment parameter color");*/

  glutReshapeFunc( reshape );
  glutMouseFunc( mouse );
  glutMotionFunc( motion );

  glutMainLoop();
  return 0;
}
