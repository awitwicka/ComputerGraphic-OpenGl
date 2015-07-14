/* 
  Open-GL program using Cg for using simple Cg fragment-shaders
  by Daniel Jungblut, IWR Heidelberg, February 2008
  based on example code of Cg Tutorial (Addison-Wesley, ISBN
  0321194969) by Randima Fernando and Mark J. Kilgard.
*/


#include <cmath>
#include <cstdlib>
#include <stdio.h>

#include <GL/glut.h>
#include <Cg/cg.h>    
#include <Cg/cgGL.h>


// Funktionsdeklarationen:

GLuint loadPNGTexture (const char *filename);
GLuint loadPNGCubeMap (const char *filename, int index);
static void checkForCgError(const char *situation);
void keyboard(unsigned char key, int x, int y);
void reshape(int width, int height);
void reshape();
void idle();
void display();
int main(int argc, char **argv);


// Globale Variablen:

static CGcontext   cg_context;
static CGprofile   cg_vertex_profile;
static CGprogram   cg_vertex_program;

static CGparameter cg_vertex_param_modelview_proj,
		   cg_vertex_param_modelview,
		   cg_vertex_param_camera_position,
		   cg_vertex_param_camera_radius,
		   cg_vertex_param_camera_angle_phi,
		   cg_vertex_param_camera_angle_theta;


static CGprofile   cg_fragment_profile;
static CGprogram   cg_fragment_program;


static CGparameter cg_fragment_param_reflectivity,
		   cg_fragment_param_texture_map,
		   cg_fragment_param_environment_map;


GLuint environment_map[6];

#define GET_PARAM_VERTEX(name) cg_vertex_param_##name = cgGetNamedParameter(cg_vertex_program, #name); checkForCgError("getting vertex parameter " #name);

#define GET_PARAM_FRAGMENT(name) cg_fragment_param_##name = cgGetNamedParameter(cg_fragment_program, #name); checkForCgError("getting fragment parameter " #name);

GLfloat camera_angle_phi = 0.0;
GLfloat camera_angle_theta = M_PI / 2.0;
GLfloat camera_radius = 10.0;

GLfloat teapot_angle = 0.0;
GLfloat camera_position[4] = {camera_radius * sin(camera_angle_theta) * sin(camera_angle_phi), 
			      camera_radius * cos(camera_angle_theta), 
			      camera_radius * sin(camera_angle_theta) * cos(camera_angle_phi) , 1.0};
GLfloat reflectivity = 0.6;



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


// Create environment map:
void create_environment_map() {

  glEnable(GL_TEXTURE_CUBE_MAP);

  char temp[255];
  for(int i = 0; i < 6; i++){
    sprintf(temp, "%i.png", i);
    environment_map[i] = loadPNGCubeMap(temp, i);
  }  


/*  glBindTexture(GL_TEXTURE_CUBE_MAP_POSITIVE_X, environment_map[1]);
  glBindTexture(GL_TEXTURE_CUBE_MAP_NEGATIVE_X, environment_map[3]);
  glBindTexture(GL_TEXTURE_CUBE_MAP_POSITIVE_Y, environment_map[0]);
  glBindTexture(GL_TEXTURE_CUBE_MAP_NEGATIVE_Y, environment_map[5]);
  glBindTexture(GL_TEXTURE_CUBE_MAP_POSITIVE_Z, environment_map[2]);
  glBindTexture(GL_TEXTURE_CUBE_MAP_NEGATIVE_Z, environment_map[4]);*/

  glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
  glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

}

