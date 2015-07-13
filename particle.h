#include <GL/glut.h>
#include <iostream>
#include <cmath>
#include <list>
#include <cstdlib>

//functions
GLuint loadPNGTexture (const char *filename);

using namespace std;

class Particle{
  public:
	float pos[3];
	float direction[3];
	float size;             
        float color[4]; 
	float age;
	float life_time;
};

class ParticleSystem{
  private:
	list<Particle> particles;
	int n;
	int r, g, b;
        float a;
	GLuint texture; 

        float RandomGenerator_f(float low, float max);

  public:
	ParticleSystem();
	void Initialization(int particles_no, int n, int r, int g, int b, float a);
	//~ParticleSystem();
	void Draw(float elapse_time, float x, float y, float z);
};


ParticleSystem::ParticleSystem(): texture(0) {}

//ParticleSystem::~ParticleSystem() {

//}

void ParticleSystem::Initialization(int texture_no, int N, int R, int G, int B, float A) {
  if (texture_no == 1) {
    texture = loadPNGTexture("sparkle.png");  //load texture depending on mode? 
  } else {
    texture = loadPNGTexture("fog.png"); 
  }
  n=N;
  r=R;
  g=G;
  b=B;
  a=A;
}

void ParticleSystem::Draw(float elapse_time, float x, float y, float z) {
  glDisable(GL_LIGHTING);
  glEnable(GL_BLEND);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE); //for transparency
  //particle options
  glEnable(GL_POINT_SPRITE);
  glTexEnvf(GL_POINT_SPRITE, GL_COORD_REPLACE, GL_TRUE);   
  glEnable(GL_TEXTURE_2D);
  glEnable(GL_PROGRAM_POINT_SIZE); //in init?
  glBindTexture(GL_TEXTURE_2D, texture);
  //glEnable(GL_POINT_SPRITE_ARB);
  //glTexEnvi(GL_POINT_SPRITE_ARB, GL_COORD_REPLACE_ARB, GL_TRUE);
  //glEnable(GL_VERTEX_PROGRAM_POINT_SIZE);
  glPointSize(15.0f);
  //glDepthMask(GL_FALSE);

  float S=1; //spread
  float R=1; //initial circle area radius
  float particles_per_Second = n;
  float particles_per_Frame = particles_per_Second*elapse_time; //number of new particles we need to generate per frame such that n in totall will exist

  for (list<Particle>::iterator it = particles.begin(); it != particles.end(); it++) {
    glBegin(GL_POINTS);
    if (it->age <= it->life_time) {
	it->age += elapse_time;
	//calculate new postion/direction in shader
	glColor4fv(it->color);
	glMultiTexCoord3f(GL_TEXTURE1, it->direction[0], it->direction[1], it->direction[2]);
        glMultiTexCoord2f(GL_TEXTURE2, it->age, it->life_time);
	glVertex3f(it->pos[0], it->pos[1], it->pos[2]);
    } else if(particles_per_Frame > 0) {
	particles_per_Frame--;
	it->age = 0;
	it->pos[0] = RandomGenerator_f(-R,R)+x; //scatered area of initial spawn -> circle
        it->pos[1] = RandomGenerator_f(-sqrt(R*R-(it->pos[0]-x)*(it->pos[0]-x)),sqrt(R*R-(it->pos[0]-x)*(it->pos[0]-x)))+y;
        it->pos[2] = z;
    	it->direction[0] = RandomGenerator_f(-(R+S),(R+S)); //how they spread ->circles
        it->direction[1] = RandomGenerator_f(-sqrt((R+S)*(R+S)-it->direction[0]*it->direction[0]),sqrt((R+S)*(R+S)-it->direction[0]*it->direction[0]));
        it->direction[2] = RandomGenerator_f(5,10);
	it->color[0] = RandomGenerator_f(0,1)*r;
	it->color[1] = RandomGenerator_f(0,1)*g;
	it->color[2] = RandomGenerator_f(0,1)*b; 
	it->color[3] = a + RandomGenerator_f(-0.1,0.1);
    }
    glEnd();
  }
  for(int i = 0; i < particles_per_Frame; ++i)
  {
    Particle tmp = Particle();
    tmp.pos[0] = RandomGenerator_f(-R,R)+x; //scatered area of initial spawn ->circle
    tmp.pos[1] = RandomGenerator_f(-sqrt(R*R-(tmp.pos[0]-x)*(tmp.pos[0]-x)),sqrt(R*R-(tmp.pos[0]-x)*(tmp.pos[0]-x)))+y;
    tmp.pos[2] = z;
    tmp.direction[0] = RandomGenerator_f(-(R+S),(R+S)); //how they spread -> circles
    tmp.direction[1] = RandomGenerator_f(-sqrt((R+S)*(R+S)-tmp.direction[0]*tmp.direction[0]),sqrt((R+S)*(R+S)-tmp.direction[0]*tmp.direction[0]));
    tmp.direction[2] = RandomGenerator_f(5,10);
    tmp.color[0] = RandomGenerator_f(0,1)*r;
    tmp.color[1] = RandomGenerator_f(0,1)*g;
    tmp.color[2] = RandomGenerator_f(0,1)*b; 
    tmp.color[3] = a + RandomGenerator_f(-0.1,0.1);
    tmp.size;             
    tmp.color; 
    tmp.age = 0;
    tmp.life_time = RandomGenerator_f(2,6); //how long particle exists
    particles.push_back(tmp);
  }

  glDisable(GL_TEXTURE_2D);
  glDisable(GL_BLEND);
  glEnable(GL_LIGHTING);
}

float ParticleSystem::RandomGenerator_f(float low, float max) {
  return low + (float)(rand())/((float)(RAND_MAX/(max-low)));
}

