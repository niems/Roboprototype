#ifndef PARTICLE_H
#define PARTICLE_H

#include <SFML\Graphics.hpp>
#include <Box2D\Box2D.h>
#include <vector>
#include <iostream>
#include "Object.h"
#include "Timer.h"
using namespace std;

//maybe each instance of this is a new category.
//so there would be an instance for the player hair, and
//another for blood spatter.

class Particle
{
private:
	vector<sf::CircleShape> shape; //particle shape used. One per effect used with setPosition to update
	vector<sf::Vector2f> position; //stores the position of each particle animation.
	vector<Timer> clock; //stores the clock for each particle animation
	vector<float> duration; //how long the effect lasts before the particles are deleted
	vector<int> type; //current particle type
	
	b2ParticleSystem *p_system; //stores the location of all the particle systems
	b2ParticleDef *p_def; //stores the location of all the definitions of the particles
//	int type; //current type of particles created

public: 
	enum TYPE{HAIR, BLOOD_SPLATTER};

	Particle(b2World *world, sf::RenderWindow &window);

	void playerHair(b2World *world, Object &player);
	void bloodSplatter(); //used for the blood splatter instance
	void updateParticles(); //updates all particles based on their type`
};

#endif