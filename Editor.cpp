#include "Editor.h"


enum {POLY_SHAPE, CIRCLE_SHAPE}; //determines what type of box2d shape to create

#define DEGTORAD 0.0174532925199432957f
#define RADTODEG 57.295779513082320876f

const float PIXELS_TO_METERS = 0.03333; //number of meters in one pixel
const float METERS_TO_PIXELS = 30.0; //number of pixels in one meter

Editor::Editor()
{
	this->object_type = BODY_TYPE::STATIC; //default starting type
	this->current_index = 0; //default starting index
	this->current_background_index = 0; //default starting background
	this->angle = 0.0;

	//load static sprites
	sf::Texture small_platform_texture;
	sf::Sprite small_platform_sprite;

	sf::Texture med_platform_texture1;
	sf::Sprite med_platform_sprite1;  

	sf::Texture med_platform_texture2;
	sf::Sprite med_platform_sprite2;

	sf::Texture med_platform_texture3;
	sf::Sprite med_platform_sprite3;

	sf::Texture med_platform_texture4;
	sf::Sprite med_platform_sprite4;

	sf::Texture med_platform_texture5;
	sf::Sprite med_platform_sprite5;

	sf::Texture med_platform_texture6;
	sf::Sprite med_platform_sprite6;

	sf::Texture med_platform_texture7;
	sf::Sprite med_platform_sprite7;

	sf::Texture large_platform_texture;
	sf::Sprite large_platform_sprite;	

	sf::Texture horizontal_boundary_texture;
	sf::Sprite horizontal_boundary_sprite;

	sf::Texture vertical_boundary_texture;
	sf::Sprite vertical_boundary_sprite;

	sf::Texture bounce_platform_texture;
	sf::Sprite bounce_platform_sprite;

	if(!small_platform_texture.loadFromFile("images//plat13.png") )
	{
		printf("Failed to load texture on line %d \n", __LINE__);
	}

	this->static_texture.push_back( small_platform_texture );
	small_platform_sprite.setTexture( this->static_texture.back() );
	small_platform_sprite.setOrigin( small_platform_texture.getSize().x / 2.0, small_platform_texture.getSize().y / 2.0 );
	this->static_sprite.push_back(small_platform_sprite);


	if(!med_platform_texture1.loadFromFile("images//nplat5.png") )
	{
		printf("Failed to load texture on line %d \n", __LINE__);
	}

	this->static_texture.push_back( med_platform_texture1 );
	med_platform_sprite1.setTexture( this->static_texture.back() );
	med_platform_sprite1.setOrigin( med_platform_texture1.getSize().x / 2.0, med_platform_texture1.getSize().y / 2.0 );
	this->static_sprite.push_back( med_platform_sprite1 );

	if(!med_platform_texture2.loadFromFile("images//nplat6.png") )
	{
		printf("Failed to load texture on line %d \n", __LINE__);
	}

	this->static_texture.push_back( med_platform_texture2 );
	med_platform_sprite2.setTexture( this->static_texture.back() );
	med_platform_sprite2.setOrigin( med_platform_texture2.getSize().x / 2.0, med_platform_texture2.getSize().y / 2.0 );
	this->static_sprite.push_back( med_platform_sprite2 );

	if(!med_platform_texture3.loadFromFile("images//nplat1.png") )
	{
		printf("Failed to load texture on line %d \n", __LINE__);
	}

	this->static_texture.push_back( med_platform_texture3 );
	med_platform_sprite3.setTexture( this->static_texture.back() );
	med_platform_sprite3.setOrigin( med_platform_texture3.getSize().x / 2.0, med_platform_texture3.getSize().y / 2.0 );
	this->static_sprite.push_back( med_platform_sprite3 );

	if(!med_platform_texture4.loadFromFile("images//moving_platform2.png") )
	{
		printf("Failed to load texture on line %d \n", __LINE__);
	}

	this->static_texture.push_back( med_platform_texture4 );
	med_platform_sprite4.setTexture( this->static_texture.back() );
	med_platform_sprite4.setOrigin( med_platform_texture4.getSize().x / 2.0, med_platform_texture4.getSize().y / 2.0 );
	this->static_sprite.push_back( med_platform_sprite4 );

	if(!med_platform_texture5.loadFromFile("images//plat15.png") )
	{
		printf("Failed to load texture on line %d \n", __LINE__);
	}
	
	this->static_texture.push_back( med_platform_texture5 );
	med_platform_sprite5.setTexture( this->static_texture.back() );
	med_platform_sprite5.setOrigin( med_platform_texture5.getSize().x / 2.0, med_platform_texture5.getSize().y / 2.0 );
	this->static_sprite.push_back( med_platform_sprite5 );

	if(!med_platform_texture6.loadFromFile("images//plat20.png") )
	{
		printf("Failed to load texture on line %d \n", __LINE__);
	}

	this->static_texture.push_back( med_platform_texture6 );
	med_platform_sprite6.setTexture( this->static_texture.back() );
	med_platform_sprite6.setOrigin( med_platform_texture6.getSize().x / 2.0, med_platform_texture6.getSize().y / 2.0 );
	this->static_sprite.push_back( med_platform_sprite6 );

	if(!med_platform_texture7.loadFromFile("images//nplat8.png") )
	{
		printf("Failed to load texture on line %d \n", __LINE__);
	}

	this->static_texture.push_back( med_platform_texture7 );
	med_platform_sprite7.setTexture( this->static_texture.back() );
	med_platform_sprite7.setOrigin( med_platform_texture7.getSize().x / 2.0, med_platform_texture7.getSize().y / 2.0 );
	this->static_sprite.push_back( med_platform_sprite7 );

	if(!large_platform_texture.loadFromFile("images//nplat7.png") )
	{
		printf("Failed to load texture on line %d \n", __LINE__);
	}

	this->static_texture.push_back( large_platform_texture );

	large_platform_sprite.setTexture( this->static_texture.back() );
	large_platform_sprite.setOrigin( large_platform_texture.getSize().x / 2.0, large_platform_texture.getSize().y / 2.0 );
	this->static_sprite.push_back(large_platform_sprite);

	if(!horizontal_boundary_texture.loadFromFile("images//horizontal_boundary.png") )
	{
		printf("Failed to load texture on line %d \n", __LINE__);
	}

	this->static_texture.push_back( horizontal_boundary_texture );
	horizontal_boundary_sprite.setTexture( this->static_texture.back() );
	horizontal_boundary_sprite.setOrigin( horizontal_boundary_texture.getSize().x / 2.0, horizontal_boundary_texture.getSize().y / 2.0 );
	this->static_sprite.push_back( horizontal_boundary_sprite );

	if(!vertical_boundary_texture.loadFromFile("images//vertical_boundary.png") )
	{
		printf("Failed to load texture on line %d \n", __LINE__);
	}

	this->static_texture.push_back( vertical_boundary_texture );
	vertical_boundary_sprite.setTexture( this->static_texture.back() );
	vertical_boundary_sprite.setOrigin( vertical_boundary_texture.getSize().x / 2.0, vertical_boundary_texture.getSize().y / 2.0 );
	this->static_sprite.push_back( vertical_boundary_sprite );

	if(!bounce_platform_texture.loadFromFile("images//bounce_platform.png") )
	{
		printf("Failed to load texture on line %d \n", __LINE__);
	}

	this->static_texture.push_back( bounce_platform_texture );
	bounce_platform_sprite.setTexture( this->static_texture.back() );
	bounce_platform_sprite.setOrigin( bounce_platform_texture.getSize().x / 2.0, bounce_platform_texture.getSize().y / 2.0 );
	this->static_sprite.push_back( bounce_platform_sprite );
	

	//load dynamic sprites

	this->dynamic_texture.push_back( sf::Texture() );
	this->dynamic_sprite.push_back( sf::Sprite() );
	sf::Texture& box_texture = this->dynamic_texture.back();
	sf::Sprite& box_sprite = this->dynamic_sprite.back();

	if(!box_texture.loadFromFile("images//crate.png") )
	{
		printf("Failed to load texture on line %d \n", __LINE__);
	}

	

	box_sprite.setTexture( box_texture );
	box_sprite.setOrigin( box_texture.getSize().x / 2.0, box_texture.getSize().y / 2.0 );
	//this->dynamic_sprite.push_back( box_sprite );
	
	this->dynamic_texture.push_back( sf::Texture() );
	this->dynamic_sprite.push_back( sf::Sprite() );
	sf::Texture& crate_texture = this->dynamic_texture.back();
	sf::Sprite& crate_sprite = this->dynamic_sprite.back();

	if(!crate_texture.loadFromFile("images//crate2.png") )
	{
		printf("Failed to load texture on line %d \n", __LINE__);
	}

	//this->dynamic_texture.push_back( crate_texture );

	this->dynamic_sprite.back().setTexture( this->dynamic_texture.back() );
	this->dynamic_sprite.back().setOrigin( crate_texture.getSize().x / 2.0, crate_texture.getSize().y / 2.0 );
	//this->dynamic_sprite.push_back( crate_sprite );
	//this->dynamic_sprite.push_back( sphere_sprite );
	


	//load kinematic sprites
	sf::Texture moving_texture;
	sf::Sprite moving_sprite;

	sf::Texture spike_texture;
	sf::Sprite spike_sprite;

	sf::Texture sphere_texture;
	sf::Sprite sphere_sprite;

	if(!moving_texture.loadFromFile("images//moving_platform.png") )
	{
		printf("Failed to load texture on line %d \n", __LINE__);
	}

	this->kinematic_texture.push_back( moving_texture );

	moving_sprite.setTexture( this->kinematic_texture.back() );
	moving_sprite.setOrigin( moving_texture.getSize().x / 2.0, moving_texture.getSize().y / 2.0 );
	this->kinematic_sprite.push_back( moving_sprite );

	if(!spike_texture.loadFromFile("images//spike3.png") )
	{
		printf("Failed to load texture on line %d \n", __LINE__);
	}

	this->kinematic_texture.push_back( spike_texture );

	spike_sprite.setTexture( this->kinematic_texture.back() );
	spike_sprite.setOrigin( spike_texture.getSize().x / 2.0, spike_texture.getSize().y / 2.0 );
	this->kinematic_sprite.push_back( spike_sprite );

	if(!sphere_texture.loadFromFile("images//tech_sphere.png") )
	{
		printf("Failed to load texture on line %d \n", __LINE__);
	}

	this->kinematic_texture.push_back( sphere_texture );

	sphere_sprite.setTexture( this->kinematic_texture.back() );
	sphere_sprite.setOrigin( sphere_texture.getSize().x / 2.0, sphere_texture.getSize().y / 2.0 );
	this->kinematic_sprite.push_back( sphere_sprite );
	
}

