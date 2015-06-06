#include "Health.h"

Health::Health(Actor *entity, int current_hp, int max_hp)
{
	sf::Vector2f pos; //position of the bar
	sf::Vector2f size; //size of the bar
	size.x = entity->getTexture()->getSize().x;
	size.y = entity->getTexture()->getSize().y / 10.0;

	this->offset.x = size.x / 2.0;
	this->offset.y = (entity->getTexture()->getSize().y / 2.0) - (size.y / 2.0);
	pos.x = entity->getEntity()->getSprite()->getPosition().x;
	pos.y = entity->getEntity()->getSprite()->getPosition().y - this->offset.y;

	this->bar = new sf::RectangleShape();
	this->bar->setSize(size);
	this->bar->setOrigin( 0.0, size.y / 2.0 );
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
	this->current_health -= damage_amount;

	if( this->current_health < 0 )
	{
		this->current_health = 0;
	}

	this->bar->setScale(this->current_health / static_cast<float>(this->max_health), 1.0);
}

sf::RectangleShape* Health::getBar()
{
	return( this->bar );
}