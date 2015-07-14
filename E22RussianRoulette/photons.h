/*!
*	@file	photons.h
*	@brief	Class and definitions for handling photons
*	@author Bastian Rieck, IWR Heidelberg, June 2010
*/

#ifndef PHOTONS_H
#define PHOTONS_H

#include <vector>	// This is the vector class of the STL, describing arrays
#include "vector.cpp"	// This is the Vector class, describing n-dimensional vectors

// Constants concerning the photon mapping

const double light_pos[] = {0.0, 0.0, 0.0};
const double light_col[] = {1.0, 1.0, 1.0};

/*!
*	Describes the normals for all walls (except the front, which is
*	missing).
*/
const double plane_normals[]	= {
					 0.0, 0.0,-1.0, // behind
					-1.0, 0.0, 0.0, // left
					 1.0, 0.0, 0.0, // right
					 0.0,-1.0, 0.0, // down
					 0.0, 1.0, 0.0  // up

};

/*!
*	Colours for all walls (except the front wall). This follows the colours
*	defined for the Cornell box.
*/
const double plane_colours[]	= {
					1.0, 0.0, 0.0,
					0.0, 1.0, 0.0,
					0.0, 0.0, 1.0,
					0.5, 0.5, 0.5,
					0.5, 0.5, 0.5
};

const double plane_d[] = {0.7, 0.7, 0.7};	// probability that diffuse
						// reflection happens with the
						// photon when colliding with
						// the plane

const double plane_s[] = {0.1, 0.1, 0.1};	// ditto for specular
						// reflection

const double plane_distance_origin = 1;

// Constants for the sphere

const double sphere_center[]	= {0.5, -0.75, 0.5};
const double sphere_d[]		= {0.4, 0.4, 0.4}; // probabilities for diffuse reflection
const double sphere_s[]		= {0.6, 0.6, 0.6}; // probabilities for specular reflection
const double sphere_radius	= 0.25;

// Constants for all photons

const unsigned int max_age = 4;

/*!
*	Describes a single photon that will be "tracked" through the scene and,
*	ultimately, discarded because of its age.
*
*	There is a shred of social criticism hidden in the previous sentence.
*/
class photon
{
	public:
		Vector<3> p; // position
		Vector<3> d; // direction
		Vector<3> c; // colour = "Energy"

		// Counts the numbers of reflections this photon
		// had to endure.
		unsigned int age;
};

/*!
*	Saves a photon event, i.e. the position and the colour of a reflection etc.
*	so that a global photon map may be drawn.
*/
class photon_event
{
	public:
		Vector<3> p; // position
		Vector<3> c; // colour
};

/*!
*	Manages all photons and saves their data in the photon map.
*/
class photon_manager
{
	public:
		void map_create(unsigned int num_photons);
		void map_draw(void);

	private:
		double random(double min, double max);

		void photon_trace(photon p);
		void photon_collide(const photon& p, double Pd, double Ps, Vector<3> normal, Vector<3> colour, bool show_reflection = true);

		std::vector<photon_event> events;
};

#endif
