#ifndef __HEADER__ //include guard, avoid double inclusion
#define __HEADER__
#include <GL/glut.h>
#endif
#include <stdio.h>
#include <list>
#include <ctime>
#include <Cg/cg.h>    
#include <Cg/cgGL.h>
#include "util/arcball.h"
#include "particle.h"
#include "mytime.h"

// WINdow Parameters
int width = 1200;
int height = 900;
// ArcBall Parameters
static gl::ArcBall arcball; //initialization
//cg stuff
static CGcontext   cg_context;
static CGprofile   cg_vertex_profile;
static CGprogram   cg_vertex_program;
static CGparameter cg_vertex_param_modelview_proj;
ParticleSystem particle1 = ParticleSystem();
float elapse_time = 0;
//functions
void draw_axis(float unit);




/********************** DISPLAY ***********************/
void display() {
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT ); //clearing buffer every frame
  glLoadIdentity();
  gluLookAt(0, 0, 20, 0, 0, 0, 0, 1, 0);
  glMultMatrixf(arcball.get());

  elapse_time = diff_seconds();
  
  cgGLBindProgram(cg_vertex_program);
  cgGLEnableProfile(cg_vertex_profile);
  cgGLSetStateMatrixParameter(cg_vertex_param_modelview_proj, CG_GL_MODELVIEW_PROJECTION_MATRIX, CG_GL_MATRIX_IDENTITY);
  particle1.Draw(elapse_time, 0.0, 0.0, -6.0);
  cgGLDisableProfile(cg_vertex_profile);

  glPushMatrix();
  glTranslatef (0.0, 0.0, -6.0); 
  draw_axis(1);
  glPopMatrix();
  glutSwapBuffers();
}

void draw_axis( float unit) {
  GLUquadricObj* circle = NULL;
  circle=gluNewQuadric();

  glColor3f( 0.1f, 0.1f, 0.1f );

  glLineWidth(2);
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
/*************** MAUSE AND KEYBOARD FUNCTIONS ****************/

void keyboard(unsigned char key, int x, int y) {
  enum {KEY_ESC = 27};
  switch (key) {
  	case KEY_ESC:
	  exit(0); 		
  	case 'q':
    	//cgDestroyProgram(cg_vertex_program);
    	//cgDestroyContext(cg_context);
    	  exit(0);
  	break;
  }
}

void reshape(int w, int h)
{ 
  arcball.set_win_size(w, h);
  float aspect = (double)w / (double)h;
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  gluPerspective(45.0, aspect, 1, 1000);
  glViewport(0, 0, w, h);
  glMatrixMode(GL_MODELVIEW);
}

void mouse(int button, int state, int x, int y)
{
  if (button == GLUT_LEFT_BUTTON) {
	arcball.set_cur(x, y);
	if (state == GLUT_DOWN) {
		arcball.begin_drag();
	} else {
		arcball.end_drag();
	}
  }
  glutPostRedisplay();
}

void motion(int x, int y)
{
  arcball.set_cur(x, y);
  glutPostRedisplay();
}

void idle()
{
  glutPostRedisplay();
}

/********************** init & MAIN ****************************/

void init() {
  glClearColor(0.1f, 0.1f, 0.1f, 0.0f);  //background
  glEnable(GL_LIGHTING);
  glEnable(GL_LIGHT0);
  glEnable(GL_COLOR_MATERIAL);
  glEnable(GL_DEPTH_TEST);
}

int main(int argc,char** argv) {
  glutInit(&argc, argv);
  glutInitDisplayMode( GLUT_DEPTH | GLUT_DOUBLE |GLUT_RGBA); 
  glutInitWindowSize (width, height);
  glutInitWindowPosition (100, 100);
  glutCreateWindow ("ParticleSystem");
  srand(time(NULL));
  init();

  glutDisplayFunc(display);
  glutIdleFunc(idle);
  glutReshapeFunc(reshape);
  glutKeyboardFunc(keyboard);
  glutMouseFunc(mouse);
  glutMotionFunc(motion);
  
  int texture_no = 2; //1 or 2
  int particle_no = 1000;
  int r = 0;
  int g = 1;
  int b = 1;
  float a = 0.8;
  particle1.Initialization(texture_no, particle_no, r, g, b, a);

  cg_context = cgCreateContext();
  cg_vertex_profile = cgGLGetLatestProfile(CG_GL_VERTEX);
  cgGLSetOptimalOptions(cg_vertex_profile);
  cg_vertex_program = cgCreateProgramFromFile(cg_context, CG_SOURCE, "vertex.cg", cg_vertex_profile, "vertex", NULL);    
  cgGLLoadProgram(cg_vertex_program);             

  cg_vertex_param_modelview_proj = cgGetNamedParameter(cg_vertex_program, "modelview_proj");

  glutMainLoop();
  return 0;
}