void Editor::addStaticObject(Object *object)
{
	this->static_object.push_back(object);
}

void Editor::addDynamicObject(Object *object)
{
	this->dynamic_object.push_back(object);
}

void Editor::addKinematicObject(Object *object)
{
	this->kinematic_object.push_back(object);
}

void Editor::setObjectType(int o_type) //sets the current object type
{
	this->object_type = o_type;
}

void Editor::setCurrentIndex(int c_index) //sets the current index
{
	this->current_index = c_index;
}

//determines how to place objects to the screen in editor mode
void Editor::keyboardActionCommands(sf::RenderWindow &window, Camera &view, b2World *world, Object &player, Timer &mouse_clock, sf::Text &object_type, sf::Vector2f &mouse_pos, string *editor_type)
{
	if(mouse_clock.getElapsedTime() >= 0.5)
	{
		if(sf::Mouse::isButtonPressed( sf::Mouse::Left ) ) //if the user wants to place the current item
		{
			switch(this->object_type)
			{
			case BODY_TYPE::STATIC:
				this->createStaticBody(window, world, mouse_pos);
				break;
			case BODY_TYPE::DYNAMIC:
				this->createDynamicBody(window, world, mouse_pos);
				break;
			case BODY_TYPE::KINEMATIC:
				this->createKinematicBody(window, world, mouse_pos);
				break;
			}

			mouse_clock.restart();
		}

		else if(sf::Mouse::isButtonPressed( sf::Mouse::Right ) ) //erases object that was clicked on
		{
			this->deleteObject(world, mouse_pos);
		}

		//switches to displaying static objects
		else if(sf::Keyboard::isKeyPressed( sf::Keyboard::Num1 ) || sf::Keyboard::isKeyPressed( sf::Keyboard::Numpad1 ) )
		{
			this->current_index = 0; //reset index
			this->object_type = BODY_TYPE::STATIC;
			object_type.setString( editor_type[this->object_type] );
			mouse_clock.restart();
		}

		//switches to displaying dynamic objects
		else if(sf::Keyboard::isKeyPressed( sf::Keyboard::Num2 ) || sf::Keyboard::isKeyPressed( sf::Keyboard::Numpad2 ) )
		{
			this->current_index = 0; //reset index
			this->object_type = BODY_TYPE::DYNAMIC;
			object_type.setString( editor_type[this->object_type] );
			mouse_clock.restart();
		}

		//switches to displaying kinematic objects
		else if(sf::Keyboard::isKeyPressed( sf::Keyboard::Num3 ) || sf::Keyboard::isKeyPressed( sf::Keyboard::Numpad3 ) )
		{
			this->current_index = 0; //reset index
			this->object_type = BODY_TYPE::KINEMATIC;
			object_type.setString( editor_type[this->object_type] );
			mouse_clock.restart();
		}

		
		//saves the current map
		else if( sf::Keyboard::isKeyPressed( sf::Keyboard::LControl ) && sf::Keyboard::isKeyPressed( sf::Keyboard::S ) )
		{			
			string file;
			cout << "Save level as: ";
			cin >> file;
			this->saveFile(view, player, file);
			cout << "map saved :D" << endl;
			mouse_clock.restart();
		}

		//write something to generate the boundaries to remove this function
		//to view the whole map at once. Used for setting up the boundaries. 
		else if(sf::Keyboard::isKeyPressed( sf::Keyboard::LControl ) && sf::Keyboard::isKeyPressed( sf::Keyboard::C ) )
		{
			//set map view size to the level size
			view.getView()->setSize( view.getLevelSize() );
			view.getView()->setCenter( sf::Vector2f( view.getLevelSize().x / 2.0, view.getLevelSize().y / 2.0 ) );
			mouse_clock.restart();
		}

		else if(sf::Keyboard::isKeyPressed( sf::Keyboard::LControl ) && sf::Keyboard::isKeyPressed( sf::Keyboard::R ) )
		{
			//reset the map view to the window size and the center is set to the player position
			view.getView()->setSize( sf::Vector2f(window.getSize().x, window.getSize().y) );
			view.getView()->setCenter( sf::Vector2f(window.getSize().x / 2.0, window.getSize().y / 2.0) );
			mouse_clock.restart();
		}

		//used to rotate sprite clockwise
		else if(sf::Keyboard::isKeyPressed(sf::Keyboard::D) )
		{
			if(sf::Keyboard::isKeyPressed( sf::Keyboard::LShift ) )
				angle += 0.5;

			else
				angle += 2.0;
		}

		//used to rotate sprite counterclockwise
		else if(sf::Keyboard::isKeyPressed(sf::Keyboard::A) )
		{
			if(sf::Keyboard::isKeyPressed( sf::Keyboard::LShift ) )
				angle -= 0.5;

			else
				angle -= 2.0;
		}
		
	}
}

