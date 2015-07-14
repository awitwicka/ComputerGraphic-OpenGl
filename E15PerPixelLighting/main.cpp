/* 
  Open-GL program using Cg for simple light models
  by Daniel Jungblut, IWR Heidelberg, February 2008
  revised by Susanne Kroemker, IWR Heidelberg, May 2014
  based on example code of Cg Tutorial (Addison-Wesley, ISBN
  0321194969) by Randima Fernando and Mark J. Kilgard.
*/


#include <cmath>
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


// declaration of used functions
 
static void checkForCgError(const char *situation);
void keyboard(unsigned char key, int x, int y);
void reshape(int width, int height);
void reshape();
void idle();
void display();
int main(int argc, char **argv);


static CGcontext   cg_context;
static CGprofile   cg_fragment_profile;
static CGprogram   cg_fragment_program;
static CGprofile   cg_vertex_profile;
static CGprogram   cg_vertex_program;

static CGparameter cg_vertex_param_modelview_proj, cg_vertex_param_translation;
static CGparameter cg_fragment_param_ambient_light,
		   cg_fragment_param_light_color,
		   cg_fragment_param_light_position,
		   cg_fragment_param_camera_position,
                   cg_fragment_param_Ke,
                   cg_fragment_param_Ka,
                   cg_fragment_param_Kd,
                   cg_fragment_param_Ks,
                   cg_fragment_param_exponent;

#define GET_PARAM_v(name) cg_vertex_param_##name = cgGetNamedParameter(cg_vertex_program, #name); checkForCgError("getting parameter " #name);
#define GET_PARAM_f(name) cg_fragment_param_##name = cgGetNamedParameter(cg_fragment_program, #name); checkForCgError("getting parameter " #name);

// global variables
float light_angle = 0.0;
float light_angle_speed = 0.015;
GLfloat ambient_light[3] = {0.2, 0.2, 0.2};
GLfloat light_color[3] = {1.0, 1.0, 1.0};
GLfloat light_position[4] = {0.0, 2.0, 4.0, 1.0};
GLfloat camera_position[4] = {0.0, 0.0, 10.0, 1.0};



void set_plastic_material() {

  GLfloat Ke[3] = {0.0, 0.0, 0.0};
  GLfloat Ka[3] = {0.0, 0.0, 0.0};
  GLfloat Kd[3] = {0.7, 0.1, 0.0};
  GLfloat Ks[3] = {0.8, 0.7, 0.6};  
  GLfloat exponent = 33.0;

  cgSetParameter3fv(cg_fragment_param_Ke, Ke);
  checkForCgError("setting Ke parameter");

  cgSetParameter3fv(cg_fragment_param_Ka, Ka);
  checkForCgError("setting Ka parameter");

  cgSetParameter3fv(cg_fragment_param_Kd, Kd);
  checkForCgError("setting Kd parameter");

  cgSetParameter3fv(cg_fragment_param_Ks, Ks);
  checkForCgError("setting Ks parameter");

  cgSetParameter1f(cg_fragment_param_exponent, exponent);
  checkForCgError("setting exponent parameter");
}

void set_brass_material() {

  GLfloat Ke[3] = {0.0, 0.0, 0.0};
  GLfloat Ka[3] = {0.35, 0.23, 0.04};
  GLfloat Kd[3] = {0.79, 0.58, 0.12};
  GLfloat Ks[3] = {0.99, 0.92, 0.82};  
  GLfloat exponent = 27.5;

  cgSetParameter3fv(cg_fragment_param_Ke, Ke);
  checkForCgError("setting Ke parameter");

  cgSetParameter3fv(cg_fragment_param_Ka, Ka);
  checkForCgError("setting Ka parameter");

  cgSetParameter3fv(cg_fragment_param_Kd, Kd);
  checkForCgError("setting Kd parameter");

  cgSetParameter3fv(cg_fragment_param_Ks, Ks);
  checkForCgError("setting Ks parameter");

  cgSetParameter1f(cg_fragment_param_exponent, exponent);
  checkForCgError("setting exponent parameter");
}

void set_my_material() {

  GLfloat Ke[3] = {0.0, 0.0, 0.0};
  GLfloat Ka[3] = {0.19125, 0.0735, 0.0225};
  GLfloat Kd[3] = {0.7038, 0.27048, 0.0828};
  GLfloat Ks[3] = {0.256777, 0.137622, 0.086014};  
  GLfloat exponent = 0.1;

  cgSetParameter3fv(cg_fragment_param_Ke, Ke);
  checkForCgError("setting Ke parameter");

  cgSetParameter3fv(cg_fragment_param_Ka, Ka);
  checkForCgError("setting Ka parameter");

  cgSetParameter3fv(cg_fragment_param_Kd, Kd);
  checkForCgError("setting Kd parameter");

  cgSetParameter3fv(cg_fragment_param_Ks, Ks);
  checkForCgError("setting Ks parameter");

  cgSetParameter1f(cg_fragment_param_exponent, exponent);
  checkForCgError("setting exponent parameter");
}

