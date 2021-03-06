#include "Particle.h"

const float PIXELS_TO_METERS = 0.03333; //number of meters in one pixel
const float METERS_TO_PIXELS = 30.0; //number of pixels in one meter

//create particle systems in constructor

Particle::Particle(b2World *world, sf::RenderWindow &window)
{
	//create blood splatter particle system
	Timer blood_splatter_clock;
	sf::CircleShape *blood_shape = new sf::CircleShape();
	b2ParticleSystem *blood_splatter_system;
	b2ParticleSystemDef blood_splatter_def;

	this->shapes.push_back( *blood_shape ); //blood spatter shape
	this->particle_systems.push_back( blood_splatter_system );
	this->system_clocks.push_back(blood_splatter_clock);

	sf::Color fill_color = sf::Color(147, 22, 33, 255);
	sf::Color outline_color = sf::Color(224, 48, 66, 255);
	float gravity_scale = -35.0;
	int radius = 2;
	int outline_thickness = 2;
	int max_particles = 500;
	float lifetime = 2.0;

	this->shapes.back().setFillColor( fill_color );
	this->shapes.back().setOutlineThickness( outline_thickness );
	this->shapes.back().setOutlineColor( outline_color );
	this->shapes.back().setRadius( radius );
	this->shapes.back().setOrigin( blood_shape->getRadius(), blood_shape->getRadius() );

	blood_splatter_def.density = 1;
	blood_splatter_def.radius = (blood_shape->getRadius() + (blood_shape->getOutlineThickness() / 2.0) ) * PIXELS_TO_METERS;
	blood_splatter_def.maxCount = max_particles; //maximum number of particles on the screen

	this->particle_systems.back() = world->CreateParticleSystem(&blood_splatter_def); //creates the particle system to hold all the particles
	this->particle_systems.back()->SetRadius( this->shapes.back().getRadius() );
	this->particle_systems.back()->SetDestructionByAge(true); //particles are automatically destroyed based on their age
	this->particle_systems.back()->SetGravityScale(gravity_scale);

	
	

	//create player hair particle system
	Timer player_hair_clock;
	fill_color = sf::Color(0, 255, 255, 180);
	outline_color = sf::Color(128, 255, 255, 150);
	gravity_scale = 50.0;
	radius = 3;
	outline_thickness = 3;
	max_particles = 500;
	lifetime = 0.25;	
	
	this->system_clocks.push_back( player_hair_clock );

	sf::CircleShape *shape = new sf::CircleShape();
	this->shapes.push_back( *shape ); //player hair shape
	this->shapes.back().setFillColor( fill_color );
	this->shapes.back().setOutlineThickness( outline_thickness );
	this->shapes.back().setOutlineColor( outline_color );
	this->shapes.back().setRadius( radius );
	this->shapes.back().setOrigin( shape->getRadius(), shape->getRadius() );

	b2ParticleSystem *particle_system;
	b2ParticleSystemDef particle_system_def;
	this->particle_systems.push_back( particle_system );
	
	particle_system_def.density = 1;
	particle_system_def.radius = (shape->getRadius() + (shape->getOutlineThickness() / 2.0) ) * PIXELS_TO_METERS;
	particle_system_def.maxCount = max_particles; //maximum number of particles on the screen

	this->particle_systems.back() = world->CreateParticleSystem(&particle_system_def); //creates the particle system to hold all the particles
	this->particle_systems.back()->SetRadius( this->shapes.back().getRadius() );
	this->particle_systems.back()->SetDestructionByAge(true); //particles are automatically destroyed based on their age
	this->particle_systems.back()->SetGravityScale(gravity_scale);

	//create explosion particle system
	Timer explosion_clock;
	b2ParticleSystem *explosion_system;
	b2ParticleSystemDef explosion_system_def;
	sf::CircleShape *explosion_shape = new sf::CircleShape();
	fill_color = sf::Color(0, 255, 255, 180);
	outline_color = sf::Color(24, 242, 178, 180);
	gravity_scale = -40.0;
	radius = 3;
	outline_thickness = 2;
	max_particles = 500;
	lifetime = 2.0;	

	this->particle_systems.push_back( explosion_system );
	this->shapes.push_back( *explosion_shape ); //player hair shape
	this->system_clocks.push_back( explosion_clock );

	this->shapes.back().setFillColor( fill_color );
	this->shapes.back().setOutlineThickness( outline_thickness );
	this->shapes.back().setOutlineColor( outline_color );
	this->shapes.back().setRadius( radius );
	this->shapes.back().setOrigin( explosion_shape->getRadius(), explosion_shape->getRadius() );

	explosion_system_def.density = 1;
	explosion_system_def.radius = (shape->getRadius() + (shape->getOutlineThickness() / 2.0) ) * PIXELS_TO_METERS;
	explosion_system_def.maxCount = max_particles; //maximum number of particles on the screen

	this->particle_systems.back() = world->CreateParticleSystem(&explosion_system_def); //creates the particle system to hold all the particles
	this->particle_systems.back()->SetRadius( this->shapes.back().getRadius() );
	this->particle_systems.back()->SetDestructionByAge(true); //particles are automatically destroyed based on their age
	this->particle_systems.back()->SetGravityScale(gravity_scale);

	//creates explosion when the player spawns in a new level
	Timer spawn_clock;
	b2ParticleSystem *spawn_system;
	b2ParticleSystemDef spawn_system_def;
	sf::CircleShape *spawn_shape = new sf::CircleShape();
	fill_color = sf::Color(255, 255, 255, 255);
	outline_color = sf::Color(230, 230, 230, 180);
	gravity_scale = -40.0;
	radius = 3;
	outline_thickness = 2;
	max_particles = 500;
	lifetime = 2.0;	

	this->particle_systems.push_back( spawn_system );
	this->shapes.push_back( *spawn_shape ); //player hair shape
	this->system_clocks.push_back( spawn_clock );

	this->shapes.back().setFillColor( fill_color );
	this->shapes.back().setOutlineThickness( outline_thickness );
	this->shapes.back().setOutlineColor( outline_color );
	this->shapes.back().setRadius( radius );
	this->shapes.back().setOrigin( spawn_shape->getRadius(), spawn_shape->getRadius() );

	spawn_system_def.density = 1;
	spawn_system_def.radius = (shape->getRadius() + (shape->getOutlineThickness() / 2.0) ) * PIXELS_TO_METERS;
	spawn_system_def.maxCount = max_particles; //maximum number of particles on the screen

	this->particle_systems.back() = world->CreateParticleSystem(&spawn_system_def); //creates the particle system to hold all the particles
	this->particle_systems.back()->SetRadius( this->shapes.back().getRadius() );
	this->particle_systems.back()->SetDestructionByAge(true); //particles are automatically destroyed based on their age
	this->particle_systems.back()->SetGravityScale(gravity_scale);

	//creates explosion when the player bounces on a platform
	Timer bounce_clock;
	b2ParticleSystem *bounce_system;
	b2ParticleSystemDef bounce_system_def;
	sf::CircleShape *bounce_shape = new sf::CircleShape();

	fill_color = sf::Color(66, 217, 200, 255);
	outline_color = sf::Color(128, 230, 217, 255);
	gravity_scale = -50.0;
	radius = 3;
	outline_thickness = 2;
	max_particles = 500;
	lifetime = 0.5;	

	this->particle_systems.push_back( bounce_system );
	this->shapes.push_back( *bounce_shape ); //player hair shape
	this->system_clocks.push_back( bounce_clock );

	this->shapes.back().setFillColor( fill_color );
	this->shapes.back().setOutlineThickness( outline_thickness );
	this->shapes.back().setOutlineColor( outline_color );
	this->shapes.back().setRadius( radius );
	this->shapes.back().setOrigin( bounce_shape->getRadius(), bounce_shape->getRadius() );

	bounce_system_def.density = 1;
	bounce_system_def.radius = (shape->getRadius() + (shape->getOutlineThickness() / 2.0) ) * PIXELS_TO_METERS;
	bounce_system_def.maxCount = max_particles; //maximum number of particles on the screen

	this->particle_systems.back() = world->CreateParticleSystem(&bounce_system_def); //creates the particle system to hold all the particles
	this->particle_systems.back()->SetRadius( this->shapes.back().getRadius() );
	this->particle_systems.back()->SetDestructionByAge(true); //particles are automatically destroyed based on their age
	this->particle_systems.back()->SetGravityScale(gravity_scale);

	//teleport particle animation
	Timer teleport_clock;
	b2ParticleSystem *teleport_system;
	b2ParticleSystemDef teleport_system_def;
	sf::CircleShape *teleport_shape = new sf::CircleShape();

	fill_color = sf::Color(255, 255, 255, 255);
	outline_color = sf::Color(128, 230, 217, 255);
	gravity_scale = -50.0;
	radius = 4;
	outline_thickness = 3;
	max_particles = 500;
	lifetime = 0.5;	

	this->particle_systems.push_back( teleport_system );
	this->shapes.push_back( *teleport_shape ); //player hair shape
	this->system_clocks.push_back( teleport_clock );

	this->shapes.back().setFillColor( fill_color );
	this->shapes.back().setOutlineThickness( outline_thickness );
	this->shapes.back().setOutlineColor( outline_color );
	this->shapes.back().setRadius( radius );
	this->shapes.back().setOrigin( teleport_shape->getRadius(), teleport_shape->getRadius() );

	teleport_system_def.density = 1;
	teleport_system_def.radius = (shape->getRadius() + (shape->getOutlineThickness() / 2.0) ) * PIXELS_TO_METERS;
	teleport_system_def.maxCount = max_particles; //maximum number of particles on the screen

	this->particle_systems.back() = world->CreateParticleSystem(&teleport_system_def); //creates the particle system to hold all the particles
	this->particle_systems.back()->SetRadius( this->shapes.back().getRadius() );
	this->particle_systems.back()->SetDestructionByAge(true); //particles are automatically destroyed based on their age
	this->particle_systems.back()->SetGravityScale(gravity_scale);


}

