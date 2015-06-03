#include <SFML/Graphics.hpp>
#include <Box2D\Box2D.h>
#include <iostream>
#include <vector>
#include <string>
#include "Timer.h"
#include "Object.h"
#include "Editor.h"
#include "Draw.h"
#include "Camera.h"
#include "Physics.h"
using namespace std;

#define getMax(a, b) ((a) > (b) ? a : b); //returns the maximum
#define getMin(a, b) ((a) > (b) ? b : a); //returns the minimum

#define DEGTORAD 0.0174532925199432957f
#define RADTODEG 57.295779513082320876f

const float PIXELS_TO_METERS = 0.03333; //number of meters in one pixel
const float METERS_TO_PIXELS = 30.0; //number of pixels in one meter

enum {POLY_SHAPE, CIRCLE_SHAPE}; //determines what type of box2d shape to create

//updates the mouse position
void updateMousePos(sf::Vector2i &pos, sf::Vector2f &world_pos, sf::RenderWindow &window);

//checks for player commands
//put in physics class or player class
void playerCommandUpdate(Timer &movement_clock, Timer &jump_clock, sf::Vector2f &speed, Object &player, sf::Vector2f &mouse_pos);

//moves the character back to the spawn point if they go out of bounds
//put this in the physics class
void levelBoundaries(Editor &editor, Camera &view, Object &player);

//updates the particle toggle
//put in particle class
void particleToggle(Object &player, Timer &mouse_clock, bool &toggle, b2ParticleDef *p_def, b2ParticleSystem *p_system);