void Editor::keyboardCycleCommands(Timer &editor_clock) //used to cycle through objects in editor mode
{
	if(editor_clock.getElapsedTime() >= 0.3)
	{
		if(sf::Keyboard::isKeyPressed( sf::Keyboard::Q ) ) //cycle backwards in sprite vector
		{
			int index = this->current_index - 1;
			this->angle = 0.0; //reset

			if(this->object_type == BODY_TYPE::STATIC)
			{
				index = (index < 0) ? (this->static_sprite.size() - 1) : index;
			}

			else if(this->object_type == BODY_TYPE::DYNAMIC)
			{
				index = (index < 0) ? (this->dynamic_sprite.size() - 1) : index;
			}

			else if(this->object_type == BODY_TYPE::KINEMATIC)
			{
				index = (index < 0) ? (this->kinematic_sprite.size() - 1) : index;
			}

			this->current_index = index;
			editor_clock.restart();
		}

		else if(sf::Keyboard::isKeyPressed( sf::Keyboard::E ) ) //cycle forwards in sprite vector
		{
			int index = this->current_index + 1;
			this->angle = 0.0; //reset

			if(this->object_type == BODY_TYPE::STATIC)
			{
				index = (index >= this->static_sprite.size()) ? 0 : index;
			}

			else if(this->object_type == BODY_TYPE::DYNAMIC)
			{
				index = (index >= this->dynamic_sprite.size()) ? 0 : index;
			}

			else if(this->object_type == BODY_TYPE::KINEMATIC)
			{
				index = (index >= this->kinematic_sprite.size()) ? 0 : index; 
			}

			this->current_index = index;
			editor_clock.restart();
		}
	}
}

