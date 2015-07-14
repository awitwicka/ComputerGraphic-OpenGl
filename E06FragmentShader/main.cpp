/* 
   Open-GL program using Cg for using simple Cg vertex-shaders
   by Daniel Jungblut, IWR Heidelberg, February 2008
   based on example code of Cg Tutorial (Addison-Wesley, ISBN
   0321194969) by Randima Fernando and Mark J. Kilgard
   revised by Susanne Kroemker, IWR Heidelberg, April 2014
 */


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

static CGcontext   cg_context;
static CGprofile   cg_vertex_profile;
static CGprogram   cg_vertex_program;

static CGprofile   cg_fragment_profile;
static CGprogram   cg_fragment_program;

static CGparameter cg_parameter_vertex_scale_factor;
static CGparameter cg_parameter_vertex_rotation_angle;
static CGparameter cg_parameter_fragment_color;

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

void keyboard(unsigned char key, int x, int y) {
  switch (key) {
    case 27:     // Escape key is define as 27
    case 'q':
      cgDestroyProgram(cg_vertex_program);
      cgDestroyProgram(cg_fragment_program);
      cgDestroyContext(cg_context);
      exit(0);
      break;
  }
}

void display() {
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  cgGLBindProgram(cg_vertex_program);
  checkForCgError("binding vertex program");

  cgGLEnableProfile(cg_vertex_profile);
  checkForCgError("enabling vertex profile");

  // Set value of uniform parameter "scale_factor" in vertex shader
  cgGLSetParameter1f(cg_parameter_vertex_scale_factor, 0.7);

  cgGLSetParameter1f(cg_parameter_vertex_rotation_angle, 90);

  cgGLBindProgram(cg_fragment_program);
  checkForCgError("binding fragment program");

  cgGLEnableProfile(cg_fragment_profile);
  checkForCgError("enabling fragment profile");

  // Set value of uniform parameter "_color" in fragment shader
  cgGLSetParameter4f(cg_parameter_fragment_color, 0.0, 0.7, 0.0, 1.0);


// Rendering of a triangle. No color is given here.
  glBegin(GL_TRIANGLES);
  glVertex2f(-0.8, 0.8);
  glVertex2f(0.8, 0.8);
  glVertex2f(0.0, -0.8);
  glEnd();

  cgGLDisableProfile(cg_fragment_profile);
  checkForCgError("disabling fragment profile");

  cgGLDisableProfile(cg_vertex_profile);
  checkForCgError("disabling vertex profile");

  glutSwapBuffers();
}


int main(int argc, char **argv) {

  glutInitWindowSize(400, 400);
  glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE);
  glutInit(&argc, argv);

  glutCreateWindow("Vertex and fragment shaders");
  glutDisplayFunc(display);
  glutKeyboardFunc(keyboard);

  glClearColor(0.1, 0.2, 0.8, 1.0); 

  cg_context = cgCreateContext();
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

  cg_parameter_vertex_rotation_angle = cgGetNamedParameter(cg_vertex_program, "rotation_angle");
  checkForCgError("getting rotation_angle parameter");

/**********/

  cg_fragment_profile = cgGLGetLatestProfile(CG_GL_FRAGMENT);
  checkForCgError("selecting fragment profile");

  cg_fragment_program = cgCreateProgramFromFile(cg_context, CG_SOURCE, "E06_fragment.cg",
      cg_fragment_profile, "simple_fragment_shader", NULL);
  checkForCgError("creating fragment program from file");
  cgGLLoadProgram(cg_fragment_program);
  checkForCgError("loading fragment program");

  cg_parameter_fragment_color = cgGetNamedParameter(cg_fragment_program, "_color");
  checkForCgError("getting _color parameter");

  glutMainLoop();
  return 0;
}
