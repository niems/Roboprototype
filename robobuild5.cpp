#include <SFML/Graphics.hpp>
#include <Box2D\Box2D.h>
#include <iostream>
#include <vector>
#include <string>
#include "Timer.h"
#include "Object.h"
#include "Editor.h"
#include "Draw.h"
using namespace std;

#define DEGTORAD 0.0174532925199432957f
#define RADTODEG 57.295779513082320876f

const float PIXELS_TO_METERS = 0.03333; //number of meters in one pixel
const float METERS_TO_PIXELS = 30.0; //number of pixels in one meter

enum GAME_STATE {LIVE, EDITOR};
enum {STATIC, DYNAMIC, KINEMATIC}; //determines what type of box2d body to create
enum {POLY_SHAPE, CIRCLE_SHAPE}; //determines what type of box2d shape to create
enum STATIC{SMALLEST_PLATFORM, SMALL_PLATFORM, LARGE_PLATFORM}; //use current index to reference which platform to create

//updates the mouse position
void updateMousePos(sf::Vector2f &pos, sf::RenderWindow &window);

//checks for player commands
void playerCommandUpdate(Timer &movement_clock, Timer &jump_clock, sf::Vector2f &speed, Object &player);

//updates the particle toggle
void particleToggle(Object &player, Timer &mouse_clock, bool &toggle, b2ParticleDef *p_def, b2ParticleSystem *p_system);

//used to control a kinematic body between two boundaries
void kinematicBoundaries(Object *object, sf::Vector2f &left_boundary, sf::Vector2f &right_boundary);