void Editor::gameModeToggle(sf::Text &mode_text, Timer &clock, string &live, string &edit, int &game_state)
{
	if(clock.getElapsedTime() >= 1.0) //long enough to toggle between game modes
	{
		if(sf::Keyboard::isKeyPressed( sf::Keyboard::Tilde ) )
		{
			clock.restart();
			this->current_index = 0; //reset index 
			this->angle = 0.0; //reset angle
			game_state = (game_state == GAME_STATE::LIVE) ? GAME_STATE::EDITOR : GAME_STATE::LIVE; //assigns the opposite mode
			mode_text.setString( (game_state == GAME_STATE::LIVE) ? live : edit ); //assigns corresponding string
		}
	}
}

void Editor::deleteObject(b2World *world, sf::Vector2f &mouse_pos)
{
	sf::FloatRect bounds;

	//deletes any static objects intersecting the cursor
	for(int i = 0; i < this->static_object.size(); i++)
	{
		bounds = this->static_object[i]->getSprite()->getGlobalBounds();
		
		if(Physics::boundsContainsPoint(bounds, mouse_pos) == true) //collision occurred
		{
			world->DestroyBody( this->static_object[i]->getBody() ); //deletes box2d body
			this->static_object.erase( this->static_object.begin() + i ); //deletes entire object in vector
		}
	}

	//deletes any dynamic objects intersecting the cursor
	for(int i = 0; i < this->dynamic_object.size(); i++)
	{
		bounds = this->dynamic_object[i]->getSprite()->getGlobalBounds();

		if(Physics::boundsContainsPoint(bounds, mouse_pos) == true) //collision occurred
		{
			world->DestroyBody( this->dynamic_object[i]->getBody() ); //deletes box2d body
			this->dynamic_object.erase( this->dynamic_object.begin() + i ); //deletes entire object in vector
		}
	}

	//deletes any kinematic objects intersecting the cursor
	for(int i = 0; i < this->kinematic_object.size(); i++)
	{
		bounds = this->kinematic_object[i]->getSprite()->getGlobalBounds();

		if(Physics::boundsContainsPoint(bounds, mouse_pos) == true) //collision occurred
		{
			world->DestroyBody( this->kinematic_object[i]->getBody() ); //deletes box2d body
			this->kinematic_object.erase( this->kinematic_object.begin() + i ); //deletes the entire object in vector
		}
	}
}

