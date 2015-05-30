#include <SFML/Graphics.hpp>
#include <Box2D\Box2D.h>
#include <iostream>
#include <vector>
#include <string>
#include "Timer.h"
#include "Object.h"
using namespace std;

#define DEGTORAD 0.0174532925199432957f
#define RADTODEG 57.295779513082320876f

const float PIXELS_TO_METERS = 0.03333; //number of meters in one pixel
const float METERS_TO_PIXELS = 30.0; //number of pixels in one meter

enum GAME_STATE {LIVE, EDITOR};
enum {DYNAMIC_BODY, STATIC_BODY, KINEMATIC_BODY}; //determines what type of box2d body to create
enum {POLY_SHAPE, CIRCLE_SHAPE}; //determines what type of box2d shape to create

//updates the mouse position
void updateMousePos(sf::Vector2f &pos, sf::RenderWindow &window);

int main()
{
    sf::RenderWindow window(sf::VideoMode(1600, 900), "Roboprototype");
	sf::Event e;
	window.setFramerateLimit(60);
	bool in_focus = true; //if false, the window won't update

	//world setup
	b2Vec2 gravity(0.0, -9.8f); //current gravity for the world
	b2World *world = new b2World(gravity);
	world->SetAllowSleeping(true); //allows particles to come to rest
	float32 time_step = 1.0 / 60.0; //the length of time passed to simulate (seconds)
	int32 velocity_iterations = 7; //how strongly to correct the velocity
	int32 position_iterations = 3; //how strongly to correct the position

	//mouse setup
	Timer mouse_clock;
	sf::Vector2f mouse_pos;

	//particles
	sf::CircleShape particle_shape;
	particle_shape.setFillColor( sf::Color(121, 224, 242) );
	particle_shape.setOutlineThickness(2);
	particle_shape.setOutlineColor( sf::Color(164, 234, 247) );
	particle_shape.setRadius(7);
	particle_shape.setOrigin( particle_shape.getRadius(), particle_shape.getRadius() );

	b2ParticleSystem *particle_system; //the world the particles inhabit
	b2ParticleSystemDef particle_system_def;
	b2ParticleDef *particle_def = new b2ParticleDef; //definition of an individual particle

	particle_system_def.density = 1;
	particle_system_def.radius = 10 * PIXELS_TO_METERS;
	particle_system_def.maxCount = 500; //maximum number of particles on the screen

	particle_system = world->CreateParticleSystem(&particle_system_def); //creates the particle system to hold all the particles
	particle_system->SetRadius( particle_shape.getRadius() );
	particle_system->SetDestructionByAge(true); //particles are automatically destroyed based on their age
	particle_system->SetGravityScale(-50.0);
	
	
	particle_def->lifetime = 10.0; //number of seconds particle will stay alive
	particle_def->color.Set(0, 255, 255, 255);
	particle_def->flags = b2_elasticParticle;
	particle_def->position.Set( window.getSize().x / 2.0, window.getSize().y / 2.0 );
	
	
	

	//base platform
	Object *temp_object = NULL;
	b2FixtureDef fixture;
	sf::Texture platform_texture;

	if(!platform_texture.loadFromFile("images//platform.png"))
	{
		printf("Failed to load platform texture on line %d \n", __LINE__);
		window.close();
	}

	fixture.density = 1;
	fixture.restitution = 0.1;
	fixture.friction = 0.5;

	temp_object = new Object(window, world, fixture, platform_texture, STATIC_BODY, POLY_SHAPE);
	temp_object->getSprite()->setColor( sf::Color(0, 0, 0, 200) );
	temp_object->getBody()->SetTransform( b2Vec2( (window.getSize().x / 2.0) * PIXELS_TO_METERS, -(window.getSize().y / 1.1) * PIXELS_TO_METERS ), 0.0 );
	temp_object->updateSpritePos();




    while (window.isOpen())
    {
        while (window.pollEvent(e))
        {
            if (e.type == sf::Event::Closed)
                window.close();

			else if(e.type == sf::Event::KeyPressed && e.key.code == sf::Keyboard::Escape)
				window.close();

			else if(e.type == sf::Event::LostFocus)
				in_focus = false;

			else if(e.type == sf::Event::GainedFocus)
				in_focus = true;			
        }

		if(in_focus == true)
		{
			//update clocks
			mouse_clock.update();
			
			//update mouse position
			updateMousePos(mouse_pos, window);

			//update world
			world->Step(time_step, velocity_iterations, position_iterations);

			cout << "Particle count: " << particle_system->GetParticleCount() << endl;

			if(mouse_clock.getElapsedTime() > 0.001)
			{
				
				if(sf::Mouse::isButtonPressed( sf::Mouse::Left ) ) //create particles
				{
					particle_def->position.Set( mouse_pos.x, mouse_pos.y );
					particle_system->CreateParticle(*particle_def);
					particle_shape.setPosition( particle_def->position.x, particle_def->position.y );
					mouse_clock.restart();
				}
				
			}

			window.clear(sf::Color(200, 200, 200) ); 

			
			for(int i = 0; i < particle_system->GetParticleCount(); i++) //loops through all the particles
			{
				b2Vec2 pos = particle_system->GetPositionBuffer()[i]; //gets the position of the current particle

				if( &pos != NULL ) //if the particle exists
				{
					particle_shape.setPosition( pos.x, pos.y );
					window.draw( particle_shape );
				}
			}
			
			window.draw( *temp_object->getSprite() );
			window.display();
		}        
    }

    return 0;
}

void updateMousePos(sf::Vector2f &pos, sf::RenderWindow &window)
{
	pos.x = sf::Mouse::getPosition(window).x;
	pos.y = sf::Mouse::getPosition(window).y;
}