void Particle::updateClocks()
{
	for(int i = 0; i < this->system_clocks.size(); i++)
	{
		this->system_clocks[i].update();
	}
}

void Particle::playerHair(b2World *world, Object &player)
{
	if(player.getBody()->GetLinearVelocity().x == 0 && player.getBody()->GetLinearVelocity().y == 0)
	{
		this->particle_systems[TYPE::HAIR]->SetGravityScale(50.0);
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

		this->particle_systems[TYPE::HAIR]->CreateParticle(p_def);
	}			
}

void Particle::bloodSplatter(b2World *world, const sf::Vector2f &pos) //creates a blood splatter at the position given
{
	//create 50 particles
	sf::Vector2f offset;
	b2ParticleDef p_def;

	p_def.lifetime = 1.5;
	p_def.flags = b2_elasticParticle;

	for(int i = 0; i < 40; i++)
	{
		offset.x = rand() % 30;
		offset.x = (rand() % 2 == 0) ? (offset.x * -1) : offset.x;
		offset.x += pos.x;

		offset.y = rand() % 10;
		offset.y = (rand() % 2 == 0) ? (offset.y * -1) : offset.y;
		offset.y += pos.y;

		p_def.position.Set( offset.x, offset.y );
		p_def.velocity.x = rand() % 450;
		p_def.velocity.x = (rand() % 2 == 0) ? p_def.velocity.x : -p_def.velocity.x;
		p_def.velocity.y = -150.0;
		

		this->particle_systems[TYPE::BLOOD_SPLATTER]->CreateParticle(p_def);
	}
		
}

