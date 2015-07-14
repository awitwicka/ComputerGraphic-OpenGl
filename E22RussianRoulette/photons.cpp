/*!
*	@file	photons.cpp
*	@brief	Skeleton implementation for creating photon maps
*	@author Bastian Rieck, IWR Heidelberg, June 2010
* 
*	updated by Susanne Kroemker, IWR, Heidelberg University, June 18, 2014
*/

#if defined(__APPLE__) || defined(MACOSX)
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

#include <math.h>
#include <stdlib.h>

#include "photons.h"

/*!
*	Initializes a new photon map that contains num_photons photons. You do
*	not need to change anything in this function.
*/
void photon_manager::map_create(unsigned int num_photons)
{
	// All stored events will be REMOVED here...
	events.clear();

	photon p;
	for(unsigned int i = 0; i < num_photons; i++)
	{
		// Initial direction

		double theta	= random(0, M_PI);
		double phi	= random(0, 2*M_PI);

		p.d[0] = sin(theta)*sin(phi);
		p.d[1] = cos(theta);
		p.d[2] = sin(theta)*cos(phi);

		// Initial position

		p.p[0] = light_pos[0];
		p.p[1] = light_pos[1];
		p.p[2] = light_pos[2];

		// Initial colour

		p.c[0] = light_col[0];
		p.c[1] = light_col[1];
		p.c[2] = light_col[2];

		// Follow the photon through the scene, trying to determine
		// whether it hits anything or not.

		p.age = 0;
		photon_trace(p);
	}
}

/*!
*	Draws the photon map, i.e., all recorded events. You do not need to
*	change this function.
*/
void photon_manager::map_draw(void)
{
	glPushMatrix();
	glPointSize(2.0);
	glBegin(GL_POINTS);
	for(size_t i = 0; i < events.size(); i++)
	{
		glColor3f( events[i].c[0], events[i].c[1], events[i].c[2]);
		glVertex3f(events[i].p[0], events[i].p[1], events[i].p[2]);
	}
	glEnd();
	glPopMatrix();
}

/*!
*	Traces a photon p through the scene, thereby generating photon map
*	events of all kinds.
*/
void photon_manager::photon_trace(photon p)
{
	if(p.age > max_age)
		return;

	// Compute the probabilities for the photons to be reflected etc.

	double Pr_plane = 0.8;
	double Pd_plane = Pr_plane*(plane_d[0]+plane_d[1]+plane_d[2])/(plane_d[0]+plane_d[1]+plane_d[2]+plane_s[0]+plane_s[1]+plane_s[2]);
	double Ps_plane = Pr_plane-Pd_plane;

	double Pr_sphere = 1.0;
	double Pd_sphere = (sphere_d[0]+sphere_d[1]+sphere_d[2])/(sphere_d[0]+sphere_d[1]+sphere_d[2]+sphere_s[0]+sphere_s[1]+sphere_s[2]);
	double Ps_sphere = Pr_sphere-Pd_sphere;

	/*
		TODO: Check for collisions with the sphere here and call
		`photon_collide()` with proper parameters.
	*/
		Vector<3> normal;
		Vector<3> colour;
		normal[0]=sphere_center[0]-p.p[0];
		normal[1]=sphere_center[1]-p.p[1];
		normal[2]=sphere_center[2]-p.p[2];
		double sphere_distance_origin = sqrt((0.5*0.5)+(0.75*0.75)+(0.5*0.5))-sphere_radius;
		double t = (sphere_distance_origin-normal*p.p)/(normal*p.d);
		if(t > 0)
		{
			// Check whether the point is actually in the bounds of
			// the plane
				Vector<3> pt  = p.p + p.d*t;
			if(	fabs(pt[0]) <= 1.0 &&
				fabs(pt[1]) <= 1.0 &&
				fabs(pt[2]) <= 1.0)
			{
				p.p = pt;
				photon_collide(p, Pd_plane, Ps_plane, normal, colour, true);
			}
		}		



	// Check for collisions with the "walls"
	for(int i = 0; i < 5*3; i += 3)
	{
		Vector<3> normal;
		Vector<3> colour;

		normal[0] = plane_normals[i];
		normal[1] = plane_normals[i+1];
		normal[2] = plane_normals[i+2];

		colour[0] = plane_colours[i];
		colour[1] = plane_colours[i+1];
		colour[2] = plane_colours[i+2];

		if((normal*p.d) != 0) //is not perpendicular vector
		{
			double t = (plane_distance_origin-normal*p.p)/(normal*p.d);
			if(t > 0)
			{
				// Check whether the point is actually in the bounds of
				// the plane

				Vector<3> pt  = p.p + p.d*t;
				if(	fabs(pt[0]) <= 1.0 &&
					fabs(pt[1]) <= 1.0 &&
					fabs(pt[2]) <= 1.0)
				{
					p.p = pt;   //AAA tu jest :D
					photon_collide(p, Pd_plane, Ps_plane, normal, colour); // what o.o? missing show reflection thing? czy my mamy pozycje scian podana? (patrz wyzej)
				}
			}
		}
	}
}

