#ifndef PARTICLE_H
#define PARTICLE_H

#include <SFML\Graphics.hpp>
#include <Box2D\Box2D.h>
#include <vector>
#include "Object.h"
#include "Timer.h"
using namespace std;

const float PIXELS_TO_METERS = 0.03333; //number of meters in one pixel
const float METERS_TO_PIXELS = 30.0; //number of pixels in one meter

//maybe each instance of this is a new category.
//so there would be an instance for the player hair, and
//another for blood spatter.

class Particle
{
private:
	vector<sf::Vector2f> position; //stores the position of each particle animation.
	vector<Timer> clock; //stores the clock for each particle animation
	
	b2ParticleSystem *p_system; //stores the location of all the particle systems
	b2ParticleDef *p_def; //stores the location of all the definitions of the particles
	int type; //current type of particles created

public: 
	enum TYPE{HAIR, BLOOD_SPLATTER};
	Particle(b2World *world, sf::RenderWindow &window, sf::Color &fill_color, sf::Color &outline_color, float &gravity_scale, int &radius, int &outline_thickness, int &max_particles, int &lifetime);
	void playerHair(Object &player, Timer &clock, bool &toggle, b2ParticleDef *p_def, b2ParticleSystem *p_system);

	void bloodSplatter(); //used for the blood splatter instance

};

#endif