//checks if you switched between live and editor mode
void gameModeToggle(Editor &editor, sf::Text &mode_text, Timer &clock, string &live, string &edit, int &game_state);


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
	
	//background setup
	sf::Texture background_texture;
	sf::Sprite background_sprite;

	if(!background_texture.loadFromFile("images//mountain_background.png") )
	{
		printf("Failed to load background image on line %d \n", __LINE__);
		window.close();
	}

	background_sprite.setTexture( background_texture );

	//game mode setup(live/editor)
	Editor editor; //used to store all objects and edit the level
	int game_state = GAME_STATE::LIVE; //current mode
	Timer editor_clock;
	Timer game_mode_clock;
	sf::Font font;
	sf::Text game_mode_text; //used to display the current game mode
	sf::Text editor_current_type; //used to display the current type of object in the editor mode
	string game_mode_live = "Mode: Live";
	string game_mode_editor = "Mode: Editor";
	string editor_type[3] = {"Object Type: Static", "Object Type: Dynamic", "Object Type: Kinematic"};


	if(!font.loadFromFile("fonts//amatic.ttf") )
	{
		printf("Failed to load font on line %d \n", __LINE__);
		window.close();
	}

	game_mode_text.setFont(font);
	game_mode_text.setString( (game_state == GAME_STATE::LIVE) ? game_mode_live : game_mode_editor );
	game_mode_text.setCharacterSize(50);
	game_mode_text.scale( window.getSize().x / 1600.0, window.getSize().y / 900); //scales based on the window size it was created on
	game_mode_text.setPosition(10.0, 10.0);
	game_mode_text.setColor( sf::Color(0, 0, 0) );

	editor_current_type.setFont(font);
	editor_current_type.setString( editor_type[editor.getObjectType()] );
	editor_current_type.setCharacterSize(50);
	editor_current_type.scale(window.getSize().x / 1600.0, window.getSize().y / 900); //scales based on the window size it was created on
	editor_current_type.setPosition( game_mode_text.getPosition().x, game_mode_text.getPosition().y + editor_current_type.getGlobalBounds().height + 20.0);
	editor_current_type.setColor( sf::Color(0, 0, 0) );

	//mouse setup
	Timer mouse_clock;
	sf::Vector2f mouse_pos;

	//particles///////////
	bool particle_toggle = true; //if true, particles are constantly created
	sf::CircleShape particle_shape;
	particle_shape.setFillColor( sf::Color(255, 255, 255, 180) );
	particle_shape.setOutlineThickness(3);
	particle_shape.setOutlineColor( sf::Color(255, 0, 0, 150) );
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
	particle_def->color.Set(0, 255, 255, 255);
	particle_def->flags = b2_elasticParticle;
	particle_def->position.Set( window.getSize().x / 2.0, window.getSize().y / 2.0 );
	//end particles////////////////////////////////////
	
	

	//base platform
	
	Object *temp_object = NULL;
	b2FixtureDef fixture;
	sf::Texture platform_texture;

	if(!platform_texture.loadFromFile("images//platform.png"))
	{
		printf("Failed to load platform texture on line %d \n", __LINE__);
		window.close();
	}


	//put any object I could use in the game in a function in the editor class!
	fixture.density = 1;
	fixture.restitution = 0.05;
	fixture.friction = 0.75;

	//creates large platform
	mouse_pos.x = window.getSize().x / 2.0;
	mouse_pos.y = window.getSize().y / 1.1;
	editor.setCurrentIndex(STATIC::LARGE_PLATFORM);
	editor.createStaticBody(window, world, mouse_pos);
	

	//smaller platform
	sf::Texture small_platform_texture;

	if(!small_platform_texture.loadFromFile("images//small_platform.png") )
	{
		printf("Failed to load small platform texture on line %d \n", __LINE__);
		window.close();
	}

	//creates small platform
	mouse_pos.x = small_platform_texture.getSize().x / 2.0;
	mouse_pos.y = window.getSize().y / 1.25;
	editor.setCurrentIndex(STATIC::SMALL_PLATFORM);
	editor.createStaticBody(window, world, mouse_pos);

	fixture.friction = 1.0;
	temp_object = new Object(window, world, fixture, small_platform_texture, KINEMATIC, POLY_SHAPE);
	temp_object->getSprite()->setColor( sf::Color(0, 0, 0, 200) );
	temp_object->getBody()->SetTransform( b2Vec2( (small_platform_texture.getSize().x * 1.5) * PIXELS_TO_METERS, -(window.getSize().y / 1.45) * PIXELS_TO_METERS), 0.0 );
	temp_object->getBody()->SetLinearVelocity( b2Vec2(200.0 * PIXELS_TO_METERS, 0.0) );
	temp_object->updateSpritePos();
	sf::Vector2f left_boundary(temp_object->getSprite()->getPosition().x, temp_object->getSprite()->getPosition().y);
	sf::Vector2f right_boundary( (window.getSize().x - small_platform_texture.getSize().x / 2.0), temp_object->getSprite()->getPosition().y );
	editor.addKinematicObject(temp_object);
	

	//player setup
	sf::Vector2f player_speed = sf::Vector2f(400.0, 600.0);
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
	Object player(window, world, fixture, player_texture, DYNAMIC, POLY_SHAPE);
	player.getBody()->SetFixedRotation(true);
	player.getBody()->SetTransform( b2Vec2( 50 * PIXELS_TO_METERS, -(window.getSize().y / 2.0) * PIXELS_TO_METERS), 0.0 );
	player.updateSpritePos();


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
			/*
			fps_clock.update();
			cout << 1.0 / fps_clock.getElapsedTime() << endl;
			fps_clock.restart();
			*/

			editor_clock.update();

			//checks for game mode change
			game_mode_clock.update();
			gameModeToggle(editor, game_mode_text, game_mode_clock, game_mode_live, game_mode_editor, game_state);

			//update mouse position
			mouse_clock.update();
			updateMousePos(mouse_pos, window);

			if(game_state == GAME_STATE::LIVE) //if you're in live mode
			{
				//update clocks
				player_clock.update();
				player_jump_clock.update();				

				//checks for player commands
				playerCommandUpdate(player_clock, player_jump_clock, player_speed, player);

				//update world
				world->Step(time_step, velocity_iterations, position_iterations);
			
				particleToggle(player, mouse_clock, particle_toggle, particle_def, particle_system);
				Object::updatePosition(player); //updates the player sprite
				Object::updatePosition(editor.getDynamicObjects());
				Object::updatePosition(editor.getKinematicObjects());
				kinematicBoundaries(editor.getKinematicObjects()[0], left_boundary, right_boundary);
			}

			else if(game_state == GAME_STATE::EDITOR) //if you're in editor mode
			{
				if(mouse_clock.getElapsedTime() >= 0.5)
				{
					if(sf::Mouse::isButtonPressed( sf::Mouse::Left ) ) //if the user wants to place the current item
					{
						if(editor.getObjectType() == STATIC)
						{
							editor.createStaticBody(window, world, mouse_pos);
						}

						else if(editor.getObjectType() == DYNAMIC)
						{
							editor.createDynamicBody(window, world, mouse_pos);
						}

						else if(editor.getObjectType() == KINEMATIC)
						{

						}

						mouse_clock.restart();
					}

					else if(sf::Mouse::isButtonPressed( sf::Mouse::Right ) )
					{
						//erase last
					}

					//switches to displaying static objects
					else if(sf::Keyboard::isKeyPressed( sf::Keyboard::Num1 ) || sf::Keyboard::isKeyPressed( sf::Keyboard::Numpad1 ) )
					{
						editor.setCurrentIndex(0); //reset index
						editor.setObjectType(STATIC);
						editor_current_type.setString( editor_type[editor.getObjectType()] );
						mouse_clock.restart();
					}

					//switches to displaying dynamic objects
					else if(sf::Keyboard::isKeyPressed( sf::Keyboard::Num2 ) || sf::Keyboard::isKeyPressed( sf::Keyboard::Numpad2 ) )
					{
						editor.setCurrentIndex(0); //reset index
						editor.setObjectType(DYNAMIC);
						editor_current_type.setString( editor_type[editor.getObjectType()] );
						mouse_clock.restart();
					}

					//switches to displaying kinematic objects
					else if(sf::Keyboard::isKeyPressed( sf::Keyboard::Num3 ) || sf::Keyboard::isKeyPressed( sf::Keyboard::Numpad3 ) )
					{
						editor.setCurrentIndex(0); //reset index
						editor.setObjectType(KINEMATIC);
						editor_current_type.setString( editor_type[editor.getObjectType()] );
						mouse_clock.restart();
					}
				}

				if(editor_clock.getElapsedTime() >= 0.3)
				{
					if(sf::Keyboard::isKeyPressed( sf::Keyboard::Q ) ) //cycle backwards in sprite vector
					{
						int index = editor.getCurrentIndex() - 1;

						if(editor.getObjectType() == STATIC)
						{
							index = (index < 0) ? (editor.getStaticSprites().size() - 1) : index;
						}

						else if(editor.getObjectType() == DYNAMIC)
						{
							index = (index < 0) ? (editor.getDynamicSprites().size() - 1) : index;
						}

						else if(editor.getObjectType() == KINEMATIC)
						{
							index = (index < 0) ? (editor.getKinematicSprites().size() - 1) : index;
						}

						editor.setCurrentIndex(index);
						editor_clock.restart();

						cout << "current index : " << editor.getCurrentIndex() << endl;
					}

					else if(sf::Keyboard::isKeyPressed( sf::Keyboard::E ) ) //cycle forwards in sprite vector
					{
						int index = editor.getCurrentIndex() + 1;

						if(editor.getObjectType() == STATIC)
						{
							index = (index >= editor.getStaticSprites().size()) ? 0 : index;
						}

						else if(editor.getObjectType() == DYNAMIC)
						{
							index = (index >= editor.getDynamicSprites().size()) ? 0 : index;
						}

						else if(editor.getObjectType() == KINEMATIC)
						{
							index = (index >= editor.getKinematicSprites().size()) ? 0 : index; 
						}

						editor.setCurrentIndex(index);
						editor_clock.restart();

						cout << "current index: " << editor.getCurrentIndex() << endl;
					}
				}				
			}
			
			 
			window.clear(sf::Color(187, 255, 255) ); 

			Draw::drawParticles(window, particle_shape, particle_system);
			Draw::draw( window, editor.getStaticObjects() ); //draws all the static objects to the screen
			Draw::draw( window, editor.getDynamicObjects() ); //draws all the dynamic objects to the screen
			Draw::draw( window, editor.getKinematicObjects() ); //draws all the kinematic objects to the screen
			Draw::draw( window, player ); //draws the player to the screen
			Draw::drawText(window, game_mode_text);


			//put all of this in a function
			if(game_state == GAME_STATE::EDITOR) //draws editor only stuff
			{
				if(editor.getObjectType() == STATIC) //if you're looking at static objects
				{
					if(editor.getStaticSprites().size() > 0) //if there are static sprites
					{
						editor.getStaticSprites()[editor.getCurrentIndex()].setPosition(mouse_pos.x, mouse_pos.y);
						window.draw( editor.getStaticSprites()[editor.getCurrentIndex()] );
					}
				}

				else if(editor.getObjectType() == DYNAMIC)
				{
					if(editor.getDynamicSprites().size() > 0) //if there are dynamic sprites
					{
						editor.getDynamicSprites()[editor.getCurrentIndex()].setPosition(mouse_pos.x, mouse_pos.y);
						window.draw( editor.getDynamicSprites()[editor.getCurrentIndex()] );
					}
				}

				else if(editor.getObjectType() == KINEMATIC)
				{
					if(editor.getKinematicSprites().size() > 0) //if there are kinematic sprites
					{
						editor.getKinematicSprites()[editor.getCurrentIndex()].setPosition(mouse_pos.x, mouse_pos.y);
						window.draw( editor.getKinematicSprites()[editor.getCurrentIndex()] );
					}
				}

				Draw::drawText(window, editor_current_type);
			}
			
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

