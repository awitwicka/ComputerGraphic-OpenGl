/* 
  Open-GL program using Cg for using simple Cg vertex-shaders
  by Daniel Jungblut, IWR Heidelberg, February 2008
  based on example code of Cg Tutorial (Addison-Wesley, ISBN
  0321194969) by Randima Fernando and Mark J. Kilgard.
*/


#include <cstdlib>
#include <stdio.h>

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


static CGcontext   cg_context;
static CGprofile   cg_vertex_profile;
static CGprogram   cg_vertex_program;

// Error checking routine for Cg:
static void checkForCgError(const char *situation) {
  CGerror error;
  const char *string = cgGetLastErrorString(&error);

  if (error != CG_NO_ERROR) {
    printf("%s: %s\n", situation, string);
    if (error == CG_COMPILER_ERROR) {
      printf("%s\n", cgGetLastListing(cg_context));
    }
    exit(1);
  }
}

 
// keyboard callback:
void keyboard(unsigned char key, int x, int y) {
  switch (key) {
  case 27: 		// Escape
  case 'q':
    cgDestroyProgram(cg_vertex_program);
    cgDestroyContext(cg_context);
    exit(0);
  break;
  }
}

 
// display function:
void display() {
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  cgGLBindProgram(cg_vertex_program);
  checkForCgError("binding vertex program");

  cgGLEnableProfile(cg_vertex_profile);
  checkForCgError("enabling vertex profile");
 
// Rendering of a triangle. No color is chosen so far!
  glBegin(GL_TRIANGLES);
    glVertex2f(-0.8, 0.8);
    glVertex2f(0.8, 0.8);
    glVertex2f(0.0, -0.8);
  glEnd();

  cgGLDisableProfile(cg_vertex_profile);
  checkForCgError("disabling vertex profile");

  glutSwapBuffers();
}
  

int main(int argc, char **argv) {

  glutInitWindowSize(400, 400);
  glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE);
  glutInit(&argc, argv);

  glutCreateWindow("A simple vertex shader");
  glutDisplayFunc(display);
  glutKeyboardFunc(keyboard);

  glClearColor(0.1, 0.2, 0.8, 1.0);

  cg_context = cgCreateContext();
  checkForCgError("creating context");

  cg_vertex_profile = cgGLGetLatestProfile(CG_GL_VERTEX);
  cgGLSetOptimalOptions(cg_vertex_profile);
  checkForCgError("selecting vertex profile");

  cg_vertex_program = cgCreateProgramFromFile(cg_context, CG_SOURCE, "E03.cg", 
						cg_vertex_profile, "simple_vertex_shader", NULL);                 

  checkForCgError("creating vertex program from file");
  cgGLLoadProgram(cg_vertex_program);
  checkForCgError("loading vertex program");

  glutMainLoop();
  return 0;
}