void draw_environment() {

  static const GLfloat vertex[24][3] = {
    { 1, -1, -1 },  { 1, 1, -1 },  { 1, 1, 1 },  { 1, -1, 1 },
    { -1, -1, -1 },  { -1, 1, -1 },  { -1, 1, 1 },  { -1, -1, 1 },
    { -1, 1, -1 },  { 1, 1, -1 },  { 1, 1, 1 },  { -1, 1, 1 },
    { -1, -1, -1 },  { 1, -1, -1 },  { 1, -1, 1 },  { -1, -1, 1 },
    { -1, -1, 1 },  { 1, -1, 1 },  { 1, 1, 1 },  { -1, 1, 1 },
    { -1, -1, -1 },  { 1, -1, -1 },  { 1, 1, -1 },  { -1, 1, -1 }};

  glPushMatrix();

    glLoadIdentity();
    gluLookAt(camera_position[0], camera_position[1], camera_position[2], 0, 0, 0, 0, 1, 0);

    GLfloat distance = 16.0;
    glScalef(distance, distance, distance);

    glEnable(GL_TEXTURE_CUBE_MAP);

    glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
    
    glBegin(GL_QUADS);

      for(int i = 0; i < 24; i++) {
   
        glTexCoord3fv(vertex[i]);
        glVertex3fv(vertex[i]);
      }

    glEnd();

    glDisable(GL_TEXTURE_CUBE_MAP);


  glPopMatrix();

}