void set_plane_material() {
  GLfloat Ke[3] = {0.0, 0.0, 0.0};
  GLfloat Ka[3] = {0.4, 1.0, 0.4};
  GLfloat Kd[3] = {0.25, 0.8, 0.25};
  GLfloat Ks[3] = {0.0, 0.0, 0.0};  
  GLfloat exponent = 0.0;

  cgSetParameter3fv(cg_fragment_param_Ke, Ke);
  checkForCgError("setting Ke parameter");

  cgSetParameter3fv(cg_fragment_param_Ka, Ka);
  checkForCgError("setting Ka parameter");

  cgSetParameter3fv(cg_fragment_param_Kd, Kd);
  checkForCgError("setting Kd parameter");

  cgSetParameter3fv(cg_fragment_param_Ks, Ks);
  checkForCgError("setting Ks parameter");

  cgSetParameter1f(cg_fragment_param_exponent, exponent);
  checkForCgError("setting exponent parameter");

}


void set_light_source_material() {

  GLfloat zero[3] = {0.0, 0.0, 0.0};

  cgSetParameter3fv(cg_fragment_param_Ke, light_color);
  checkForCgError("setting Ke parameter");

  cgSetParameter3fv(cg_fragment_param_Ka, zero);
  checkForCgError("setting Ka parameter");

  cgSetParameter3fv(cg_fragment_param_Kd, zero);
  checkForCgError("setting Kd parameter");

  cgSetParameter3fv(cg_fragment_param_Ks, zero);
  checkForCgError("setting Ks parameter");

  cgSetParameter1f(cg_fragment_param_exponent, 0);
  checkForCgError("setting exponent parameter");
}


// error checking routine for Cg
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


// keyboard callback
void keyboard(unsigned char key, int x, int y) {

  static bool animation = false;

  switch (key) {
  case 27: 		// Escape
  case 'q':
    cgDestroyProgram(cg_vertex_program);
    cgDestroyContext(cg_context);
    exit(0);
  break;
  case ' ':
    animation = !animation;
    if(animation) glutIdleFunc(idle);
    else glutIdleFunc(NULL); 
  break;
  }
}


// reshape function
void reshape(int width, int height) {
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  float aspect_ratio = float(width) / float(height);
  float field_of_view = 60.0;
  gluPerspective(field_of_view, aspect_ratio, 0.1, 30.0);
  glViewport(0, 0, width, height);
  glMatrixMode(GL_MODELVIEW);
}


// idle function
void idle() {

  light_angle += light_angle_speed;
  if(light_angle > 2 * M_PI)
    light_angle -= 2 * M_PI;

  light_position[0] = 4.0 * sin(light_angle);
  light_position[1] = 2.0;
  light_position[2] = 4.0 * cos(light_angle) ;

  glutPostRedisplay();
}