void Particle::explosion(b2World *world, const sf::Vector2f &pos)
{
	sf::Vector2f offset;
	b2ParticleDef p_def;

	p_def.lifetime = 2.0;
	p_def.flags = b2_elasticParticle;

	for(int i = 0; i < 150; i++)
	{
		offset.x = rand() % 30;
		offset.x = (rand() % 2 == 0) ? (offset.x * -1) : offset.x;
		offset.x += pos.x;

		offset.y = rand() % 10;
		offset.y = (rand() % 2 == 0) ? (offset.y * -1) : offset.y;
		offset.y += pos.y;

		p_def.position.Set( offset.x, offset.y );
		p_def.velocity.x = (rand() % 450) + 100;
		p_def.velocity.x = (rand() % 2 == 0) ? p_def.velocity.x : -p_def.velocity.x;

		p_def.velocity.y = (rand() % 500) + 100;
		p_def.velocity.y = (rand() % 2 == 0) ? p_def.velocity.y : -p_def.velocity.y;
		

		this->particle_systems[TYPE::EXPLOSION]->CreateParticle(p_def);
	}
}

void Particle::spawn(b2World *world, const sf::Vector2f &pos)
{
	sf::Vector2f offset;
	b2ParticleDef p_def;

	p_def.lifetime = 0.3;
	p_def.flags = b2_elasticParticle;

	for(int i = 0; i < 100; i++)
	{
		offset.x = rand() % 30;
		offset.x = (rand() % 2 == 0) ? (offset.x * -1) : offset.x;
		offset.x += pos.x;

		offset.y = rand() % 10;
		offset.y = (rand() % 2 == 0) ? (offset.y * -1) : offset.y;
		offset.y += pos.y;

		p_def.position.Set( offset.x, offset.y );
		p_def.velocity.x = (rand() % 200) + 100;
		p_def.velocity.x = (rand() % 2 == 0) ? p_def.velocity.x : -p_def.velocity.x;

		p_def.velocity.y = (rand() % 200) + 100;
		p_def.velocity.y = (rand() % 2 == 0) ? p_def.velocity.y : -p_def.velocity.y;
		

		this->particle_systems[TYPE::SPAWN]->CreateParticle(p_def);
	}
}

