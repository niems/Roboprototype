#ifndef PHYSICS_H
#define PHYSICS_H

#include <SFML\Graphics.hpp>
#include "Editor.h"
#include "Object.h"
#include "Camera.h"

class Physics
{
public:
	//keeps the kinematic body between the left and right boundary
	static void kinematicBoundaries(sf::Vector2f &left_boundary, sf::Vector2f &right_boundary, vector<Object*> &body);
	
	static bool boundsContainsPoint(sf::FloatRect &bounds, sf::Vector2f &point); //if the bounds of the object contains the point, returns true
	
	static bool boundsIntersectsBounds(sf::FloatRect &bounds1, sf::FloatRect &bounds2); //if the bounds of the objects contains the other bounds, returns true

	//static void levelBoundaries(Editor &editor, Camera &view, Object &player); //keeps the player within the level
};

#endif