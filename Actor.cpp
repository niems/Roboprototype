#include "Actor.h"

enum {POLY_SHAPE, CIRCLE_SHAPE}; //determines what type of box2d shape to create

#define getMax(a, b) ((a) > (b) ? a : b); //returns the maximum
#define getMin(a, b) ((a) > (b) ? b : a); //returns the minimum

#define DEGTORAD 0.0174532925199432957f
#define RADTODEG 57.295779513082320876f

const float PIXELS_TO_METERS = 0.03333; //number of meters in one pixel
const float METERS_TO_PIXELS = 30.0; //number of pixels in one meter

Actor::Actor(sf::RenderWindow &window, b2World *world, int current_index, int body_type, int shape_type)
{
	b2FixtureDef fixture;
	this->texture = new sf::Texture();
	this->velocity = sf::Vector2f(500.0, 600.0);
	this->alive = true; //actor always starts alive

	fixture.restitution = 0.1;
	fixture.friction = 1.0;
	fixture.density = 2.0;
	fixture.filter.categoryBits = Editor::ENTITY_CATEGORY::FRIENDLY;
	fixture.filter.maskBits = Editor::ENTITY_CATEGORY::BOUNCE | Editor::ENTITY_CATEGORY::BOUNDARY | Editor::ENTITY_CATEGORY::DYNAMIC_OBJECT | Editor::ENTITY_CATEGORY::PLATFORM | Editor::ENTITY_CATEGORY::WEAPON;

	if(!this->texture->loadFromFile("images//player.png") )
	{
		printf("Error %d : Failed to load player texture. \n", __LINE__);
	}

	this->entity = new Object(window, world, fixture, *(this->texture), -1, Editor::BODY_TYPE::DYNAMIC, POLY_SHAPE);
	this->entity->getBody()->SetFixedRotation(true);
}

sf::Texture* Actor::getTexture()
{
	return( this->texture );
}

Object* Actor::getEntity()
{
	return( this->entity );
}

bool Actor::isAlive()
{
	return( this->alive );
}

void Actor::updateClocks()
{
	this->clock.update();
	this->jump_clock.update();
}

void Actor::commandUpdate(sf::Vector2f &mouse_pos)
{
	if( this->clock.getElapsedTime() >= 0.05 )
		{
			if( sf::Keyboard::isKeyPressed( sf::Keyboard::D ) ) //moving right
			{
				b2Vec2 vel = this->entity->getBody()->GetLinearVelocity();
				float desired_vel = getMin( vel.x + (this->velocity.x / 50.0), this->velocity.x * PIXELS_TO_METERS ); 
				float vel_change = desired_vel - vel.x; //max velocity - current velocity
				float impulse = this->entity->getBody()->GetMass() * vel_change;
				this->entity->getBody()->ApplyLinearImpulse( b2Vec2(impulse, 0), this->entity->getBody()->GetWorldCenter(), true );
				this->clock.restart();
			}

			else if( sf::Keyboard::isKeyPressed( sf::Keyboard::A ) ) //moving left
			{
				b2Vec2 vel = this->entity->getBody()->GetLinearVelocity();
				float desired_vel = getMax( vel.x - (this->velocity.x / 50.0), -this->velocity.x * PIXELS_TO_METERS );
				float vel_change = desired_vel - vel.x; //max velocity - current velocity
				float impulse = this->entity->getBody()->GetMass() * vel_change;
				this->entity->getBody()->ApplyLinearImpulse( b2Vec2(impulse, 0), this->entity->getBody()->GetWorldCenter(), true );
				this->clock.restart();
			}

			else if( sf::Keyboard::isKeyPressed( sf::Keyboard::S ) ) //stop moving on the x axis
			{
				b2Vec2 vel;
				vel.x = 0.0;
				vel.y = this->entity->getBody()->GetLinearVelocity().y * 2.0;
				vel.y = (vel.y > 0) ? (vel.y * -1) : vel.y;

				this->entity->getBody()->SetLinearVelocity(vel);
				this->clock.restart();
			}
		}

		if( this->jump_clock.getElapsedTime() >= 1.0 )
		{
			if( sf::Keyboard::isKeyPressed( sf::Keyboard::Space ) )
			{
				this->entity->getBody()->SetLinearVelocity(b2Vec2(this->entity->getBody()->GetLinearVelocity().x, 0.0) ); //cancels out the velocity on the x axis before the impulse
				b2Vec2 impulse(0.0, this->entity->getBody()->GetMass() * 13);
				this->entity->getBody()->ApplyLinearImpulse(impulse, this->entity->getBody()->GetWorldCenter(), true);
				
				this->jump_clock.restart();
			}

			//resets the player to the mouse cursor position
			else if( sf::Keyboard::isKeyPressed( sf::Keyboard::LShift ) && sf::Keyboard::isKeyPressed( sf::Keyboard::R ) )
			{
				this->entity->getBody()->SetTransform( b2Vec2(mouse_pos.x * PIXELS_TO_METERS, -mouse_pos.y * PIXELS_TO_METERS), this->entity->getBody()->GetAngle() );
				
				this->jump_clock.restart();
			}
		}
}

void Actor::contactUpdate(b2World *world, Particle &particles)
{
	for(b2ContactEdge *edge = this->entity->getBody()->GetContactList(); edge; edge = edge->next)
	{
		if(edge->contact->GetFixtureA()->GetFilterData().categoryBits == Editor::ENTITY_CATEGORY::BOUNDARY ||
		   edge->contact->GetFixtureB()->GetFilterData().categoryBits == Editor::ENTITY_CATEGORY::BOUNDARY )
		{
			if(particles.getSystemClocks()[Particle::TYPE::EXPLOSION].getElapsedTime() >= 0.5)
			{
				particles.explosion(world, this->entity->getSprite()->getPosition() );
				particles.getSystemClocks()[Particle::TYPE::EXPLOSION].restart();
			}
		}

		else if(edge->contact->GetFixtureA()->GetFilterData().categoryBits == Editor::ENTITY_CATEGORY::WEAPON ||
			    edge->contact->GetFixtureB()->GetFilterData().categoryBits == Editor::ENTITY_CATEGORY::WEAPON )
			{
				if(particles.getSystemClocks()[Particle::TYPE::BLOOD_SPLATTER].getElapsedTime() >= 0.5)
				{
					particles.bloodSplatter(world, this->entity->getSprite()->getPosition() ); 
					particles.getSystemClocks()[Particle::TYPE::BLOOD_SPLATTER].restart();
				}
			}

		else if(edge->contact->GetFixtureA()->GetFilterData().categoryBits == Editor::ENTITY_CATEGORY::BOUNCE ||
			    edge->contact->GetFixtureB()->GetFilterData().categoryBits == Editor::ENTITY_CATEGORY::BOUNCE )
		{
			/*
			//edge->contact->GetFixtureA()->GetBody()->GetAngle(
			float impulse = player.getBody()->GetMass() * 10;
			b2Vec2 pos = b2Vec2(edge->contact->GetFixtureB()->GetBody()->GetPosition().x, edge->contact->GetFixtureB()->GetBody()->GetPosition().y + (50 * PIXELS_TO_METERS) );

			player.getBody()->SetTransform( pos, 0.0);
			player.getBody()->ApplyLinearImpulse( b2Vec2(0.0, impulse), player.getBody()->GetWorldCenter(), true); 
			clock.restart();
			*/
			
		}

		
	}
}