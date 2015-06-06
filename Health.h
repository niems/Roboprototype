#ifndef HEALTH_H
#define HEALTH_H

#include <SFML\Graphics.hpp>
#include "Actor.h"

class Health
{
private:
	sf::RectangleShape *bar;
	sf::Vector2f offset;
	int current_health;
	int max_health;
	

public:
	Health(Actor *entity, int current_hp, int max_hp); //uses entity for size and position

	//only resize the health bar when healed or damaged
	void updateBar(sf::Vector2f &pos); //updates the bar based on the position and the y offset

	void damage(int damage_amount); //amount of damage the player takes

	sf::RectangleShape* getBar();
};

#endif