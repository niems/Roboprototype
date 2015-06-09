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
	Object *entity_contact;
	b2Vec2 *entity_contact_pos; //the contact position of the entity
	
	sf::Texture *texture; //holds entity texture
	sf::Vector2f velocity; //actor velocity
	bool alive;
	bool level_complete;

	//should be able to remove this with a sensor
	Timer jump_clock; //used only for jumping
	Timer clock; //used for commands
	Timer contact_clock; //used for updating contacts
	Timer death_clock; //used for when the player dies before the respawn

public:
	Actor(sf::RenderWindow &window, b2World *world, int current_index, int body_type, int shape_type);

	sf::Texture* getTexture(); //returns the entity texture
	Object* getEntity(); //returns the entity
	Health* getHealthBar(); 
	Timer* getDeathClock(); //returns the death clock

	Object* getEntityContact(); 
	b2Vec2* getEntityContactPos(); //returns the position of the object contacting the player

	bool isAlive(); //returns if the entity is alive
	bool isLevelComplete(); //returns if the level is complete

	void updateClocks(); //updates all clocks
	void commandUpdate(sf::Vector2f &mouse_pos);
	int contactUpdate(sf::RenderWindow &window, b2World *world, Editor &editor, Camera &view, Particle &particles);

	void setAlive(bool status);
	void setLevelComplete(bool status); //determines if the level is complete

	void death(); //sets everything so the character is dead
	void respawn(sf::RenderWindow &window, b2World *world, Editor &editor, Camera &view, Object &player, string &file_name); //resets everything for the character respawn
	void loadNextLevel(sf::RenderWindow &window, b2World *world, Editor &editor, Camera &view);	

};


#endif