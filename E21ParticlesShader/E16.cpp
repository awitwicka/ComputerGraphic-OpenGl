/***********************************************
  Musterlösung zu Aufgabe E07 - Zeitmessung
  by Daniel Jungblut, IWR Heidelberg, Mai 2008 - daniel.jungblut@iwr.uni-heidelberg.de
  using code of Anja Schäfer
************************************************/

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
#include <cstdio>
#include <cmath>
#include <cstdlib>
#include <list>
#include <ctime>

#include "mytime.h"

static CGcontext   cg_context;
static CGprofile   cg_vertex_profile;
static CGprogram   cg_vertex_program;

static CGparameter cg_parameter_vertex_velocity;
static CGparameter cg_parameter_vertex_time;
static CGparameter cg_parameter_vertex_lifeTime;
static CGparameter cg_parameter_vertex_isAlieve;

GLuint loadPNGTexture (const char *filename);

GLuint water_tex;
GLuint fire_tex;

// Initialisierung aller Routinen:
void display(void);
void idle(void);
void init();
int main(int, char **);
void mouse (int button, int state, int x, int y);
void keyboard(unsigned char key, int x, int y);	

// Variablen für die fps Bestimmung:
float t;
int frames = 0;
float timer = 0;
float fps = 0.0;

//Particle struct
struct Particle {
    float position[3];
    float velocity[3];
    float t;
    float lifeTime;
    bool isAlive;
};

std::list<Particle> water_list;

//how many particles are emitted per second
float particles_per_Second = 2000;

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

float myRand(float lower, float upper)
{
    return lower + ((float)rand() / RAND_MAX) * (upper - lower);
}

void drawParticles(std::list<Particle> &drawList, GLuint texture)
{

    static float overTime = 0.0;
    int particlesToEmit = particles_per_Second * t + overTime;
    overTime += particles_per_Second * t - particlesToEmit;


    glBindTexture(GL_TEXTURE_2D, texture);
    glTexEnvi(GL_POINT_SPRITE, GL_COORD_REPLACE, GL_TRUE);


    for(std::list<Particle>::iterator it = drawList.begin(); it != drawList.end(); ++it)
    {
        //update and draw particles if alive
        if(it->isAlive)
        {

            it->t += t;
            if(it->t > it->lifeTime)
                it->isAlive = false;


            glPointSize(10.0 * (it->lifeTime - it->t) / it->lifeTime + 0.1);

            glColor4f(1 - (it->lifeTime - it->t) / it->lifeTime,
                      1-(it->lifeTime - it->t) / it->lifeTime
                      ,1.0f, 1.0);
            glBegin(GL_POINTS);

            glVertex3f( it->position[0] + it->t * it->velocity[0],
                    it->position[1] + it->t * it->velocity[1] - 0.5 * 9.81 * it->t * it->t,
                    it->position[2] + it->t * it->velocity[2]);

            glEnd();

        }
        //revive particle if necessary
        else if(particlesToEmit > 0)
        {
            --particlesToEmit;
            it->isAlive = true;
            it->t = 0;
            it->lifeTime = myRand(1.5,2.5);

            it->position[0] = myRand(-0.5,0.5);
            it->position[1] = 0;
            it->position[2] = myRand(-0.5,0.5);

            it->velocity[0] = myRand(-2, 2);
            it->velocity[1] = myRand(5.0,10.0);
            it->velocity[2] = myRand(-2, 2);

  cgGLEnableTextureParameter(cg_parameter_vertex_velocity);
  glVertex2f(0.0f, 0.0f);
  glTexCoord3fv(it->velocity); 
  cgGLDisableTextureParameter(cg_parameter_vertex_velocity);

        }
    }
    //create new particles if necessary
    for(int i = 0; i < particlesToEmit; ++i)
    {
        Particle neu;

        neu.isAlive = true;
        neu.t = 0;
        neu.lifeTime = myRand(1.5,2.5);

        neu.position[0] = myRand(-0.5,0.5);
        neu.position[1] = 0;
        neu.position[2] = myRand(-0.5, 0.5);

        neu.velocity[0] = myRand(-2, 2);
        neu.velocity[1] = myRand(5.0,10.0);
        neu.velocity[2] = myRand(-2, 2);

  cgGLEnableTextureParameter(cg_parameter_vertex_velocity);
  glVertex2f(0.0f, 0.0f);
  glTexCoord3fv(neu->velocity); 
  cgGLDisableTextureParameter(cg_parameter_vertex_velocity);

        water_list.push_back(neu);
    }

}

