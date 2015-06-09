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
	this->health_bar = new Health(sf::Vector2f(this->texture->getSize().x, this->texture->getSize().y), sf::Vector2f(this->entity->getSprite()->getPosition().x, this->entity->getSprite()->getPosition().y), 100, 100);

}

sf::Texture* Actor::getTexture()
{
	return( this->texture );
}

Object* Actor::getEntity()
{
	return( this->entity );
}

Health* Actor::getHealthBar()
{
	return( this->health_bar );
}

Timer* Actor::getDeathClock()
{
	return( &(this->death_clock) );
}

Object* Actor::getEntityContact()
{
	return( this->entity_contact );
}

b2Vec2* Actor::getEntityContactPos()
{
	return( this->entity_contact_pos );
}

bool Actor::isAlive()
{
	return( this->alive );
}

bool Actor::isLevelComplete()
{
	return( this->level_complete );
}

void Actor::updateClocks()
{
	this->clock.update();
	this->jump_clock.update();
	this->death_clock.update();
	this->contact_clock.update();
}

void Actor::commandUpdate(sf::Vector2f &mouse_pos)
{
	if(this->alive == true) //only updates if the player is alive
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
}

//(sf::RenderWindow &window, b2World *world, Editor &editor, Camera &view)
int Actor::contactUpdate(sf::RenderWindow &window, b2World *world, Editor &editor, Camera &view, Particle &particles)
{
	int contact = -1; //the player isn't in contact with anything
	this->entity_contact = NULL; //point to the object in contact with the player
	this->entity_contact_pos = new b2Vec2(-1.0, -1.0); 

	if(this->alive == true) //only updates if the player is alive
	{
		for(b2ContactEdge *edge = this->entity->getBody()->GetContactList(); edge; edge = edge->next)
		{
			
			if(edge->contact->GetFixtureA()->GetFilterData().categoryBits == Editor::ENTITY_CATEGORY::BOUNDARY ||
			   edge->contact->GetFixtureB()->GetFilterData().categoryBits == Editor::ENTITY_CATEGORY::BOUNDARY )
			{
				contact = Editor::ENTITY_CATEGORY::BOUNDARY;

				if(particles.getSystemClocks()[Particle::TYPE::EXPLOSION].getElapsedTime() >= 0.5)
				{
					particles.explosion(world, this->entity->getSprite()->getPosition() );
					particles.bloodSplatter(world, this->entity->getSprite()->getPosition() ); 
					this->health_bar->damage(this->health_bar->getMaxHealth()); //kills player
					
					this->death(); //sets everything up so that the player is dead

					particles.getSystemClocks()[Particle::TYPE::EXPLOSION].restart();
				}
			} 

			else if(edge->contact->GetFixtureA()->GetFilterData().categoryBits == Editor::ENTITY_CATEGORY::WEAPON ||
					edge->contact->GetFixtureB()->GetFilterData().categoryBits == Editor::ENTITY_CATEGORY::WEAPON )
				{
					contact = Editor::ENTITY_CATEGORY::WEAPON;

					if(particles.getSystemClocks()[Particle::TYPE::BLOOD_SPLATTER].getElapsedTime() >= 0.5)
					{
						particles.bloodSplatter(world, this->entity->getSprite()->getPosition() ); 
						this->health_bar->damage(20);

						if(this->health_bar->getCurrentHealth() == 0)
						{
							particles.explosion(world, this->entity->getSprite()->getPosition() );
							this->death(); //sets everything so the player is dead
						}
					
						particles.getSystemClocks()[Particle::TYPE::BLOOD_SPLATTER].restart();
					}
				}
			
			
			else if(edge->contact->GetFixtureA()->GetFilterData().categoryBits == Editor::ENTITY_CATEGORY::TELEPORT ||
			   edge->contact->GetFixtureB()->GetFilterData().categoryBits == Editor::ENTITY_CATEGORY::TELEPORT )
			{
				contact = Editor::ENTITY_CATEGORY::TELEPORT;
				this->level_complete = true; //the player has reached the portal
			} 

			
			if(this->contact_clock.getElapsedTime() >= 0.5)
			{
				if(edge->contact->GetFixtureA()->GetFilterData().categoryBits == Editor::ENTITY_CATEGORY::BOUNCE ||
					edge->contact->GetFixtureB()->GetFilterData().categoryBits == Editor::ENTITY_CATEGORY::BOUNCE )
				{
					this->contact_clock.restart();

					contact = Editor::ENTITY_CATEGORY::BOUNCE;
					//edge->contact->GetFixtureA()->GetBody()->GetPosition();

					if(edge->contact->GetFixtureA()->GetFilterData().categoryBits == Editor::ENTITY_CATEGORY::BOUNCE)
					{
						const b2Vec2 pos = edge->contact->GetFixtureA()->GetBody()->GetPosition();

						this->entity_contact_pos->x = pos.x;
						this->entity_contact_pos->y = pos.y;
					}

					else if(edge->contact->GetFixtureB()->GetFilterData().categoryBits == Editor::ENTITY_CATEGORY::BOUNCE)
					{
						const b2Vec2 pos = edge->contact->GetFixtureB()->GetBody()->GetPosition();

						this->entity_contact_pos->x = pos.x;
						this->entity_contact_pos->y = pos.y;
					}

					//particle bounce explosion
					/*
					if(edge->contact->GetFixtureA()->GetFilterData().categoryBits == Editor::ENTITY_CATEGORY::BOUNCE)
					{
						this->entity_contact = static_cast<Object *>( edge->contact->GetFixtureA()->GetUserData() );
						this->entity_contact_pos = &(this->entity_contact->getBody()->GetPosition());
					}

					else if(edge->contact->GetFixtureB()->GetFilterData().categoryBits == Editor::ENTITY_CATEGORY::BOUNCE)
					{
						this->entity_contact = static_cast<Object *>( edge->contact->GetFixtureB()->GetUserData() );
						this->entity_contact_pos = this->entity_contact->getBody()->GetPosition();
					}
					*/
				}	
			}
			
		}

		//if the player is intersecting the portal, load next level

	}

	return contact; //returns if the player had contact with any objects
	
}