void Particle::teleport(b2World *world, const sf::Vector2f &pos)
{
	sf::Vector2f offset;
	b2ParticleDef p_def;

	p_def.lifetime = 0.5;
	p_def.flags = b2_elasticParticle;

	for(int i = 0; i < 50; i++)
	{
		offset.x = rand() % 15;
		offset.x = (rand() % 2 == 0) ? (offset.x * -1) : offset.x;
		offset.x += pos.x;

		offset.y = rand() % 5;
		offset.y = (rand() % 2 == 0) ? (offset.y * -1) : offset.y;
		offset.y += pos.y;

		p_def.position.Set( offset.x, offset.y );
		p_def.velocity.x = (rand() % 100) + 50;
		p_def.velocity.x = (rand() % 2 == 0) ? p_def.velocity.x : -p_def.velocity.x;

		p_def.velocity.y = (rand() % 100) + 50;
		p_def.velocity.y = (rand() % 2 == 0) ? p_def.velocity.y : -p_def.velocity.y;
		

		this->particle_systems[TYPE::TELEPORT]->CreateParticle(p_def);
	}
}

void Particle::bounce(b2World *world, const sf::Vector2f &pos)
{
	sf::Vector2f offset;
	b2ParticleDef p_def;

	p_def.lifetime = 1.0;
	p_def.flags = b2_elasticParticle;

	for(int i = 0; i < 100; i++)
	{
		offset.x = rand() % 30;
		offset.x = (rand() % 2 == 0) ? (offset.x * -1) : offset.x;
		offset.x += pos.x;

		offset.y = rand() % 10;
		offset.y = (rand() % 2 == 0) ? (offset.y * -1) : offset.y;
		offset.y += pos.y;

		p_def.position.Set( offset.x, offset.y );
		p_def.velocity.x = 60;
		p_def.velocity.x = (rand() % 2 == 0) ? p_def.velocity.x : -p_def.velocity.x;

		//p_def.velocity.y = (rand() % 200) + 200;
		//p_def.velocity.y *= -1;
		//p_def.velocity.y = (rand() % 2 == 0) ? p_def.velocity.y : -p_def.velocity.y;

		p_def.velocity.y = (rand() % 500) + 100;

		this->particle_systems[TYPE::BOUNCE]->CreateParticle(p_def);
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

vector<Timer>& Particle::getSystemClocks()
{
	return( this->system_clocks );
}