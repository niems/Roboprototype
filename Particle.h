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
	vector<b2ParticleSystem *> particle_systems; //used to reference the particle systems
	vector<sf::CircleShape> shapes; //particle shape used. One per effect used with setPosition to update
	vector<sf::Vector2f> positions; //stores the position of each particle animation.
	vector<Timer> system_clocks; //stores the clock for each particle animation
	vector<float> durations; //how long the effect lasts before the particles are deleted
	vector<int> types; //current particle type

public: 
	enum TYPE{BLOOD_SPLATTER, HAIR, EXPLOSION, SPAWN};

	Particle(b2World *world, sf::RenderWindow &window);

	void updateClocks(); //updates all clocks at once

	void playerHair(b2World *world, Object &player);
	void bloodSplatter(b2World *world, const sf::Vector2f &pos); //used for the blood splatter instance
	void explosion(b2World *world, const sf::Vector2f &pos); //creates an explosion at the position
	void spawn(b2World *world, const sf::Vector2f &pos); //creates an explosion when the player spawns in a new level

	sf::CircleShape& getShape(int type); //returns the shape for the corresponding particle type.
	vector<b2ParticleSystem *>& getParticleSystems();
	vector<Timer>& getSystemClocks();
};

#endif