// display function
void display() {

  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  GLfloat translation[3];

  cgGLBindProgram(cg_vertex_program);
  checkForCgError("binding vertex program");

  cgGLEnableProfile(cg_vertex_profile);
  checkForCgError("enabling vertex profile");

  cgGLBindProgram(cg_fragment_program);
  checkForCgError("binding fragment program");

  cgGLEnableProfile(cg_fragment_profile);
  checkForCgError("enabling fragment profile");

  glMatrixMode(GL_MODELVIEW);

    glPushMatrix();

    gluLookAt(camera_position[0], camera_position[1], camera_position[2],    // camera Position
	    0.0, 0.0, 0.0,   				         	     // view Center
            0.0, 1.0, 0.0);			        	    	     // up vector


    cgGLSetParameter3fv(cg_fragment_param_ambient_light, ambient_light);
    cgGLSetParameter3fv(cg_fragment_param_light_color, light_color);
    cgGLSetParameter4fv(cg_fragment_param_light_position, light_position);
    cgGLSetParameter4fv(cg_fragment_param_camera_position, camera_position);

    glPushMatrix();

      translation[0] = 2.0; translation[1] = 0.0; translation[2] = 0.0;

      glTranslatef(translation[0], translation[1], translation[2]);
      cgGLSetParameter3fv(cg_vertex_param_translation, translation);

      cgGLSetStateMatrixParameter(cg_vertex_param_modelview_proj, CG_GL_MODELVIEW_PROJECTION_MATRIX, CG_GL_MATRIX_IDENTITY);
      checkForCgError("set parameter modelview_proj");
   
      set_brass_material();
      glutSolidSphere(2.0, 30, 30);

    glPopMatrix();

    glPushMatrix();
    
      translation[0] = -2.0; translation[1] = 0.0; translation[2] = -1.0;

      glTranslatef(translation[0], translation[1], translation[2]);
      cgGLSetParameter3fv(cg_vertex_param_translation, translation);

      cgGLSetStateMatrixParameter(cg_vertex_param_modelview_proj, CG_GL_MODELVIEW_PROJECTION_MATRIX, CG_GL_MATRIX_IDENTITY);
      checkForCgError("set parameter modelview_proj");

      set_plastic_material();
      glutSolidCube(3.0);

    glPopMatrix();

    glPushMatrix();
    
      translation[0] = 0.0; translation[1] = -0.5; translation[2] = 3.0;

      glTranslatef(translation[0], translation[1], translation[2]);
      cgGLSetParameter3fv(cg_vertex_param_translation, translation);

      cgGLSetStateMatrixParameter(cg_vertex_param_modelview_proj, CG_GL_MODELVIEW_PROJECTION_MATRIX, CG_GL_MATRIX_IDENTITY);
      checkForCgError("set parameter modelview_proj");

      set_my_material();
      glutSolidTorus(0.25, 0.7, 30, 30);

    glPopMatrix();

    glPushMatrix();

      translation[0] = 0.0; translation[1] = -1.5; translation[2] = 0.0;

      glTranslatef(translation[0], translation[1], translation[2]);
      cgGLSetParameter3fv(cg_vertex_param_translation, translation);  

      cgGLSetStateMatrixParameter(cg_vertex_param_modelview_proj, CG_GL_MODELVIEW_PROJECTION_MATRIX, CG_GL_MATRIX_IDENTITY);
      checkForCgError("set parameter modelview_proj"); 

      set_plane_material();
      glBegin(GL_QUADS);
        glNormal3f(0.0, 1.0, 0.0);
        glVertex3f(-10.0, 0.0, -10.0);
        glVertex3f(10.0, 0.0, -10.0);
        glVertex3f(10.0, 0.0, 10.0);
        glVertex3f(-10.0, 0.0, 10.0);
      glEnd();

    glPopMatrix();

    glPushMatrix();

      glTranslatef(light_position[0], light_position[1], light_position[2]);

      cgGLSetParameter3fv(cg_vertex_param_translation, light_position);

      cgGLSetStateMatrixParameter(cg_vertex_param_modelview_proj, CG_GL_MODELVIEW_PROJECTION_MATRIX, CG_GL_MATRIX_IDENTITY);
      checkForCgError("set parameter modelview_proj");

      set_light_source_material();
      glutSolidSphere(0.2, 10, 10);

    glPopMatrix();

  glPopMatrix();

  cgGLDisableProfile(cg_vertex_profile);
  checkForCgError("disabling vertex profile");

  cgGLDisableProfile(cg_fragment_profile);
  checkForCgError("disabling fragment profile");

  glutSwapBuffers();
}


int main(int argc, char **argv) {

  glutInitWindowSize(600, 600);
  glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH);
  glutInit(&argc, argv);

  glutCreateWindow("Per vertex lighting (Gouraud shading)");
  glutDisplayFunc(display);
  glutKeyboardFunc(keyboard);
  glutReshapeFunc(reshape);

  glClearColor(0.0, 0.0, 0.0, 1.0);

  cg_context = cgCreateContext();
  checkForCgError("creating context");
/********/
  cg_vertex_profile = cgGLGetLatestProfile(CG_GL_VERTEX);
  cgGLSetOptimalOptions(cg_vertex_profile);
  checkForCgError("selecting vertex profile");

  cg_vertex_program = cgCreateProgramFromFile(cg_context, CG_SOURCE, "E12_vertex.cg", 
						cg_vertex_profile, "per_vertex_lighting", NULL);                 

  checkForCgError("creating vertex program from file");
  cgGLLoadProgram(cg_vertex_program);
  checkForCgError("loading vertex program");

  GET_PARAM_v(modelview_proj);
  GET_PARAM_v(translation);

/********/
  cg_fragment_profile = cgGLGetLatestProfile(CG_GL_FRAGMENT);
  checkForCgError("selecting fragment profile");

  cg_fragment_program = cgCreateProgramFromFile(cg_context, CG_SOURCE, "E15_fragment.cg",
      						cg_fragment_profile, "per_fragment_lighting", NULL);
  checkForCgError("creating fragment program from file");
  cgGLLoadProgram(cg_fragment_program);
  checkForCgError("loading fragment program");

  
 
  GET_PARAM_f(ambient_light);
  GET_PARAM_f(light_color);
  GET_PARAM_f(light_position);
  GET_PARAM_f(camera_position);
  GET_PARAM_f(Ke);
  GET_PARAM_f(Ka);
  GET_PARAM_f(Kd);
  GET_PARAM_f(Ks);
  GET_PARAM_f(exponent);

  glEnable(GL_DEPTH_TEST);

  glutMainLoop();
  return 0;
}
