#include <SFML/Graphics.hpp>
#include <Box2D\Box2D.h>
#include <iostream>
#include <vector>
#include <string>
#include <sstream>
#include "Timer.h"
#include "Object.h"
#include "Editor.h"
#include "Draw.h"
#include "Camera.h"
#include "Physics.h"
#include "Particle.h"
#include "Actor.h"
#include "Health.h"
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

string float_to_string(float f)
{
	std::stringstream stream;

	stream << f;

	return stream.str();
}

int main()
{
	static const std::vector<sf::VideoMode>& valid_video_mode = sf::VideoMode::getFullscreenModes();

    //sf::RenderWindow window(sf::VideoMode(valid_video_mode[0].width, valid_video_mode[0].height, valid_video_mode[0].bitsPerPixel), "Roboprototype", sf::Style::None);
	sf::RenderWindow window(sf::VideoMode(1600,900), "Roboprototype");
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

	//level timer setup
	sf::Text level_time_text;
	Timer level_clock;
	sf::Vector2i level_time_pos(window.getSize().x / 2.0, 10);

	level_time_text.setFont(font);
	level_time_text.setCharacterSize(60);
	level_time_text.setPosition( level_time_pos.x, level_time_pos.y );
	level_time_text.setString("Timer");
	level_time_text.scale(window.getSize().x / 1600.0, window.getSize().y / 900); //scales based on the window size it was created on
	level_time_text.setColor( sf::Color(255, 255, 255) );


	//Draw::drawText(window, level_time_text, sf::Vector2i(level_time_text.getPosition().x, level_time_text.getPosition().y) );

	//particles///////////
	Particle particles(world, window);

	//background
	sf::Texture background_tile_texture1;
	sf::Sprite background_tile_sprite1;

	if(!background_tile_texture1.loadFromFile("images//cube_grid.png") )
	{
		printf("Failed to load texture on line %d \n", __LINE__);
	}

	background_tile_sprite1.setTexture( background_tile_texture1 );
	background_tile_sprite1.setColor( sf::Color(255, 255, 255, 255) );
	

	//player setup
	int max_hp = 100;
	Actor actor(window, world, -1, Editor::BODY_TYPE::DYNAMIC, POLY_SHAPE);	

	sf::Vector2f center_pos(window.getSize().x / 2.0, window.getSize().y / 2.0);
	sf::Vector2f view_size(window.getSize().x, window.getSize().y);
	sf::Vector2f level_size;

	Camera main_view(center_pos, view_size, level_size);

	editor.current_level_index = Editor::FILE::TEST_LEVEL;
	editor.current_level = editor.levels[editor.current_level_index];

	editor.loadFile(window, world, main_view, *(actor.getEntity()), editor.current_level);
	//editor.addLevelBoundaries(window, main_view, world); //use when creating a new level. Save the boundaries and then comment out.

	//creates kinematic platform boundaries
	mouse_pos_world.x = -50.0;
	mouse_pos_world.y = window.getSize().y / 1.35;
	sf::Vector2f left_boundary(0.0, mouse_pos_world.y);
	sf::Vector2f right_boundary(main_view.getLevelSize().x, mouse_pos_world.y);
	
	particles.spawn(world, actor.getEntity()->getSprite()->getPosition());

    while (window.isOpen())
    {
		window.setView( *(main_view.getView()) );

        while (window.pollEvent(e))
        {
            if (e.type == sf::Event::Closed)
                window.close();

			else if(e.type == sf::Event::KeyPressed && e.key.code == sf::Keyboard::Escape)
			{
				window.close();
				return 0;
			}

			else if(e.type == sf::Event::LostFocus)
			{
				in_focus = false;
				level_clock.pause();
			}

			else if(e.type == sf::Event::GainedFocus)
			{
				in_focus = true;

				if(game_state == Editor::GAME_STATE::LIVE) //only resumes clock if the game mode is live
					level_clock.resume();
			}

			else if(e.type == sf::Event::MouseWheelMoved)
			{				
				main_view.zoomControl(window, e);
			}
        }

		if(in_focus == true)
		{
			editor_clock.update();
			particles.updateClocks();
			level_clock.update();
			
			//checks for game mode change
			game_mode_clock.update();
			editor.gameModeToggle(game_mode_text, game_mode_clock, level_clock, game_mode_live, game_mode_editor, game_state);

			//update mouse position
			mouse_clock.update();
			updateMousePos(mouse_pos, mouse_pos_world, window);

			if(game_state == Editor::GAME_STATE::LIVE) //if you're in live mode
			{
				main_view.boundaryControl(actor.getEntity()->getSprite()->getPosition()); //camera follows player in live mode

				//put all of this in the isLevelComplete() function.
				if(actor.isLevelComplete() == true) //the the player finished the level
				{
					actor.loadNextLevel(window, world, editor, main_view); 
					particles.spawn(world, actor.getEntity()->getSprite()->getPosition());
					left_boundary = sf::Vector2f(0.0, mouse_pos_world.y);
					right_boundary = sf::Vector2f(main_view.getLevelSize().x, mouse_pos_world.y);
					
					level_clock.restart();
				}

				//update clocks
				actor.updateClocks();			

				//checks for player commands
				actor.commandUpdate(mouse_pos_world);

				//update world
				world->Step(time_step, velocity_iterations, position_iterations);
				Object::updatePosition(*(actor.getEntity())); //updates the player sprite

				if(actor.contactUpdate(window, world, editor, main_view, particles) == Editor::ENTITY_CATEGORY::BOUNCE)
				{
					
					sf::Vector2f p_texture( actor.getTexture()->getSize() ); //player texture
					sf::Vector2f b_texture( editor.getStaticTextures()[Editor::STATIC::BOUNCE_PLATFORM].getSize() ); //bounce texture

					b2Vec2 *platform_position =  actor.getEntityContactPos();
					actor.getEntity()->getBody()->SetTransform( b2Vec2( actor.getEntity()->getBody()->GetPosition().x, platform_position->y + ( ( (b_texture.y / 2.0) + (p_texture.y / 2.0) + 5) * PIXELS_TO_METERS ) ), actor.getEntity()->getBody()->GetAngle() );

					actor.getEntity()->getBody()->SetLinearVelocity(b2Vec2(actor.getEntity()->getBody()->GetLinearVelocity().x, 0.0) ); //cancels out the velocity on the x axis before the impulse
					b2Vec2 impulse(0.0, actor.getEntity()->getBody()->GetMass() * 30);
					actor.getEntity()->getBody()->ApplyLinearImpulse(impulse, actor.getEntity()->getBody()->GetWorldCenter(), true);
					particles.bounce(world, actor.getEntity()->getSprite()->getPosition() );
					
				}
				
			
				particles.playerHair(world, *(actor.getEntity())); 

				
				
				//PUT THIS IN A FUNCTION (editor class)
				if(editor.levelBoundaries(main_view, *(actor.getEntity()) ) == true) //if the player went out of bounds
				{
					//Figure out why it hits this right when it starts.
					particles.explosion(world, actor.getEntity()->getSprite()->getPosition());
					actor.getHealthBar()->damage( actor.getHealthBar()->getMaxHealth() );
					actor.death();

					particles.getSystemClocks()[Particle::TYPE::EXPLOSION].restart();
				} 
				

				//updates player health bar to the current position
				actor.getHealthBar()->updateBar( sf::Vector2f(actor.getEntity()->getSprite()->getPosition().x, actor.getEntity()->getSprite()->getPosition().y) );

				Object::updatePosition(editor.getDynamicObjects()); //updates the sprite position of the dynamic objects
				Object::updatePosition(editor.getKinematicObjects()); //updates the sprite position of the kinematic objects
				Physics::kinematicBoundaries(left_boundary, right_boundary, editor.getKinematicObjects());
			}

			else if(game_state == Editor::GAME_STATE::EDITOR) //if you're in editor mode
			{
				main_view.cursorBoundaryControl(mouse_pos); //camera follows mouse in edit mode
				//determines how to place objects to the screen in editor mode
				editor.keyboardActionCommands(window, main_view, world, *(actor.getEntity()), mouse_clock, editor_object_type, mouse_pos_world, editor_type, game_state); 
				editor.keyboardCycleCommands(editor_clock); //used to cycle through objects in editor mode
			}			
			 
			window.clear(sf::Color(30, 30, 30) );//sf::Color(0, 255, 255) );
			//Draw::drawBackgroundGrid(window, main_view, background_tile_sprite1, background_tile_texture1);
			
			if(actor.isAlive())
				Draw::drawParticles(window, world, particles, Particle::TYPE::HAIR); //draws the player hair to the screen

			Draw::drawParticles(window, world, particles, Particle::TYPE::BLOOD_SPLATTER); //draws all blood splatters to the screen
			Draw::drawParticles(window, world, particles, Particle::TYPE::EXPLOSION); //draws all explosions to the screen
			Draw::drawParticles(window, world, particles, Particle::TYPE::SPAWN); //draws all the spawn explosions to the screen
			Draw::drawParticles(window, world, particles, Particle::TYPE::BOUNCE); //draws all the bounce particles to the screen
			
			Draw::draw( window, editor.getDynamicObjects() ); //draws all the dynamic objects to the screen
			Draw::draw( window, editor.getKinematicObjects() ); //draws all the kinematic objects to the screen
			Draw::draw( window, editor.getStaticObjects() ); //draws all the static objects to the screen

			if(actor.isAlive() == true)
			{
				Draw::draw( window, *(actor.getEntity()) ); //draws the player to the screen
				window.draw( *actor.getHealthBar()->getBar() );
			}

			else
			{
				//actor.respawn(editor); //respaws the character at the spawn point after a period of time
				actor.respawn( window, world, editor, main_view, *(actor.getEntity()), editor.getFileName() );

				if(actor.isAlive() == true) //creates the spawn explosion right when the character respawns
					particles.spawn(world, actor.getEntity()->getSprite()->getPosition() );

				level_clock.restart(); //starts the level timer over since the player is respawning
			}				

			Draw::drawText(window, game_mode_text, game_mode_text_pos);	//draws live/editor text to the screen

			//PUT THIS IN A FUNCTION
			level_time_text.setString( float_to_string( level_clock.getElapsedTime() ) );
			string temp_string = level_time_text.getString();
			temp_string = temp_string.substr( 0, temp_string.find('.') + 2 );
			level_time_text.setString( temp_string );

			Draw::drawText(window, level_time_text, level_time_pos );
			
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