int main()
{
    sf::RenderWindow window(sf::VideoMode(1600, 900), "Roboprototype");
	sf::Event e;
	window.setFramerateLimit(60);
	bool in_focus = true; //if false, the window won't update

	//world setup
	Timer fps_clock; //used to determine the fps
	b2Vec2 gravity(0.0, -20.0f); //current gravity for the world
	b2World *world = new b2World(gravity);
	world->SetAllowSleeping(true); //allows particles to come to rest
	float32 time_step = 1.0 / 60.0; //the length of time passed to simulate (seconds)
	int32 velocity_iterations = 7; //how strongly to correct the velocity
	int32 position_iterations = 3; //how strongly to correct the position


	//game mode setup(live/editor)
	//PUT EVERYTHING EXCEPT THE EDITOR INSIDE THE EDITOR CLASS
	Editor editor; //used to store all objects and edit the level
	int game_state = Editor::GAME_STATE::LIVE; //current mode
	Timer editor_clock;
	Timer game_mode_clock;
	sf::Font font;
	sf::Text game_mode_text; //used to display the current game mode
	sf::Text editor_object_type; //used to display the current type of object in the editor mode
	string game_mode_live = "Mode: Live";
	string game_mode_editor = "Mode: Editor";
	string editor_type[3] = {"Object Type: Static", "Object Type: Dynamic", "Object Type: Kinematic"};
	sf::Vector2i game_mode_text_pos(10, 10); 
	

	if(!font.loadFromFile("fonts//amatic.ttf") )
	{
		printf("Failed to load font on line %d \n", __LINE__);
		window.close();
	}

	game_mode_text.setFont(font);
	game_mode_text.setString( (game_state == Editor::GAME_STATE::LIVE) ? game_mode_live : game_mode_editor );
	game_mode_text.setCharacterSize(50);
	game_mode_text.scale( window.getSize().x / 1600.0, window.getSize().y / 900); //scales based on the window size it was created on
	game_mode_text.setPosition(game_mode_text_pos.x, game_mode_text_pos.y);
	game_mode_text.setColor( sf::Color(255, 255, 255) );

	editor_object_type.setFont(font);
	editor_object_type.setString( editor_type[editor.getObjectType()] );
	editor_object_type.setCharacterSize(50);
	editor_object_type.scale(window.getSize().x / 1600.0, window.getSize().y / 900); //scales based on the window size it was created on
	editor_object_type.setColor( sf::Color(255, 255, 255) );
	sf::Vector2i editor_object_pos( game_mode_text_pos.x, game_mode_text_pos.y + editor_object_type.getGlobalBounds().height * 1.5 );
	editor_object_type.setPosition( window.mapPixelToCoords(editor_object_pos) );

	//mouse setup
	Timer mouse_clock;
	sf::Vector2i mouse_pos;
	sf::Vector2f mouse_pos_world; //world coordinates for the current mouse position

	//particles///////////
	//fill color *
	//radius *
	//outline thickness *
	//outline color *
	//max number of particles
	//lifetime
	//gravity scale *
	//window *
	//void Particle(sf::RenderWindow &window, sf::Color &fill_color, sf::Color &outline_color, float &gravity_scale, int &radius, int &outline_thickness, int &max_particles, int &lifetime);
	bool particle_toggle = true; //if true, particles are constantly created
	sf::CircleShape particle_shape;
	particle_shape.setFillColor( sf::Color(0, 255, 255, 180) );
	particle_shape.setOutlineThickness(3);
	particle_shape.setOutlineColor( sf::Color(128, 255, 255, 150) );
	particle_shape.setRadius(3);
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
	//particle_def->color.Set(0, 255, 255, 255);
	particle_def->flags = b2_elasticParticle;
	//particle_def->position.Set( window.getSize().x / 2.0, window.getSize().y / 2.0 );
	//end particles////////////////////////////////////	

	//background
	sf::Texture background_tile_texture1;
	sf::Sprite background_tile_sprite1;

	sf::Texture background_tile_texture2;
	sf::Sprite background_tile_sprite2;

	sf::Texture background_tile_texture3;
	sf::Sprite background_tile_sprite3;

	sf::Texture background_tile_texture4;
	sf::Sprite background_tile_sprite4;

	if(!background_tile_texture1.loadFromFile("images//cube_grid.png") )
	{
		printf("Failed to load texture on line %d \n", __LINE__);
	}

	background_tile_sprite1.setTexture( background_tile_texture1 );
	background_tile_sprite1.setColor( sf::Color(255, 255, 255, 255) );
	

	//player setup
	b2FixtureDef fixture;
	sf::Vector2f player_speed = sf::Vector2f(500.0, 600.0);
	Timer player_clock;
	Timer player_jump_clock;
	sf::Texture player_texture;

	if(!player_texture.loadFromFile("images//player.png") )
	{
		printf("Error %d : Failed to load player texture.\n", __LINE__);
		return -1;
	}

	fixture.restitution = 0.1;
	fixture.friction = 1.0;
	fixture.density = 2.0;
	Object player(window, world, fixture, player_texture, -1, Editor::BODY_TYPE::DYNAMIC, POLY_SHAPE);
	player.getBody()->SetFixedRotation(true);
	
	

	sf::Vector2f center_pos(window.getSize().x / 2.0, window.getSize().y / 2.0);
	sf::Vector2f view_size(window.getSize().x, window.getSize().y);
	sf::Vector2f level_size;

	//player.getBody()->SetTransform( b2Vec2(10.0 * PIXELS_TO_METERS, -level_size.y * PIXELS_TO_METERS), 0.0 );
	//sf::Vector2f view_size(level_size.x, level_size.y);
	//sf::Vector2f level_size(editor.getBackgroundTextures()[editor.getBackgroundIndex()].getSize().x, editor.getBackgroundTextures()[editor.getBackgroundIndex()].getSize().y);
	Camera main_view(center_pos, view_size, level_size);

	string file = "default.txt";
	editor.loadFile(window, world, main_view, player, file);

	cout << "static bodies: " << editor.getStaticObjects().size() << endl;
	cout << "dynamic bodies: " << editor.getDynamicObjects().size() << endl;
	cout << "kinematic bodies: " << editor.getKinematicObjects().size() << endl;

	//creates kinematic platform boundaries
	mouse_pos_world.x = -50.0;
	mouse_pos_world.y = window.getSize().y / 1.35;
	sf::Vector2f left_boundary(0.0, mouse_pos_world.y);
	sf::Vector2f right_boundary(main_view.getLevelSize().x, mouse_pos_world.y);

    while (window.isOpen())
    {
		window.setView( *(main_view.getView()) );

        while (window.pollEvent(e))
        {
            if (e.type == sf::Event::Closed)
                window.close();

			else if(e.type == sf::Event::KeyPressed && e.key.code == sf::Keyboard::Escape)
			{
				cout << "static body: " << editor.getStaticObjects().size() << endl;
				cout << "dynamic body: " << editor.getDynamicObjects().size() << endl;
				cout << "kinematic body: " << editor.getKinematicObjects().size() << endl << endl;
				window.close();
				return 0;
			}

			else if(e.type == sf::Event::LostFocus)
				in_focus = false;

			else if(e.type == sf::Event::GainedFocus)
				in_focus = true;

			else if(e.type == sf::Event::MouseWheelMoved)
			{				
				main_view.zoomControl(window, e);
			}
        }

		if(in_focus == true)
		{
			editor_clock.update();

			//checks for game mode change
			game_mode_clock.update();
			editor.gameModeToggle(game_mode_text, game_mode_clock, game_mode_live, game_mode_editor, game_state);

			//update mouse position
			mouse_clock.update();
			updateMousePos(mouse_pos, mouse_pos_world, window);

			if(game_state == Editor::GAME_STATE::LIVE) //if you're in live mode
			{
				main_view.boundaryControl(player.getSprite()->getPosition()); //camera follows player in live mode

				//update clocks
				player_clock.update();
				player_jump_clock.update();				

				//checks for player commands
				playerCommandUpdate(player_clock, player_jump_clock, player_speed, player, mouse_pos_world);

				//update world
				world->Step(time_step, velocity_iterations, position_iterations);
			
				particleToggle(player, mouse_clock, particle_toggle, particle_def, particle_system);
				levelBoundaries(editor, main_view, player); //keeps the player in the level
				Object::updatePosition(player); //updates the player sprite
				Object::updatePosition(editor.getDynamicObjects()); //updates the sprite position of the dynamic objects
				Object::updatePosition(editor.getKinematicObjects()); //updates the sprite position of the kinematic objects
				Physics::kinematicBoundaries(left_boundary, right_boundary, editor.getKinematicObjects());
			}

			else if(game_state == Editor::GAME_STATE::EDITOR) //if you're in editor mode
			{
				main_view.cursorBoundaryControl(mouse_pos); //camera follows mouse in edit mode
				//determines how to place objects to the screen in editor mode
				editor.keyboardActionCommands(window, main_view, world, player, mouse_clock, editor_object_type, mouse_pos_world, editor_type, game_state); 
				editor.keyboardCycleCommands(editor_clock); //used to cycle through objects in editor mode
			}			
			 
			window.clear(sf::Color(20, 20, 20) );//sf::Color(0, 255, 255) );
			//Draw::drawBackgroundGrid(window, main_view, background_tile_sprite1, background_tile_texture1);

			Draw::drawParticles(window, particle_shape, particle_system);			
			
			Draw::draw( window, editor.getDynamicObjects() ); //draws all the dynamic objects to the screen
			Draw::draw( window, editor.getKinematicObjects() ); //draws all the kinematic objects to the screen
			Draw::draw( window, editor.getStaticObjects() ); //draws all the static objects to the screen

			Draw::draw( window, player ); //draws the player to the screen
			Draw::drawText(window, game_mode_text, game_mode_text_pos);	//draws live/editor text to the screen		
			
			if(game_state == Editor::GAME_STATE::EDITOR) //draws editor only stuff
			{
				Draw::drawEditor(window, editor, mouse_pos_world, editor_object_type, editor_object_pos, game_state);
			}
			
			window.display();
		}        
    }
	
    return 0;
}

