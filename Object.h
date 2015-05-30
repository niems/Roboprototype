#ifndef OBJECT_H
#define OBJECT_H

#include <Box2D\Box2D.h>
#include <SFML\Graphics.hpp>
#include "Object.h"
using namespace std;

//fit all the initialization code here, so it would just need to be passed the texture
//and the box2d body type, and it could do the rest.

class Object
{
private:
	b2Body *body;
	sf::Sprite sprite; 
	int index; //current index of the item created

public:
	Object();

	//creates the default setup
	Object(sf::RenderWindow &window, b2World *world, b2FixtureDef &fixture, sf::Texture &texture, int current_index, int body_type, int shape_type);

	void updateSpritePos(); //updates the sprite position to the box2d body position
	void setIndex(int &index); //sets the current index

	static void updatePosition(Object &object);//used to update a single object(like the player)
	static void updatePosition(vector<Object*> &object);//updates all the sprite positions

	int getCurrentIndex(); //returns the current index of the object6
	b2Body* getBody(); //returns the box2d body
	sf::Sprite* getSprite(); //returns the body sprite

};


#endif