#ifndef HEALTH_H
#define HEALTH_H

#include <SFML\Graphics.hpp>

class Health
{
private:
	sf::RectangleShape *bar;
	sf::Vector2f offset;
	int current_health;
	int max_health;

public:
	Health();
	Health(sf::Vector2f &texture_size, sf::Vector2f &pos, int current_hp, int max_hp);
	//only resize the health bar when healed or damaged
	void updateBar(sf::Vector2f &pos); //updates the bar based on the position and the y offset

	void damage(int damage_amount); //amount of damage the player takes
	void heal(int heal_amount); //amount the player is healed

	float colorBar(); //used when the bar changes sizes. Colors the bar based on the percentage of the player's health

	int getMaxHealth();

	sf::RectangleShape* getBar();
};

#endif