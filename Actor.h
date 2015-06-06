#ifndef ACTOR_H
#define ACTOR_H

#include <SFML\Graphics.hpp>
#include <Box2D\Box2D.h>
#include "Timer.h"
#include "Object.h"
#include "Editor.h"
#include "Particle.h"

class Actor
{
private:
	Object *entity; //holds the box2d body and sprite
	sf::Texture *texture; //holds entity texture
	sf::Vector2f velocity; //actor velocity
	bool alive;

	//should be able to remove this with a sensor
	Timer jump_clock; //used only for jumping
	Timer clock; //used for commands

public:
	Actor(sf::RenderWindow &window, b2World *world, int current_index, int body_type, int shape_type);

	sf::Texture* getTexture(); //returns the entity texture
	Object* getEntity(); //returns the entity
	bool isAlive(); //returns if the entity is alive

	void updateClocks(); //updates all clocks
	void commandUpdate(sf::Vector2f &mouse_pos);
	void contactUpdate(b2World *world, Particle &particles);

};


#endif