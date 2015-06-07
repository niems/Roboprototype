#ifndef ACTOR_H
#define ACTOR_H

#include <SFML\Graphics.hpp>
#include <Box2D\Box2D.h>
#include "Timer.h"
#include "Object.h"
#include "Editor.h"
#include "Particle.h"
#include "Health.h"

class Actor
{
private:
	Health *health_bar;
	Object *entity; //holds the box2d body and sprite
	
	sf::Texture *texture; //holds entity texture
	sf::Vector2f velocity; //actor velocity
	bool alive;

	//should be able to remove this with a sensor
	Timer jump_clock; //used only for jumping
	Timer clock; //used for commands
	Timer death_clock; //used for when the player dies before the respawn

public:
	Actor(sf::RenderWindow &window, b2World *world, int current_index, int body_type, int shape_type);

	sf::Texture* getTexture(); //returns the entity texture
	Object* getEntity(); //returns the entity
	Health* getHealthBar(); 
	Timer* getDeathClock(); //returns the death clock

	bool isAlive(); //returns if the entity is alive

	void updateClocks(); //updates all clocks
	void commandUpdate(sf::Vector2f &mouse_pos);
	void contactUpdate(b2World *world, Particle &particles);

	void setAlive(bool status);
	void death(); //sets everything so the character is dead
	void respawn(sf::RenderWindow &window, b2World *world, Editor &editor, Camera &view, Object &player, string &file_name); //resets everything for the character respawn
	//loadFile(sf::RenderWindow &window, b2World *world, Camera &view, Object &player, string &file_name);

	

};


#endif