int Editor::getObjectType() //returns the current object type
{
	return( this->object_type );
}

int Editor::getCurrentIndex() //returns the index of the current object
{
	return( this->current_index );
}

int Editor::getBackgroundIndex()
{
	return( this->current_background_index );
}

sf::Vector2f& Editor::getSpawnPoint()
{
	return( this->spawn_point );
}

float Editor::getAngle()
{
	return( this->angle );
}

sf::Sprite* Editor::getCurrentBackground()
{
	return( &background_sprites[this->current_background_index] );
}

vector<sf::Texture>& Editor::getBackgroundTextures()
{
	return( this->background_textures );
}

vector<Object*> Editor::getStaticObjects()
{
	return( this->static_object );
}

vector<Object*> Editor::getDynamicObjects()
{
	return( this->dynamic_object );
}

vector<Object*> Editor::getKinematicObjects()
{
	return( this->kinematic_object );
}

vector<sf::Sprite>& Editor::getStaticSprites()
{
	return( (this->static_sprite) );
}

vector<sf::Sprite>& Editor::getDynamicSprites()
{
	return( (this->dynamic_sprite) );
}

vector<sf::Sprite>& Editor::getKinematicSprites()
{
	return( (this->kinematic_sprite) );
}

vector<sf::Texture>& Editor::getStaticTextures() //returns all the textures for the static sprites loaded
{
	return( this->static_texture );
}