void updateMousePos(sf::Vector2i &pos, sf::Vector2f &world_pos, sf::RenderWindow &window)
{
	//mouse position relative to the window
	pos.x = sf::Mouse::getPosition(window).x;
	pos.y = sf::Mouse::getPosition(window).y;
	
	world_pos = window.mapPixelToCoords(pos); //gets the world coordinates for the mouse position
}

void playerCommandUpdate(Timer &movement_clock, Timer &jump_clock, sf::Vector2f &speed, Object &player, sf::Vector2f &mouse_pos)
{
	if( movement_clock.getElapsedTime() >= 0.05 )
		{
			if( sf::Keyboard::isKeyPressed( sf::Keyboard::D ) ) //moving right
			{
				b2Vec2 vel = player.getBody()->GetLinearVelocity();
				float desired_vel = getMin( vel.x + (speed.x / 50.0), speed.x * PIXELS_TO_METERS ); 
				float vel_change = desired_vel - vel.x; //max velocity - current velocity
				float impulse = player.getBody()->GetMass() * vel_change;
				player.getBody()->ApplyLinearImpulse( b2Vec2(impulse, 0), player.getBody()->GetWorldCenter(), true );
				movement_clock.restart();
			}

			else if( sf::Keyboard::isKeyPressed( sf::Keyboard::A ) ) //moving left
			{
				b2Vec2 vel = player.getBody()->GetLinearVelocity();
				float desired_vel = getMax( vel.x - (speed.x / 50.0), -speed.x * PIXELS_TO_METERS );
				float vel_change = desired_vel - vel.x; //max velocity - current velocity
				float impulse = player.getBody()->GetMass() * vel_change;
				player.getBody()->ApplyLinearImpulse( b2Vec2(impulse, 0), player.getBody()->GetWorldCenter(), true );
				movement_clock.restart();
			}

			else if( sf::Keyboard::isKeyPressed( sf::Keyboard::S ) ) //stop moving on the x axis
			{
				b2Vec2 vel;
				vel.x = 0.0;
				vel.y = player.getBody()->GetLinearVelocity().y * 2.0;
				vel.y = (vel.y > 0) ? (vel.y * -1) : vel.y;

				player.getBody()->SetLinearVelocity(vel);
				movement_clock.restart();
			}
		}

		if( jump_clock.getElapsedTime() >= 1.0 )
		{
			if( sf::Keyboard::isKeyPressed( sf::Keyboard::Space ) )
			{
				player.getBody()->SetLinearVelocity(b2Vec2(player.getBody()->GetLinearVelocity().x, 0.0) ); //cancels out the velocity on the x axis before the impulse
				b2Vec2 impulse(0.0, player.getBody()->GetMass() * 13);
				player.getBody()->ApplyLinearImpulse(impulse, player.getBody()->GetWorldCenter(), true);
				
				jump_clock.restart();
			}

			//resets the player to the mouse cursor position
			else if( sf::Keyboard::isKeyPressed( sf::Keyboard::LShift ) && sf::Keyboard::isKeyPressed( sf::Keyboard::R ) )
			{
				player.getBody()->SetTransform( b2Vec2(mouse_pos.x * PIXELS_TO_METERS, -mouse_pos.y * PIXELS_TO_METERS), player.getBody()->GetAngle() );
				
				jump_clock.restart();
			}
		}
}

