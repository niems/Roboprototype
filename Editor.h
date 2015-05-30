#ifndef EDITOR_H
#define EDITOR_H

#include <Box2D\Box2D.h>
#include <SFML\Graphics.hpp>
#include <vector>
#include <fstream>
#include <stdlib.h>
#include <time.h>
#include <iostream>
#include "Object.h"
#include "Timer.h"
#include "Physics.h"
#include "Camera.h"
using namespace std;

/*
Used to store all the objects created in the world, and also
allows the placement of more objects
*/

/*
load in all the sprites in the editor constructor
3 vectors of sprites, each is one of the body types. ex. static_sprites, dynamic_sprites, kinematic_sprites
reset current index to 0 when switching types
use 3 enumerators to tell in main.cpp which sprite you want out of the vector instead of just indexing
a random number. That will make it easier to read.
*/

class Editor
{
private:
	sf::Vector2f spawn_point; //starts player here when the level starts or if they die
	int object_type; //determines which type of object to show(static/dynamic/kinematic)
	int current_index; //this is the index of the current object
	int current_background_index; //the background being used with the current level

	vector<sf::Sprite> background_sprites;
	vector<sf::Texture> background_textures;

	vector<sf::Sprite> static_sprite; //stores the sprites that are used to cycle through in the editor
	vector<sf::Texture> static_texture; //parallel vector that stores the size of the sprite

	vector<sf::Sprite> dynamic_sprite; //stores the sprites that are used to cycle through in the editor
	vector<sf::Texture> dynamic_texture; //parallel vector that stores the size of the sprite

	vector<sf::Sprite> kinematic_sprite; //stores the sprites that are used to cycle through in the editor
	vector<sf::Texture> kinematic_texture; //parallel vector that stores the size of the sprite

	vector<Object*> static_object;      //stores all the static objects in the world
	vector<Object*> dynamic_object;    //stores all the dynamic objects in the world
	vector<Object*> kinematic_object; //stores all the kinematic objects in the world

public:
	Editor();
	enum GAME_STATE {LIVE, EDITOR}; //two game states
	enum BODY_TYPE{STATIC, DYNAMIC, KINEMATIC}; //determines what type of box2d body to create	
	enum STATIC{SMALL_PLATFORM, MED_PLATFORM1, MED_PLATFORM2, MED_PLATFORM3, MED_PLATFORM4, MED_PLATFORM5, MED_PLATFORM6, MED_PLATFORM7, LARGE_PLATFORM, H_BOUNDARY, V_BOUNDARY, BOUNCE_PLATFORM}; 
	enum DYNAMIC{CRATE, CRATE2}; 
	enum KINEMATIC{MOVING_PLATFORM, SPIKES, SPHERE};

	void addStaticObject(Object *object);      //adds a static object to the vector
	void addDynamicObject(Object *object);    //adds a dynamic object to the vector
	void addKinematicObject(Object *object); //adds a kinematic object to the vector
	void setObjectType(int o_type); //sets the current object type
	void setCurrentIndex(int c_index); //sets the current index

	//determines how to place objects to the screen in editor mode
	void keyboardActionCommands(sf::RenderWindow &window, Camera &view, b2World *world, Object &player, Timer &mouse_clock, sf::Text &object_type, sf::Vector2f &mouse_pos, string *editor_type);
	void keyboardCycleCommands(Timer &editor_clock); //used to cycle through objects in editor mode

	//allows you toggle between live and editor mode
	void gameModeToggle(sf::Text &mode_text, Timer &clock, string &live, string &edit, int &game_state);

	//cycles through all objects to see if the cursor is intersecting any.
	//objects that are being intersected are deleted.
	void deleteObject(b2World *world, sf::Vector2f &mouse_pos);

	//creates the static body at the mouse cursor
	void createStaticBody(sf::RenderWindow &window, b2World *world, sf::Vector2f &mouse_pos);
	
	//creates the dynamic body at the mouse cursor
	void createDynamicBody(sf::RenderWindow &window, b2World *world, sf::Vector2f &mouse_pos);

	//creates the kinematic body at the mouse cursor
	void createKinematicBody(sf::RenderWindow &window, b2World *world, sf::Vector2f &mouse_pos);

	//saves the current map to the file
	void saveFile(Camera &view, Object &player, string &file_name);

	//loads the current map from the file
	void loadFile(sf::RenderWindow &window, b2World *world, Camera &view, Object &player, string &file_name);

	int getObjectType(); //returns the current object type
	int getCurrentIndex(); //returns the index of the current object
	int getBackgroundIndex(); //returns the index of the current background
	
	sf::Vector2f& getSpawnPoint(); //returns the spawn point for the current level
	sf::Sprite* getCurrentBackground(); //returns the background for the current level
	vector<sf::Texture>& getBackgroundTextures(); //returns the background textures

	vector<Object*> getStaticObjects();      //returns all static objects
	vector<Object*> getDynamicObjects();    //returns all dynamic objects
	vector<Object*> getKinematicObjects(); //returns all kinematic objects

	vector<sf::Sprite>& getStaticSprites();      //returns all the loaded static sprites
	vector<sf::Sprite>& getDynamicSprites();    //returns all the loaded dynamic sprites
	vector<sf::Sprite>& getKinematicSprites(); //returns all the loaded kinematic sprites

	vector<sf::Texture>& getStaticTextures(); //returns all the textures for the static sprites loaded
	vector<sf::Texture>& getDynamicTextures(); //returns all the textures for the dynamic sprites loaded
	vector<sf::Texture>& getKinematicTextures(); //returns all the textures for the kinematic sprites loaded

	
};


#endif