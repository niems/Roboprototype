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

class Particle
{
private:
	vector<b2ParticleSystem *> p_system; //stores the location of all the particle systems
	vector<b2ParticleDef *> p_def; //stores the location of all the definitions of the particles

public: 
	Particle(b2World *world, sf::RenderWindow &window, sf::Color &fill_color, sf::Color &outline_color, float &gravity_scale, int &radius, int &outline_thickness, int &max_particles, int &lifetime);
	void playerHair(Object &player, Timer &clock, bool &toggle, b2ParticleDef *p_def, b2ParticleSystem *p_system);

};

#endif