void playerCommandUpdate(Timer &movement_clock, Timer &jump_clock, sf::Vector2f &speed, Object &player)
{
	if( movement_clock.getElapsedTime() >= 0.05 )
		{
			if( sf::Keyboard::isKeyPressed( sf::Keyboard::D ) ) //moving right
			{
				b2Vec2 vel = player.getBody()->GetLinearVelocity();

				if( vel.x < 0.0 ) //player was just moving left
					player.getBody()->SetAngularVelocity(0.0); //stops rotation

				vel.x = speed.x * PIXELS_TO_METERS;
				player.getBody()->SetLinearVelocity(vel);
				movement_clock.restart();
			}

			else if( sf::Keyboard::isKeyPressed( sf::Keyboard::A ) ) //moving left
			{
				b2Vec2 vel = player.getBody()->GetLinearVelocity();

				if( vel.x > 0.0 ) //player was just moving right
					player.getBody()->SetAngularVelocity(0.0); //stops rotation

				vel.x = -speed.x * PIXELS_TO_METERS;
				player.getBody()->SetLinearVelocity(vel);
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
				b2Vec2 impulse(0.0, player.getBody()->GetMass() * 13);
				player.getBody()->ApplyLinearImpulse(impulse, player.getBody()->GetWorldCenter(), true);
				
				jump_clock.restart();
			}
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
			

			p_def->lifetime = 0.25;
			p_def->position.Set( pos.x, pos.y );
			p_system->CreateParticle(*p_def);
		}		
	}
}