vector<sf::Texture>& Editor::getDynamicTextures() //returns all the textures for the dynamic sprites loaded
{
	return( this->dynamic_texture );
}

vector<sf::Texture>& Editor::getKinematicTextures() //returns all the textures for the kinematic sprites loaded
{
	return( this->kinematic_texture );
}


void Editor::createStaticBody(sf::RenderWindow &window, b2World *world, sf::Vector2f &mouse_pos)
{
	Object *temp_object = NULL;
	b2FixtureDef fixture;
	
	//creates the static object at the mouse cursor
	if(this->current_index == STATIC::SMALL_PLATFORM) 
	{
		fixture.density = 1;
		fixture.restitution = 0.05;
		fixture.friction = 0.75;

		temp_object = new Object(window, world, fixture, this->static_texture[this->current_index], this->current_index, BODY_TYPE::STATIC, POLY_SHAPE);
		//set angle here
	}

	else if(this->current_index == STATIC::MED_PLATFORM1)
	{
		fixture.density = 1;
		fixture.restitution = 0.05;
		fixture.friction = 0.75;

		temp_object = new Object(window, world, fixture, this->static_texture[this->current_index], this->current_index, BODY_TYPE::STATIC, POLY_SHAPE);
	}

	else if(this->current_index == STATIC::MED_PLATFORM2)
	{
		fixture.density = 1;
		fixture.restitution = 0.05;
		fixture.friction = 0.75;

		temp_object = new Object(window, world, fixture, this->static_texture[this->current_index], this->current_index, BODY_TYPE::STATIC, POLY_SHAPE);
	}

	else if(this->current_index == STATIC::MED_PLATFORM3)
	{
		fixture.density = 1;
		fixture.restitution = 0.05;
		fixture.friction = 0.75;

		temp_object = new Object(window, world, fixture, this->static_texture[this->current_index], this->current_index, BODY_TYPE::STATIC, POLY_SHAPE);
	}

	else if(this->current_index == STATIC::MED_PLATFORM4)
	{
		fixture.density = 1;
		fixture.restitution = 0.05;
		fixture.friction = 0.75;

		temp_object = new Object(window, world, fixture, this->static_texture[this->current_index], this->current_index, BODY_TYPE::STATIC, POLY_SHAPE);
	}

	else if(this->current_index == STATIC::MED_PLATFORM5)
	{
		fixture.density = 1;
		fixture.restitution = 0.05;
		fixture.friction = 0.75;

		temp_object = new Object(window, world, fixture, this->static_texture[this->current_index], this->current_index, BODY_TYPE::STATIC, POLY_SHAPE);
	}

	else if(this->current_index == STATIC::MED_PLATFORM6)
	{
		fixture.density = 1;
		fixture.restitution = 0.05;
		fixture.friction = 0.75;

		temp_object = new Object(window, world, fixture, this->static_texture[this->current_index], this->current_index, BODY_TYPE::STATIC, POLY_SHAPE);
	}

	else if(this->current_index == STATIC::MED_PLATFORM7)
	{
		fixture.density = 1;
		fixture.restitution = 0.05;
		fixture.friction = 0.75;

		temp_object = new Object(window, world, fixture, this->static_texture[this->current_index], this->current_index, BODY_TYPE::STATIC, POLY_SHAPE);
	}

	else if(this->current_index == STATIC::LARGE_PLATFORM) 
	{
		fixture.density = 1;
		fixture.restitution = 0.05;
		fixture.friction = 0.75;

		temp_object = new Object(window, world, fixture, this->static_texture[this->current_index], this->current_index, BODY_TYPE::STATIC, POLY_SHAPE);
	}

	else if(this->current_index == STATIC::H_BOUNDARY)
	{
		fixture.density = 1;
		fixture.restitution = 0.05;
		fixture.friction = 0.0;

		temp_object = new Object(window, world, fixture, this->static_texture[this->current_index], this->current_index, BODY_TYPE::STATIC, POLY_SHAPE);
	}

	else if(this->current_index == STATIC::V_BOUNDARY)
	{
		fixture.density = 1;
		fixture.restitution = 0.05;
		fixture.friction = 0.0;

		temp_object = new Object(window, world, fixture, this->static_texture[this->current_index], this->current_index, BODY_TYPE::STATIC, POLY_SHAPE);
	}

	else if(this->current_index == STATIC::BOUNCE_PLATFORM)
	{
		fixture.density = 1;
		fixture.restitution = 0.75;
		fixture.friction = 0.1;

		temp_object = new Object(window, world, fixture, this->static_texture[this->current_index], this->current_index, BODY_TYPE::STATIC, POLY_SHAPE);
	}

	temp_object->getBody()->SetTransform( b2Vec2( mouse_pos.x * PIXELS_TO_METERS, -mouse_pos.y * PIXELS_TO_METERS ), -this->angle * DEGTORAD );
	temp_object->getSprite()->setRotation(this->angle);
	temp_object->updateSpritePos();
	this->static_object.push_back(temp_object);
	
}