void display(void) {

  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  cgGLBindProgram(cg_vertex_program);
  checkForCgError("binding vertex program");

  cgGLEnableProfile(cg_vertex_profile);
  checkForCgError("enabling vertex profile");
  // parameter

  //cgGLSetParameter1f(cg_parameter_vertex_scale_factor, 0.7);
  //cgGLSetParameter1f(cg_parameter_vertex_rotation, rotation);

  // Zeit die seit dem rendern des letzten Bildes vergangen ist:
  cgGLEnableTextureParameter(cg_parameter_vertex_time);
  t = diff_seconds();
  glVertex2f(0.0f, 0.0f);
  glTexCoord1f(Particle->t);  
  cgGLDisableTextureParameter(cg_parameter_vertex_time);
  // Berechnung der Framerate mit Hilfe der Zeitmessungsfunktion:
  frames++;
  timer += t;
  if (timer > 1.0) {
    printf("Frames per second: %i\n", frames);
    timer -= 1.0;		
    frames = 0;

    printf("Number of particles: %d\n", water_list.size());
  }
  //cgGLEnableTextureParameter(cg_parameter_vertex_velocity);
  drawParticles(water_list,water_tex);
  //glVertex2f(0.0f, 0.0f);
  //glTexCoord3fv(Particle->velocity); 
  //cgGLDisableTextureParameter(cg_parameter_vertex_velocity);



  cgGLDisableProfile(cg_vertex_profile);
  checkForCgError("disabling vertex profile");

  glutSwapBuffers();
}

void idle(void) {
  glutPostRedisplay();
}


// keyboard callback:
void keyboard(unsigned char key, int x, int y) {
  switch (key) {
  case 27: 		// Escape
  case 'q':
    exit(0);
  break;
  }
}


void init() {
  
  glClearColor (0.0,0.0,0.0,0.0);
  glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  gluPerspective(50,1.5,1,40);
  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();
  gluLookAt(0,2,10,0,2,0,0,1,0);

  //Enable Point sprites
  glEnable(GL_POINT_SPRITE);
  glEnable(GL_TEXTURE_2D);

  //set texture
  water_tex = loadPNGTexture("particle_water.png");

  water_list.clear();


}


int main(int argc,char** argv) {
    diff_seconds();
    srand(time(NULL));
  
  glutInit(&argc, argv);
  glutInitDisplayMode( GLUT_DEPTH | GLUT_DOUBLE |GLUT_RGB); 
  glutInitWindowSize (900,600);
  glutInitWindowPosition (240, 40);
  glutCreateWindow ("E16 - Partikelsystem");

  glutKeyboardFunc(keyboard);

  glEnable(GL_LIGHTING);
  glEnable(GL_LIGHT0);
  glEnable(GL_COLOR_MATERIAL);
	
  init();
  glutDisplayFunc(display);  

  glutIdleFunc(idle);
  
  glEnable(GL_DEPTH_TEST);
/********************************************/

  cg_context = cgCreateContext();
  checkForCgError("creating context");

  cg_vertex_profile = cgGLGetLatestProfile(CG_GL_VERTEX);
  cgGLSetOptimalOptions(cg_vertex_profile);
  checkForCgError("selecting vertex profile");

  cg_vertex_program = cgCreateProgramFromFile(cg_context, CG_SOURCE, "vertex.cg", 
      cg_vertex_profile, "more_complex_vertex_shader", NULL);                 

  checkForCgError("creating vertex program from file");
  cgGLLoadProgram(cg_vertex_program);
  checkForCgError("loading vertex program");
/**************************************************/
  cg_parameter_vertex_velocity = cgGetNamedParameter(cg_vertex_program, "Texture0"); 
  cg_parameter_vertex_time = cgGetNamedParameter(cg_vertex_program, "Texture1");


  glutMainLoop(); 
 
  return 0;
}