// keyboard callback:
void keyboard(unsigned char key, int x, int y) {

  static bool animation = false;

  GLfloat v_move = 0.3;

  switch (key) {
  case 27: 		// Escape
  case 'q':
    cgDestroyProgram(cg_vertex_program);
    cgDestroyProgram(cg_fragment_program);
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


// reshape function:
void reshape(int width, int height) {
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  float aspect_ratio = float(width) / float(height);
  float field_of_view = 50.0;
  gluPerspective(field_of_view, aspect_ratio, 0.1, 100.0);
  glViewport(0, 0, width, height);
  glMatrixMode(GL_MODELVIEW);
}



static int x_old = 0;
static int y_old = 0;
static bool moving = false;

void mouse_func(int button, int state, int x, int y) {

  if((button == GLUT_LEFT_BUTTON) && (state == GLUT_DOWN)) {
    moving = true;
    x_old = x;
    y_old = y;
  }
  if((button == GLUT_LEFT_BUTTON) && (state == GLUT_UP)) {
    moving = false;
  }
}

void mouse_motion(int x, int y) {

  if(moving) {

    camera_angle_theta += 0.1 * GLfloat(y_old - y);
    if(camera_angle_theta > M_PI - 0.1) camera_angle_theta = M_PI - 0.1;
    if(camera_angle_theta < 0.1) camera_angle_theta = 0.1;

    camera_angle_phi += 0.01 * GLfloat(x_old - x);
    if(camera_angle_phi > 2.0 * M_PI) camera_angle_phi -= 2.0 * M_PI;
    if(camera_angle_phi < 0.0 * M_PI) camera_angle_phi += 2.0 * M_PI;

    camera_position[0] = camera_radius * sin(camera_angle_theta) * sin(camera_angle_phi);
    camera_position[1] = camera_radius * cos(camera_angle_theta);
    camera_position[2] = camera_radius * sin(camera_angle_theta) * cos(camera_angle_phi);

    x_old = x;
    y_old = y;
  
    glutPostRedisplay();
  }
}


// idle function:
void idle() {

  teapot_angle += 0.001;
  if(teapot_angle > 2 * M_PI) teapot_angle -= 2 * M_PI;

 
  glutPostRedisplay();
}


// display function:
void display() {

  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  glMatrixMode(GL_MODELVIEW);

  draw_environment();

  cgGLBindProgram(cg_vertex_program);
  checkForCgError("binding vertex program");

  cgGLEnableProfile(cg_vertex_profile);
  checkForCgError("enabling vertex profile");

  cgGLBindProgram(cg_fragment_program);
  checkForCgError("binding fragment program");

  cgGLEnableProfile(cg_fragment_profile);
  checkForCgError("enabling fragment profile");

  glPushMatrix();

    gluLookAt(camera_position[0], camera_position[1], camera_position[2],    // camera Position
	    0.0, 0.0, 0.0,   				         	     // view Center
            0.0, 1.0, 0.0);			        	    	     // up vector

    //glTranslatef(0.0, -1.0, 0.0);
    
    // Diese 3 Zeilen für den Teapot einkommentieren:
//    glRotatef(10, 1, 0, 0);
//    glRotatef(teapot_angle  / (2*M_PI) * 360, 0, 1, 0);
//    glRotatef(-90, 1, 0, 0);

    cgGLSetStateMatrixParameter(cg_vertex_param_modelview_proj, CG_GL_MODELVIEW_PROJECTION_MATRIX, CG_GL_MATRIX_IDENTITY);
    checkForCgError("set parameter modelview_proj"); 

    cgGLSetStateMatrixParameter(cg_vertex_param_modelview, CG_GL_MODELVIEW_MATRIX, CG_GL_MATRIX_IDENTITY);
    checkForCgError("set parameter modelview"); 

    cgGLSetParameter3fv(cg_vertex_param_camera_position, camera_position);

    cgGLSetParameter1f(cg_vertex_param_camera_angle_phi, camera_angle_phi);
    cgGLSetParameter1f(cg_vertex_param_camera_angle_theta, camera_angle_theta);
    cgGLSetParameter1f(cg_vertex_param_camera_radius, camera_radius);

    cgGLSetParameter1f(cg_fragment_param_reflectivity, reflectivity);

    cgGLSetTextureParameter(cg_fragment_param_texture_map, 0);
    cgGLSetTextureParameter(cg_fragment_param_environment_map, environment_map[0]);

    glColor4f(0.2, 0.3, 0.8, 1.0);

    glutSolidSphere(2.0, 30, 30);

    // Der Teapot:
    //glutSolidTeapot(1.0);


  glPopMatrix();

  cgGLDisableProfile(cg_fragment_profile);
  checkForCgError("disabling fragment profile");

  cgGLDisableProfile(cg_vertex_profile);
  checkForCgError("disabling vertex profile");

  glutSwapBuffers();
}


int main(int argc, char **argv) {

  glutInitWindowSize(600, 600);
  glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH);
  glutInit(&argc, argv);

  glutCreateWindow("Environment Mapping");
  glutDisplayFunc(display);
  glutKeyboardFunc(keyboard);
  glutReshapeFunc(reshape);
  glutMouseFunc(mouse_func);
  glutMotionFunc(mouse_motion);

  glClearColor(0.0, 0.0, 0.0, 1.0); 

  cg_context = cgCreateContext();
  checkForCgError("creating context");

  cg_vertex_profile = cgGLGetLatestProfile(CG_GL_VERTEX);
  cgGLSetOptimalOptions(cg_vertex_profile);
  checkForCgError("selecting vertex profile");

  cg_vertex_program = cgCreateProgramFromFile(cg_context, CG_SOURCE, "E24_vertex.cg", 
				cg_vertex_profile, "vertex_shader_for_environment_mapping", NULL);                 

  checkForCgError("creating vertex program from file");
  cgGLLoadProgram(cg_vertex_program);
  checkForCgError("loading vertex program");

  GET_PARAM_VERTEX(modelview_proj);
  GET_PARAM_VERTEX(modelview);
  GET_PARAM_VERTEX(camera_position);
  GET_PARAM_VERTEX(camera_angle_theta);
  GET_PARAM_VERTEX(camera_angle_phi);
  GET_PARAM_VERTEX(camera_radius);


  cg_fragment_profile = cgGLGetLatestProfile(CG_GL_FRAGMENT);
  cgGLSetOptimalOptions(cg_fragment_profile);
  checkForCgError("selecting fragment profile");

  cg_fragment_program = cgCreateProgramFromFile(cg_context, CG_SOURCE, "E24_fragment.cg", 
				cg_fragment_profile, "fragment_shader_for_environment_mapping", NULL);                 

  checkForCgError("creating fragment program from file");
  cgGLLoadProgram(cg_fragment_program);
  checkForCgError("loading fragment program");

  GET_PARAM_FRAGMENT(reflectivity);
  GET_PARAM_FRAGMENT(texture_map);
  GET_PARAM_FRAGMENT(environment_map);
  
 
  create_environment_map();
  glEnable(GL_DEPTH_TEST);

  glutMainLoop();
  return 0;
}