void levelBoundaries(Editor &editor, Camera &view, Object &player)
{
	//if the player goes out of bounds, they return to the spawn point
	if(player.getSprite()->getPosition().x < 0 || player.getSprite()->getPosition().x > view.getLevelSize().x)
	{
		player.getBody()->SetTransform( b2Vec2(editor.getSpawnPoint().x * PIXELS_TO_METERS, -editor.getSpawnPoint().y * PIXELS_TO_METERS), player.getBody()->GetAngle() );
	}

	else if(player.getSprite()->getPosition().y < 0 || player.getSprite()->getPosition().y > view.getLevelSize().y)
	{
		player.getBody()->SetTransform( b2Vec2(editor.getSpawnPoint().x * PIXELS_TO_METERS, -editor.getSpawnPoint().y * PIXELS_TO_METERS), player.getBody()->GetAngle() );
	}
}

//updates the particle toggle
void particleToggle(Object &player, Timer &mouse_clock, bool &toggle, b2ParticleDef *p_def, b2ParticleSystem *p_system)
{
	if(mouse_clock.getElapsedTime() >= 0.5)
	{
		if( sf::Keyboard::isKeyPressed( sf::Keyboard::P ) )
		{
			toggle = (toggle == false) ? true : false;
			mouse_clock.restart();
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