void Editor::createDynamicBody(sf::RenderWindow &window, b2World *world, sf::Vector2f &mouse_pos)
{
	Object *temp_object = NULL;
	b2FixtureDef fixture;

	if(this->current_index == DYNAMIC::CRATE)
	{
		fixture.density = 1;
		fixture.friction = 0.5;
		fixture.restitution = 0.25;

		temp_object = new Object(window, world, fixture, this->dynamic_texture[this->current_index], this->current_index, BODY_TYPE::DYNAMIC, POLY_SHAPE);
	}

	else if(this->current_index == DYNAMIC::CRATE2)
	{
		fixture.density = 1;
		fixture.friction = 0.5;
		fixture.restitution = 0.25;
		
		temp_object = new Object(window, world, fixture, this->dynamic_texture[this->current_index], this->current_index, BODY_TYPE::DYNAMIC, POLY_SHAPE);
	}

	temp_object->getBody()->SetTransform( b2Vec2( mouse_pos.x * PIXELS_TO_METERS, -mouse_pos.y * PIXELS_TO_METERS ), -this->angle * DEGTORAD );
	temp_object->getSprite()->setRotation(this->angle);
	temp_object->updateSpritePos();
	this->dynamic_object.push_back(temp_object);
}

void Editor::createKinematicBody(sf::RenderWindow &window, b2World *world, sf::Vector2f &mouse_pos)
{
	Object *temp_object = NULL;
	b2FixtureDef fixture;

	if(this->current_index == KINEMATIC::MOVING_PLATFORM) 
	{
		fixture.friction = 1.0;
		fixture.density = 1;
		fixture.restitution = 0.05;

		temp_object = new Object(window, world, fixture, this->kinematic_texture[this->current_index], this->current_index, BODY_TYPE::KINEMATIC, POLY_SHAPE);
		temp_object->getBody()->SetLinearVelocity( b2Vec2(200.0 * PIXELS_TO_METERS, 0.0) );
	}

	else if(this->current_index == KINEMATIC::SPIKES)
	{
		fixture.friction = 1.0;
		fixture.density = 1;
		fixture.restitution = 0.05;

		temp_object = new Object(window, world, fixture, this->kinematic_texture[this->current_index], this->current_index, BODY_TYPE::KINEMATIC, POLY_SHAPE);
		temp_object->getBody()->SetLinearVelocity( b2Vec2(500.0 * PIXELS_TO_METERS, 0.0) );
		temp_object->getBody()->SetAngularVelocity(450.0 * ( (rand() % 2 == 0) ? 1 : (-1) ) * DEGTORAD );
		
	}

	else if(this->current_index == KINEMATIC::SPHERE)
	{
		fixture.friction = 1.0;
		fixture.density = 1;
		fixture.restitution = 0.05;

		temp_object = new Object(window, world, fixture, this->kinematic_texture[this->current_index], this->current_index, BODY_TYPE::KINEMATIC, CIRCLE_SHAPE);
		temp_object->getBody()->SetAngularVelocity(-360.0 * DEGTORAD );
	}

	temp_object->getBody()->SetTransform( b2Vec2( mouse_pos.x * PIXELS_TO_METERS, -mouse_pos.y * PIXELS_TO_METERS ), -this->angle * DEGTORAD );
	temp_object->getSprite()->setRotation(this->angle);
	temp_object->updateSpritePos();
	this->kinematic_object.push_back(temp_object);
}

