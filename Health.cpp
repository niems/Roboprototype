#include "Health.h"

Health::Health()
{
	this->bar = NULL;
	this->current_health = 0;
	this->max_health = 0;
}

Health::Health(sf::Vector2f &texture_size, sf::Vector2f &pos, int current_hp, int max_hp)
{
	sf::Vector2f bar_pos;
	sf::Vector2f bar_size;
	bar_size.x = texture_size.x;
	bar_size.y = texture_size.y / 10.0;

	this->offset.x = bar_size.x / 2.0;
	this->offset.y = (texture_size.y / 2.0) - (bar_size.y / 2.0);
	
	bar_pos = pos;

	this->bar = new sf::RectangleShape();
	this->bar->setSize(bar_size);
	this->bar->setOrigin( 0.0, bar_size.y / 2.0 );
	this->bar->setFillColor( sf::Color(0, 255, 0) );
	this->bar->setOutlineThickness(3);
	this->bar->setOutlineColor( sf::Color(0, 0, 0) );

	this->current_health = current_hp;
	this->max_health = max_hp;
}


void Health::updateBar(sf::Vector2f &pos)
{
	pos.x -= this->offset.x;
	pos.y -= this->offset.y + 15.0;
	this->bar->setPosition(pos);
}

void Health::damage(int damage_amount)
{
	float scale;
	this->current_health -= damage_amount;

	if( this->current_health <= 0 )
	{
		this->current_health = 0;
	}

	scale = this->colorBar();
	this->bar->setScale(scale, 1.0);
}

void Health::heal(int heal_amount)
{
	float scale;
	this->current_health += heal_amount;

	if(this->current_health > this->max_health)
		this->current_health = this->max_health;

	scale = this->colorBar();
	this->bar->setScale(scale, 1.0);
}

float Health::colorBar()
{
	float scale = this->current_health / static_cast<float>(this->max_health);

	if(scale >= 0.7)
		this->bar->setFillColor( sf::Color(0, 255, 0) );

	else if(scale >= 0.5)
		this->bar->setFillColor( sf::Color(255, 255, 0) );

	else
		this->bar->setFillColor( sf::Color(255, 0, 0) );

	return scale;
}

int Health::getMaxHealth()
{
	return( this->max_health );
}

int Health::getCurrentHealth()
{
	return( this->current_health );
}

sf::RectangleShape* Health::getBar()
{
	return( this->bar );
}