void Actor::setAlive(bool status)
{
	this->alive = status;
}

void Actor::setLevelComplete(bool status)
{
	this->level_complete = status;
}

void Actor::death()
{
	this->alive = false;
	this->entity->getBody()->SetType(b2_staticBody); //sets to a static body so it won't move
	this->death_clock.restart();
}

void Actor::respawn(sf::RenderWindow &window, b2World *world, Editor &editor, Camera &view, Object &player, string &file_name)
{
	if(this->death_clock.getElapsedTime() >= 0.85)
	{
		editor.deleteAllObjects(world);
		editor.loadFile(window, world, view, player, file_name); //use editor.current_level for file name

		sf::Vector2f spawn;
		spawn.x = editor.getSpawnPoint().x;
		spawn.y = editor.getSpawnPoint().y;
		
		this->alive = true;
		this->entity->getBody()->SetType(b2_dynamicBody);
		this->getHealthBar()->heal(this->getHealthBar()->getMaxHealth());

		this->entity->getBody()->SetTransform( b2Vec2(spawn.x * PIXELS_TO_METERS, -spawn.y * PIXELS_TO_METERS), this->entity->getBody()->GetAngle() );
	}
	
}

void Actor::loadNextLevel(sf::RenderWindow &window, b2World *world, Editor &editor, Camera &view)
{
		this->level_complete = false; //reset. The new level isn't completed yet

		editor.current_level_index++; //goes to the index of the next level
		if(editor.current_level_index > editor.max_level) //checks if the level exists
			editor.current_level_index = Editor::FILE::LEVEL1; //the level system is circular

		editor.current_level = editor.levels[editor.current_level_index]; //sets the current level

		editor.deleteAllObjects(world);
		editor.loadFile(window, world, view, *(this->entity), editor.current_level); //loads next level

		sf::Vector2f spawn; //player's spawn point on the new level
		spawn.x = editor.getSpawnPoint().x;
		spawn.y = editor.getSpawnPoint().y;
		
		this->alive = true;
		this->entity->getBody()->SetType(b2_dynamicBody);
		this->getHealthBar()->heal(this->getHealthBar()->getMaxHealth()); //full health for new level

		this->entity->getBody()->SetTransform( b2Vec2(spawn.x * PIXELS_TO_METERS, -spawn.y * PIXELS_TO_METERS), this->entity->getBody()->GetAngle() );
		this->entity->getBody()->SetLinearVelocity( b2Vec2(0.0, 0.0) );
}