void Editor::saveFile(Camera &view, Object &player, string &file_name)
{
	fstream save_file(file_name, ios::out); //opens the save file for output
	Object *o;

	//saves the current camera settings
	save_file << view.getLevelSize().x << "," << view.getLevelSize().y << endl;

	//player current index is set to -1 to indicate it's the player
	save_file << player.getSprite()->getPosition().x << "," << player.getSprite()->getPosition().y << "," << player.getSprite()->getRotation() << "," << Editor::BODY_TYPE::DYNAMIC << "," << -1 << endl;

	for(int i = 0; i < this->getStaticObjects().size(); i++) //saves all static objects to file
	{
		o = this->getStaticObjects()[i];
		save_file << o->getSprite()->getPosition().x << "," << o->getSprite()->getPosition().y << "," << o->getSprite()->getRotation() << "," << Editor::BODY_TYPE::STATIC << "," << o->getCurrentIndex() << endl;
	}

	for(int i = 0; i < this->getDynamicObjects().size(); i++) //saves all static objects to file
	{
		o = this->getDynamicObjects()[i];
		save_file << o->getSprite()->getPosition().x << "," << o->getSprite()->getPosition().y << "," << o->getSprite()->getRotation() << "," <<  Editor::BODY_TYPE::DYNAMIC << "," << o->getCurrentIndex() << endl;
	}

	for(int i = 0; i < this->getKinematicObjects().size(); i++) //saves all static objects to file
	{
		o = this->getKinematicObjects()[i];
		save_file << o->getSprite()->getPosition().x << "," << o->getSprite()->getPosition().y << "," << o->getSprite()->getRotation() << "," << Editor::BODY_TYPE::KINEMATIC << "," << o->getCurrentIndex() << endl;
	}

	save_file.close();

}

void Editor::loadFile(sf::RenderWindow &window, b2World *world, Camera &view, Object &player, string &file_name)
{
	fstream load_file(file_name, ios::in);

	string line;
	sf::Vector2f pos; //position of object
	sf::Vector2f level_size; //size of the current level
	float angle; //angle of object
	int body_type; //type of object(static/dynamic/kinematic)
	int index; //index of object
	char end_line;

	load_file >> level_size.x >> end_line >> level_size.y;
	view.setLevelSize(level_size);

	while( !load_file.eof() ) //while the eof isn't reached
	{
		load_file >> pos.x >> end_line >> pos.y >> end_line >> this->angle >> end_line >> body_type >> end_line >> index;

		if(index != -1)
		{
			this->current_index = index; //used to determine which object to create

			switch(body_type) //creates the body based on the type loaded in
				{
				case BODY_TYPE::STATIC:
					this->createStaticBody(window, world, pos);
					break;
				case BODY_TYPE::DYNAMIC:
					this->createDynamicBody(window, world, pos);
					break;
				case BODY_TYPE::KINEMATIC:
					this->createKinematicBody(window, world, pos);
					break;
				}
		}

		else //the player
		{
			this->spawn_point = pos; //starting point of the character for the current level
			player.getBody()->SetTransform( b2Vec2( pos.x * PIXELS_TO_METERS, -pos.y * PIXELS_TO_METERS), this->angle * DEGTORAD );
			player.updateSpritePos();
		}
	}

	load_file.close();
}