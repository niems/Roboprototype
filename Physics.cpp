#include "Physics.h"

const float PIXELS_TO_METERS = 0.03333; //number of meters in one pixel
const float METERS_TO_PIXELS = 30.0; //number of pixels in one meter

void Physics::kinematicBoundaries(sf::Vector2f &left_boundary, sf::Vector2f &right_boundary, vector<Object*> &body)
{
	sf::Vector2f pos;
	b2Vec2 vel;

	for(int i = 0; i < body.size(); i++) //goes through all kinematic bodies to check if they've reached a boundary
	{
		pos = sf::Vector2f(body[i]->getBody()->GetPosition().x * METERS_TO_PIXELS, body[i]->getBody()->GetPosition().y * METERS_TO_PIXELS);
		vel = body[i]->getBody()->GetLinearVelocity();

		vel.x = (vel.x > 0) ? vel.x : (vel.x * -1); //makes x velocity positive

		if(pos.x <= left_boundary.x) //if the platform is at the left boundary, reverse
		{
			body[i]->getBody()->SetLinearVelocity(vel);
		}

		else if(pos.x >= right_boundary.x) //if the platform is at the right boundary, reverse
		{
			vel.x *= -1;
			body[i]->getBody()->SetLinearVelocity(vel);
		}
	}
}

bool Physics::boundsContainsPoint(sf::FloatRect &bounds, sf::Vector2f &point)
{
	if(bounds.contains(point)) //the point is intersecting the bounds
		return true;

	return false; //the point is not intersecting the bounds
}

bool Physics::boundsIntersectsBounds(sf::FloatRect &bounds1, sf::FloatRect &bounds2)
{
	if(bounds1.intersects(bounds2))
		return true; //the bounds are intersecting each other

	return false; //the bounds are not intersecting each other
}