//used to control a kinematic body between two boundaries
void kinematicBoundaries(Object *object, sf::Vector2f &left_boundary, sf::Vector2f &right_boundary)
{
	sf::Vector2f pos( object->getBody()->GetPosition().x * METERS_TO_PIXELS, object->getBody()->GetPosition().y * METERS_TO_PIXELS );

	b2Vec2 vel = object->getBody()->GetLinearVelocity();
	vel.x = (vel.x > 0) ? vel.x : (vel.x * -1); //makes x velocity positive

	if(pos.x <= left_boundary.x) //if the platform is at the left boundary, reverse
	{
		object->getBody()->SetLinearVelocity(vel);
	}

	else if(pos.x >= right_boundary.x) //if the platform is at the right boundary, reverse
	{
		vel.x *= -1;
		object->getBody()->SetLinearVelocity(vel);
	}
}

void gameModeToggle(Editor &editor, sf::Text &mode_text, Timer &clock, string &live, string &edit, int &game_state)
{
	if(clock.getElapsedTime() >= 1.0) //long enough to toggle between game modes
	{
		if(sf::Keyboard::isKeyPressed( sf::Keyboard::Tilde ) )
		{
			clock.restart();
			editor.setCurrentIndex(0); //reset index 
			game_state = (game_state == GAME_STATE::LIVE) ? GAME_STATE::EDITOR : GAME_STATE::LIVE; //assigns the opposite mode
			mode_text.setString( (game_state == GAME_STATE::LIVE) ? live : edit ); //assigns corresponding string
		}
	}
}