/*!
*	Handles a photon collision: Photon p is colliding with an object
*	identified by its normal and its colour. The probabilities for diffuse
*	and specular reflection are taken into account and a new photon event
*	is generated.
*
*	NOTE: You need to change this function in order to solve the exercise.
*/
void photon_manager::photon_collide(const photon& p, double Pd, double Ps, Vector<3> normal, Vector<3> colour, bool show_reflection) 
{
	double xi = static_cast<double>(rand())/static_cast<double>((RAND_MAX));

	photon q;
	photon_event e;

	// Choose the type of collision response, i.e.: diffuse/specular reflection or
	// absorption
	if(xi <= Pd)
	{
		/*
		*	TODO:
		*
		*	Handle diffuse reflection here:
		*		- choose a random direction for the _new_
		*		  photon
		*		- add an event to the photon map
		*		- trace the new photon
		*/
		double theta	= random(0, M_PI);
		double phi	= random(0, 2*M_PI);
		q.d[0] = sin(theta)*sin(phi);
		q.d[1] = cos(theta);
		q.d[2] = sin(theta)*cos(phi);
		//czy ja mam zmieniac kolor -> chyba tak zczyta ze sciany jak eszcze nie zczytany
		q.c = colour;
		q.p = p.p;
		//wrzuc na kolejke etc
		e.c = q.c;
		e.p = q.p;
		events.push_back(e);
		photon_trace(q);
	}

	else if(xi > Pd && xi <= Ps+Pd)
	{
		/*
		*	TODO:
		*
		*	Handle specular reflection here:
		*		- reflect the photon on the surface
		*		- add an event to the photon map
		*		- trace the new photon
		*/
		//double dot = (p.d[0]*normal[0]+p.d[0]*normal[0]+p.d[0]*normal[0]);
		//q.d = p.d - (normal*2*(p.d[0]*normal[0]+p.d[0]*normal[0]+p.d[0]*normal[0])); //v-2n(vdotn)	
		double theta	= random(0, M_PI);
		double phi	= random(0, 2*M_PI);
		q.d[0] = sin(theta)*sin(phi);
		q.d[1] = cos(theta);
		q.d[2] = sin(theta)*cos(phi);
		//czy ja mam zmieniac kolor -> chyba tak zczyta ze sciany jak eszcze nie zczytany
		q.c = colour;
		q.p = p.p;
		//wrzuc na kolejke etc
		e.c = q.c;
		e.p = q.p;
		events.push_back(e);
		photon_trace(q);
		photon_trace(q);
	}
	else
	{
		// absorption: Do not trace the photon any further
		e.c = p.c;
		e.p = p.p;
		events.push_back(e); //-> if its absorbed it shouldnt displaywhite dot?// push != pop tylko po co ja mam rysowac cos co jest zabsorbowane????
	}
}

/*!
*	Generates a random double between min and max. Assumes that the random
*	number generator has been set up properly, e.g., by calling
*	srandom(time(NULL));
*/
double photon_manager::random(double min, double max)
{
	return(::random()/(static_cast<double>(RAND_MAX)+1.0)*(max-min)+min);
}
