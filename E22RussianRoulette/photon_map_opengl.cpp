/*!
*	@file	photon_map_opengl.cpp
*	@brief	OpenGL/GLUT stuff for creating a photon map with OpenGL
*	@author Bastian Rieck, IWR Heidelberg, June 2010
*       
*       updated by Susanne Kroemker, IWR, Heidelberg University, June 18, 2014
*
*
*	The light source is positioned in the middle of the room and emits
*	photons in _all_ directions. The sphere will always be shown for
*	visualization purposes. All colours are created by the photons.
*
*	Keyboard shortcuts:
*		*	TAB toggles between showing and hiding the walls.
*		*	+/- increase/decrease the number of emitted photons by 10000
*		*	r resets the photon map and creates a new one, but does not
*			change the number of photons
*/

#if defined(__APPLE__) || defined(MACOSX)
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

#include <stdlib.h>

#include <iostream>

#include "photons.h"

// Photon map variables

photon_manager photons;
const size_t INITIAL_NUM_PHOTONS	= 200000;
size_t num_photons			= INITIAL_NUM_PHOTONS;

// Window-related variables; don't change anything here...

const char* w_name = "Photon Mapping with OpenGL";

const double w_z_near = 0.01;
const double w_z_far = 100.0;

int w_self	= 0;
int w_width	= 600;
int w_height	= 600;

bool show_walls = false;

/*!
*	Initializes OpenGL/GLUT and the photon map.
*/
void w_init(int argc, char* argv[])
{
	srandom(time(NULL));

	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);

	glutInitWindowSize(w_width, w_height);
	glutInitWindowPosition(10, 10);
	w_self = glutCreateWindow(w_name);

	glClearColor(0.0,0.0,0.0,1.0);

	glShadeModel(GL_SMOOTH);
	glEnable(GL_POLYGON_SMOOTH);
	glEnable(GL_DEPTH_TEST);

	photons.map_create(num_photons);
}

/*!
*	Destroys the current window.
*/
void w_destroy(void)
{
	glutDestroyWindow(w_self);
}

// Custom display function for the window. This is where
// the fun starts.
void w_display(void)
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glColor3f(1.0, 1.0, 1.0);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glPushMatrix();

	gluLookAt(	0,
			0,
			3,
			0,
			0,
			-1,
			0.0,
			1.0,
			0.0);

	// Draw the walls
	if(show_walls)
	{
		glPushMatrix();
		glBegin(GL_QUADS);

		// back wall
		glColor3f(1.0, 0.0, 0.0);
		glVertex3f(-1.05,-1.05, -1.05);
		glVertex3f( 1.05,-1.05, -1.05);
		glVertex3f( 1.05, 1.05, -1.05);
		glVertex3f(-1.05, 1.05, -1.05);

		// left wall
		glColor3f(0.0, 1.0, 0.0);
		glVertex3f(-1.0,-1.0, 1.0);
		glVertex3f(-1.0,-1.0,-1.0);
		glVertex3f(-1.0, 1.0,-1.0);
		glVertex3f(-1.0, 1.0, 1.0);

		// right wall
		glColor3f(0.0, 0.0, 1.0);
		glVertex3f( 1.0,-1.0, 1.0);
		glVertex3f( 1.0,-1.0,-1.0);
		glVertex3f( 1.0, 1.0,-1.0);
		glVertex3f( 1.0, 1.0, 1.0);

		// floor
		glColor3f(0.5, 0.5, 0.5);
		glVertex3f(-1.0,-1.0, 1.0);
		glVertex3f( 1.0,-1.0, 1.0);
		glVertex3f( 1.0,-1.0,-1.0);
		glVertex3f(-1.0,-1.0,-1.0);

		// ceiling
		glColor3f(0.5, 0.5, 0.5);
		glVertex3f(-1.0, 1.0, 1.0);
		glVertex3f( 1.0, 1.0, 1.0);
		glVertex3f( 1.0, 1.0,-1.0);
		glVertex3f(-1.0, 1.0,-1.0);

		glEnd();
		glPopMatrix();
	}

	// Draw the sphere
	glPushMatrix();
	glColor3f(0.0, 0.0, 0.0);
	glTranslatef(sphere_center[0], sphere_center[1], sphere_center[2]);
	glutSolidSphere(sphere_radius, 50, 50);
	glPopMatrix();

	photons.map_draw();

	glPopMatrix();
	glutSwapBuffers();
}

/*!
*	Resets the window.
*/
void w_reset(void)
{
	photons.map_create(num_photons);
	glutPostRedisplay();
}

/*!
*	Reshape function; aspect ratio is recalculated so that the scene does
*	not look distorted.
*/
void w_reshape(GLsizei width, GLsizei height)
{
	glViewport(0,0,width,height);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	w_width = width;
	w_height = height;

	gluPerspective(45.0, width/static_cast<GLfloat>(height), w_z_near, w_z_far);
}

/*!
*	Keyboard callback function.
*/
void k_callback(unsigned char key, int x, int y)
{
	switch(key)
	{
		case 'q':
			w_destroy();
			exit(0);

		case '\t':
			show_walls = !show_walls;
			break;

		case '+':
		case '-':
			num_photons += (key == '+'? 10000 : -10000);
			w_reset();
			std::cout << "No. of photons: " << num_photons << "\n";
			break;

		case ' ':
		case 'r':
			w_reset();
			break;

		default:
			break;

	}

	glutPostRedisplay();
}

/*!
*	Main function. Initializes the photon map.
*/
int main(int argc, char* argv[])
{
	w_init(argc, argv);

	glutSetWindow(w_self);
	glutDisplayFunc(w_display);
	glutReshapeFunc(w_reshape);
	glutKeyboardFunc(k_callback);

	glutPostRedisplay();
	glutMainLoop();
	return(0);
}
