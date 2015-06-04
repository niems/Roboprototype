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

	/*
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

	this->shapes.push_back( *shape ); //player hair shape
	*/


	//create blood splatter particle system
	sf::CircleShape *blood_shape = new sf::CircleShape();
	b2ParticleSystem *blood_splatter_system;
	b2ParticleSystemDef blood_splatter_def;

	fill_color = sf::Color(230, 0, 0, 150);
	outline_color = sf::Color(255, 0, 0, 150);
	gravity_scale = 50.0;
	radius = 3;
	outline_thickness = 3;
	max_particles = 500;
	lifetime = 1.5;

	blood_shape->setFillColor( fill_color );
	blood_shape->setOutlineThickness( outline_thickness );
	blood_shape->setOutlineColor( outline_color );
	blood_shape->setRadius( radius );
	blood_shape->setOrigin( blood_shape->getRadius(), blood_shape->getRadius() );

	blood_splatter_def.density = 1;
	blood_splatter_def.radius = (blood_shape->getRadius() + (blood_shape->getOutlineThickness() / 2.0) ) * PIXELS_TO_METERS;
	blood_splatter_def.maxCount = max_particles; //maximum number of particles on the screen

	blood_splatter_system = world->CreateParticleSystem(&blood_splatter_def); //creates the particle system to hold all the particles
	blood_splatter_system->SetRadius( blood_shape->getRadius() );
	blood_splatter_system->SetDestructionByAge(true); //particles are automatically destroyed based on their age
	blood_splatter_system->SetGravityScale(gravity_scale);

	this->shapes.push_back( *blood_shape ); //blood spatter shape
	this->particle_systems.push_back( blood_splatter_system );


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

		//world->GetParticleSystemList()[TYPE::HAIR].CreateParticle(p_def);
	}			
}

void Particle::bloodSplatter(b2World *world, const sf::Vector2f &pos) //creates a blood splatter at the position given
{
	//create 50 particles
	sf::Vector2f offset;
	b2ParticleDef p_def;

	p_def.lifetime = 1.5;
	p_def.flags = b2_elasticParticle;

	world->GetParticleSystemList()[TYPE::BLOOD_SPLATTER].SetGravityScale(-50.0);

	for(int i = 0; i < 50; i++)
	{
		offset.x = rand() % 30;
		offset.x = (rand() % 2 == 0) ? (offset.x * -1) : offset.x;
		offset.x += pos.x;

		offset.y = rand() % 10;
		offset.y = (rand() % 2 == 0) ? (offset.y * -1) : offset.y;
		offset.y += pos.y;

		p_def.position.Set( offset.x, offset.y );
		world->GetParticleSystemList()[TYPE::BLOOD_SPLATTER].CreateParticle(p_def);
		//this->particle_systems[TYPE::BLOOD_SPLATTER]->CreateParticle(p_def);
	}
		
}

sf::CircleShape& Particle::getShape(int type)
{
	return( this->shapes[type] );
}

vector<b2ParticleSystem *>& Particle::getParticleSystems()
{
	return( this->particle_systems );
}