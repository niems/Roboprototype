#include "Particle.h"

Particle::Particle(b2World *world, sf::RenderWindow &window, sf::Color &fill_color, sf::Color &outline_color, float &gravity_scale, int &radius, int &outline_thickness, int &max_particles, int &lifetime)
{
	//bool particle_toggle = true; //if true, particles are constantly created
	sf::CircleShape particle_shape;
	particle_shape.setFillColor( fill_color );
	particle_shape.setOutlineThickness(outline_thickness);
	particle_shape.setOutlineColor( outline_color );
	particle_shape.setRadius(radius);
	particle_shape.setOrigin( particle_shape.getRadius(), particle_shape.getRadius() );

	b2ParticleSystem *particle_system; //the world the particles inhabit
	b2ParticleSystemDef particle_system_def;
	b2ParticleDef *particle_def = new b2ParticleDef; //definition of an individual particle

	particle_system_def.density = 1;
	particle_system_def.radius = (particle_shape.getRadius() + (particle_shape.getOutlineThickness() / 2.0) ) * PIXELS_TO_METERS;
	particle_system_def.maxCount = 500; //maximum number of particles on the screen

	particle_system = world->CreateParticleSystem(&particle_system_def); //creates the particle system to hold all the particles
	particle_system->SetRadius( particle_shape.getRadius() );
	particle_system->SetDestructionByAge(true); //particles are automatically destroyed based on their age
	particle_system->SetGravityScale(50.0);	
	
	particle_def->lifetime = 0.25; //number of seconds particle will stay alive
	particle_def->color.Set(0, 255, 255, 255);
	particle_def->flags = b2_elasticParticle;
	particle_def->position.Set( window.getSize().x / 2.0, window.getSize().y / 2.0 );

	this->p_system = particle_system;
	this->p_def = particle_def;
}

void Particle::playerHair(Object &player, Timer &clock, bool &toggle, b2ParticleDef *p_def, b2ParticleSystem *p_system)
{
	if(clock.getElapsedTime() >= 0.5)
	{
		if( sf::Keyboard::isKeyPressed( sf::Keyboard::P ) )
		{
			toggle = (toggle == false) ? true : false;
			clock.restart();
		}
	}

	if(toggle == true) 
	{
		sf::Vector2f pos;

		if(player.getBody()->GetLinearVelocity().x == 0 && player.getBody()->GetLinearVelocity().y == 0)
		{
			pos.x = rand() % 5;
			pos.x = (rand() % 2 == 0) ? (pos.x * -1) : pos.x; 
			pos.x += player.getSprite()->getPosition().x;

			pos.y = player.getSprite()->getPosition().y;
			pos.y -= 15.0;
			

			//p_def->lifetime = 0.25;
			p_def->position.Set( pos.x, pos.y );
			p_system->CreateParticle(*p_def);
		}		
	}
}