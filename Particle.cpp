#include "Particle.h"

const float PIXELS_TO_METERS = 0.03333; //number of meters in one pixel
const float METERS_TO_PIXELS = 30.0; //number of pixels in one meter

//create particle systems in constructor

Particle::Particle(b2World *world, sf::RenderWindow &window)
{
	sf::Color fill_color = sf::Color(0, 255, 255, 180);
	sf::Color outline_color = sf::Color(128, 255, 255, 150);
	float gravity_scale = 50.0;
	int radius = 3;
	int outline_thickness = 3;
	int max_particles = 500;
	int lifetime = 0.25;

	//create player hair particle system
	sf::CircleShape *shape = new sf::CircleShape();
	shape->setFillColor( fill_color );
	shape->setOutlineThickness( outline_thickness );
	shape->setOutlineColor( outline_color );
	shape->setRadius( radius );
	shape->setOrigin( shape->getRadius(), shape->getRadius() );

	b2ParticleSystem *particle_system;
	b2ParticleSystemDef particle_system_def;
	
	particle_system_def.density = 1;
	particle_system_def.radius = (shape->getRadius() + (shape->getOutlineThickness() / 2.0) ) * PIXELS_TO_METERS;
	particle_system_def.maxCount = max_particles; //maximum number of particles on the screen

	particle_system = world->CreateParticleSystem(&particle_system_def); //creates the particle system to hold all the particles
	particle_system->SetRadius( shape->getRadius() );
	particle_system->SetDestructionByAge(true); //particles are automatically destroyed based on their age
	particle_system->SetGravityScale(gravity_scale);

	cout << "particle systems: " << world->GetParticleSystemList() << endl;

}

void Particle::playerHair(b2World *world, Object &player)
{
	if(player.getBody()->GetLinearVelocity().x == 0 && player.getBody()->GetLinearVelocity().y == 0)
	{
		sf::Vector2f pos;
		b2ParticleDef p_def;

		pos.x = rand() % 5;
		pos.x = (rand() % 2 == 0) ? (pos.x * -1) : pos.x; 
		pos.x += player.getSprite()->getPosition().x;

		pos.y = player.getSprite()->getPosition().y;
		pos.y -= 15.0;

		p_def.position.Set( pos.x, pos.y );
		p_def.lifetime = 0.25;
		p_def.flags = b2_elasticParticle;

		world->GetParticleSystemList()[TYPE::HAIR].CreateParticle(p_def);
		//p_def->position.Set( pos.x, pos.y );
		//p_system->CreateParticle(*p_def);
	}			
}