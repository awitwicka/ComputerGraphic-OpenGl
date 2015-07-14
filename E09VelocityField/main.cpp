/* 
  Open-GL program using Cg for using simple Cg shaders
  by Daniel Jungblut, IWR Heidelberg University, February 2008
  revised by Susanne Kroemker, IWR Heidelberg University, April 2014
  based on example code of Cg Tutorial (Addison-Wesley, ISBN
  0321194969) by Randima Fernando and Mark J. Kilgard.
*/

#include <math.h>
#include <cstdlib>
#include <stdio.h>

#include <GL/glut.h>
#include <Cg/cg.h>
#include <Cg/cgGL.h>

//#include <GL/glew.h>

GLuint loadPNGTexture (const char *filename);

// Globale Variablen:
static CGcontext   cg_context;
static CGprofile   cg_vertex_profile;
static CGprogram   cg_vertex_program;

static CGprofile   cg_fragment_profile;
static CGprogram   cg_fragment_program;

static CGparameter cg_fragment_param_texture_id;

GLuint texture_id = 0;

bool wireframe = false;
void drawVertexArray(void);
void drawSquare(float x1, float y1, float x2, float y2, int xtiles, int ytiles);

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
    cgDestroyProgram(cg_fragment_program);
    cgDestroyContext(cg_context);
    glDeleteTextures(1, &texture_id);
    exit(0);
  break;
  case ' ':
    wireframe = !wireframe;
    if(wireframe) glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    else glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    glutPostRedisplay();
  break;
  }
}


// display function:
void display() {
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  glEnable(GL_TEXTURE_2D);
  glBindTexture(GL_TEXTURE_2D, texture_id);

  cgGLBindProgram(cg_vertex_program);
  checkForCgError("binding vertex program");

  cgGLEnableProfile(cg_vertex_profile);
  checkForCgError("enabling vertex profile");

  cgGLBindProgram(cg_fragment_program);
  checkForCgError("binding fragment program");

  cgGLEnableProfile(cg_fragment_profile);
  checkForCgError("enabling fragment profile");

  cgGLSetTextureParameter(cg_fragment_param_texture_id, texture_id);
  checkForCgError("setting texture_id parameter");

  cgGLEnableTextureParameter(cg_fragment_param_texture_id);
  checkForCgError("enable texture_id texture");

  // Rendern eines Quadrats:
  /*glBegin(GL_QUADS);
    glTexCoord2f(0.0, 0.0);  glVertex2f(-0.9, -0.9);
    glTexCoord2f(0.0, 1.0);  glVertex2f(-0.9, 0.9);
    glTexCoord2f(1.0, 1.0);  glVertex2f(0.9, 0.9);
    glTexCoord2f(1.0, 0.0);  glVertex2f(0.9, -0.9);
  glEnd();*/

  drawSquare(-0.9, -0.9, 0.9, 0.9, 80, 80);

  cgGLDisableProfile(cg_fragment_profile);
  checkForCgError("disabling fragment profile");

  cgGLDisableProfile(cg_vertex_profile);
  checkForCgError("disabling vertex profile");

  glDisable(GL_TEXTURE_2D);

  glutSwapBuffers();
}

void drawSquare(float x1, float y1, float x2, float y2, int xtiles, int ytiles) {
    float tile_width  = (x2 - x1) / xtiles;
    float tile_height = (y2 - y1) / ytiles;
    float x, y;

    glBegin(GL_QUADS);
        for (y = 0; y < ytiles; y++) {
            for (x = 0; x < xtiles; x++) {
                glTexCoord2f(x * 1/xtiles, y * 1/ytiles);             glVertex2f(x1 + x * tile_width, y1 + y * tile_height); // The bottom left corner  
                glTexCoord2f(x * 1/xtiles, (y + 1) * 1/ytiles);       glVertex2f(x1 + x * tile_width, y1 + (y + 1) * tile_height); // The top left corner  
                glTexCoord2f((x + 1) * 1/xtiles, (y + 1) * 1/ytiles); glVertex2f(x1 + (x + 1) * tile_width, y1 + (y + 1) * tile_height); // The top right corner  
                glTexCoord2f((x + 1) * 1/xtiles, y * 1/ytiles);       glVertex2f(x1 + (x + 1) * tile_width, y1 + y * tile_height); // The bottom right corner    
            }
        }
    glEnd();
}


int main(int argc, char **argv) {

  glutInitWindowSize(400, 400);
  glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE);
  glutInit(&argc, argv);

  glutCreateWindow("Shaders and textures");
  glutDisplayFunc(display);
  glutKeyboardFunc(keyboard);

  glClearColor(0.1, 0.2, 0.8, 1.0); 

  cg_context = cgCreateContext();
  checkForCgError("creating context");

  cg_vertex_profile = cgGLGetLatestProfile(CG_GL_VERTEX);
  cgGLSetOptimalOptions(cg_vertex_profile);
  checkForCgError("selecting vertex profile");

  cg_vertex_program = cgCreateProgramFromFile(cg_context, CG_SOURCE, "E09_vertex.cg", 
						cg_vertex_profile, "vertex_shader", NULL);                 

  checkForCgError("creating vertex program from file");
  cgGLLoadProgram(cg_vertex_program);
  checkForCgError("loading vertex program");



  cg_fragment_profile = cgGLGetLatestProfile(CG_GL_FRAGMENT);
  checkForCgError("selecting fragment profile");

  cg_fragment_program = cgCreateProgramFromFile(cg_context, CG_SOURCE, "E09_fragment.cg",
						 cg_fragment_profile, "fragment_shader", NULL);
  checkForCgError("creating fragment program from file");
  cgGLLoadProgram(cg_fragment_program);
  checkForCgError("loading fragment program");

  cg_fragment_param_texture_id = cgGetNamedParameter(cg_fragment_program, "texture_id");
  checkForCgError("getting texture_id parameter");

  texture_id = loadPNGTexture("noise.png");

  glutMainLoop();
  return 0;
}
