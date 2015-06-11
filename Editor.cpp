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

	this->levels.push_back("level1.txt");
	this->current_level = levels[FILE::LEVEL1]; //start level
	this->current_level_index = FILE::LEVEL1;

	this->levels.push_back("level2.txt");
	this->levels.push_back("level3.txt");

	this->levels.push_back("level4.txt");
	this->max_level = FILE::LEVEL4; //highest level the player can reach

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

	sf::Texture glow_platform_texture;
	sf::Sprite glow_platform_sprite;

	sf::Texture vertical_boundary_texture;
	sf::Sprite vertical_boundary_sprite;

	sf::Texture bounce_platform_texture;
	sf::Sprite bounce_platform_sprite;

	sf::Texture level_boundary_texture;
	sf::Sprite level_boundary_sprite;

	if(!small_platform_texture.loadFromFile("images//orb.png") )
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

	if(!med_platform_texture3.loadFromFile("images//small_level_boundary.png") )
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

	if(!med_platform_texture5.loadFromFile("images//frame.png") )
	{
		printf("Failed to load texture on line %d \n", __LINE__);
	}
	
	this->static_texture.push_back( med_platform_texture5 );
	med_platform_sprite5.setTexture( this->static_texture.back() );
	med_platform_sprite5.setOrigin( med_platform_texture5.getSize().x / 2.0, med_platform_texture5.getSize().y / 2.0 );
	this->static_sprite.push_back( med_platform_sprite5 );

	if(!med_platform_texture6.loadFromFile("images//portal_door.png") )
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

	if(!glow_platform_texture.loadFromFile("images//glow_platform.png") )
	{
		printf("Failed to load texture on line %d \n", __LINE__);
	}

	this->static_texture.push_back( glow_platform_texture );
	glow_platform_sprite.setTexture( this->static_texture.back() );
	glow_platform_sprite.setOrigin( glow_platform_texture.getSize().x / 2.0, glow_platform_texture.getSize().y / 2.0 );
	this->static_sprite.push_back( glow_platform_sprite );

	if(!vertical_boundary_texture.loadFromFile("images//vertical_boundary.png") )
	{
		printf("Failed to load texture on line %d \n", __LINE__);
	}

	this->static_texture.push_back( vertical_boundary_texture );
	vertical_boundary_sprite.setTexture( this->static_texture.back() );
	vertical_boundary_sprite.setOrigin( vertical_boundary_texture.getSize().x / 2.0, vertical_boundary_texture.getSize().y / 2.0 );
	this->static_sprite.push_back( vertical_boundary_sprite );

	if(!bounce_platform_texture.loadFromFile("images//bounce_platform_invert.png") )
	{
		printf("Failed to load texture on line %d \n", __LINE__);
	}

	this->static_texture.push_back( bounce_platform_texture );
	bounce_platform_sprite.setTexture( this->static_texture.back() );
	bounce_platform_sprite.setOrigin( bounce_platform_texture.getSize().x / 2.0, bounce_platform_texture.getSize().y / 2.0 );
	this->static_sprite.push_back( bounce_platform_sprite );

	if(!level_boundary_texture.loadFromFile("images//level_boundary.png") )
	{
		printf("Failed to load texture on line %d \n", __LINE__);
	}

	this->static_texture.push_back( level_boundary_texture );
	level_boundary_sprite.setTexture( this->static_texture.back() );
	level_boundary_sprite.setOrigin( level_boundary_texture.getSize().x / 2.0, level_boundary_texture.getSize().y / 2.0 );
	this->static_sprite.push_back( level_boundary_sprite );
	

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

	sf::Texture portal_texture;
	sf::Sprite portal_sprite;

	sf::Texture saw_texture;
	sf::Sprite saw_sprite;

	if(!moving_texture.loadFromFile("images//moving_platform.png") )
	{
		printf("Failed to load texture on line %d \n", __LINE__);
	}

	this->kinematic_texture.push_back( moving_texture );

	moving_sprite.setTexture( this->kinematic_texture.back() );
	moving_sprite.setOrigin( moving_texture.getSize().x / 2.0, moving_texture.getSize().y / 2.0 );
	this->kinematic_sprite.push_back( moving_sprite );

	if(!spike_texture.loadFromFile("images//obstacle1.png") )
	{
		printf("Failed to load texture on line %d \n", __LINE__);
	}

	this->kinematic_texture.push_back( spike_texture );

	spike_sprite.setTexture( this->kinematic_texture.back() );
	spike_sprite.setOrigin( spike_texture.getSize().x / 2.0, spike_texture.getSize().y / 2.0 );
	this->kinematic_sprite.push_back( spike_sprite );

	if(!sphere_texture.loadFromFile("images//cube.png") )
	{
		printf("Failed to load texture on line %d \n", __LINE__);
	}

	this->kinematic_texture.push_back( sphere_texture );

	sphere_sprite.setTexture( this->kinematic_texture.back() );
	sphere_sprite.setOrigin( sphere_texture.getSize().x / 2.0, sphere_texture.getSize().y / 2.0 );
	this->kinematic_sprite.push_back( sphere_sprite );

	if(!portal_texture.loadFromFile("images//blackhole3.png") )
	{
		printf("Failed to load texture on line %d \n", __LINE__);
	}

	this->kinematic_texture.push_back( portal_texture );

	portal_sprite.setTexture( this->kinematic_texture.back() );
	portal_sprite.setOrigin( portal_texture.getSize().x / 2.0, portal_texture.getSize().y / 2.0 );
	this->kinematic_sprite.push_back( portal_sprite );

	if(!saw_texture.loadFromFile("images//cube_boundary.png") )
	{
		printf("Failed to load texture on line %d \n", __LINE__);
	}

	this->kinematic_texture.push_back( saw_texture );

	saw_sprite.setTexture( this->kinematic_texture.back() );
	saw_sprite.setOrigin( saw_texture.getSize().x / 2.0, saw_texture.getSize().y / 2.0 );
	this->kinematic_sprite.push_back( saw_sprite );
	
}

void Editor::nextLevel() //sets variables for next level
{
	this->current_level_index++;
	this->current_level = this->levels[this->current_level_index];
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

void Editor::addLevelBoundaries(sf::RenderWindow &window, Camera &view, b2World *world)
{
	Object *temp_object;
	b2FixtureDef fixture;
	sf::Vector2f source_pos(0.0, 0.0);
	sf::Vector2f size( this->static_texture[this->current_index].getSize() );
	this->current_index = STATIC::LEVEL_BOUNDARY; //sets it up so the boundaries are loaded

	fixture.density = 1.0;
	fixture.restitution = 0.0;
	fixture.friction = 0.0; 
	fixture.filter.categoryBits = BOUNDARY;
	fixture.filter.maskBits = FRIENDLY | DYNAMIC_OBJECT | WEAPON;	
	
	//loop through all horizontal boundaries
	for(source_pos.x = 0.0; source_pos.x < view.getLevelSize().x; source_pos.x += size.x)
	{
		temp_object = new Object(window, world, fixture, this->static_texture[this->current_index], this->current_index, BODY_TYPE::STATIC, POLY_SHAPE);
		
		temp_object->getBody()->SetTransform( b2Vec2( source_pos.x * PIXELS_TO_METERS, 0.0 * PIXELS_TO_METERS ), 0.0 * DEGTORAD );
		temp_object->getSprite()->setRotation(0.0);
		temp_object->updateSpritePos();
		this->static_object.push_back(temp_object);


		temp_object = new Object(window, world, fixture, this->static_texture[this->current_index], this->current_index, BODY_TYPE::STATIC, POLY_SHAPE);

		temp_object->getBody()->SetTransform( b2Vec2( source_pos.x * PIXELS_TO_METERS, -view.getLevelSize().y * PIXELS_TO_METERS ), 0.0 * DEGTORAD );
		temp_object->getSprite()->setRotation(0.0);
		temp_object->updateSpritePos();
		this->static_object.push_back(temp_object);


	}	

	
	//loop through all vertical boundaries
	for(source_pos.y = 0.0; source_pos.y < view.getLevelSize().y; source_pos.y += size.y)
	{
		temp_object = new Object(window, world, fixture, this->static_texture[this->current_index], this->current_index, BODY_TYPE::STATIC, POLY_SHAPE);
		temp_object->getBody()->SetTransform( b2Vec2( 0.0 * PIXELS_TO_METERS, -source_pos.y * PIXELS_TO_METERS ), 90.0 * DEGTORAD );
		temp_object->getSprite()->setRotation(90.0);
		temp_object->updateSpritePos();
		this->static_object.push_back(temp_object);

		temp_object = new Object(window, world, fixture, this->static_texture[this->current_index], this->current_index, BODY_TYPE::STATIC, POLY_SHAPE);
		temp_object->getBody()->SetTransform( b2Vec2( view.getLevelSize().x * PIXELS_TO_METERS, -source_pos.y * PIXELS_TO_METERS ), 90.0 * DEGTORAD );
		temp_object->getSprite()->setRotation(90.0);
		temp_object->updateSpritePos();
		this->static_object.push_back(temp_object);
	}
	
}

//determines how to place objects to the screen in editor mode
void Editor::keyboardActionCommands(sf::RenderWindow &window, Camera &view, b2World *world, Object &player, Timer &mouse_clock, sf::Text &object_type, sf::Vector2f &mouse_pos, string *editor_type, int &game_state)
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

			cout << "static body: " << this->getStaticObjects().size() << endl;
			cout << "dynamic body: " << this->getDynamicObjects().size() << endl;
			cout << "kinematic body: " << this->getKinematicObjects().size() << endl << endl;

			mouse_clock.restart();
		}

		else if( sf::Keyboard::isKeyPressed( sf::Keyboard::LControl ) && sf::Keyboard::isKeyPressed( sf::Keyboard::L ) )
		{
			string filename; 
			
			cout << "Load level as: ";
			cin >> filename;			

			if(this->fileExists(filename) == true) //if the file exists
			{
				this->deleteAllObjects(world); //erases the current map
				this->loadFile(window, world, view, player, filename); //only changes the map if the file loads
				game_state = GAME_STATE::LIVE; //starts the level
				window.setActive();
			}

			else
			{
				cout << "Unable to load file." << endl;
			}


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

bool Editor::levelBoundaries(Camera &view, Object &player)
{
	bool modified = false;
	//if the player goes out of bounds, they return to the spawn point
	if(player.getSprite()->getPosition().x < 0 || player.getSprite()->getPosition().x > view.getLevelSize().x)
	{
		player.getBody()->SetTransform( b2Vec2(this->spawn_point.x * PIXELS_TO_METERS, -this->spawn_point.y* PIXELS_TO_METERS), player.getBody()->GetAngle() );
		modified = true;
	}

	else if(player.getSprite()->getPosition().y < 0 || player.getSprite()->getPosition().y > view.getLevelSize().y)
	{
		player.getBody()->SetTransform( b2Vec2(this->spawn_point.x * PIXELS_TO_METERS, -this->spawn_point.y * PIXELS_TO_METERS), player.getBody()->GetAngle() );
		modified = true;
	}

	return modified;
} 

void Editor::deleteObject(b2World *world, sf::Vector2f &mouse_pos)
{
	sf::FloatRect bounds;

	//deletes any static objects intersecting the cursor
	for(int i = 0; i < this->static_object.size(); i)
	{
		bounds = this->static_object[i]->getSprite()->getGlobalBounds();
		
		if(Physics::boundsContainsPoint(bounds, mouse_pos) == true) //collision occurred
		{
			world->DestroyBody( this->static_object[i]->getBody() ); //deletes box2d body
			this->static_object.erase( this->static_object.begin() + i ); //deletes entire object in vector
		}

		else
		{
			i++;
		}
	}

	//deletes any dynamic objects intersecting the cursor
	for(int i = 0; i < this->dynamic_object.size(); i)
	{
		bounds = this->dynamic_object[i]->getSprite()->getGlobalBounds();

		if(Physics::boundsContainsPoint(bounds, mouse_pos) == true) //collision occurred
		{
			world->DestroyBody( this->dynamic_object[i]->getBody() ); //deletes box2d body
			this->dynamic_object.erase( this->dynamic_object.begin() + i ); //deletes entire object in vector
		}

		else
		{
			i++;
		}
	}

	//deletes any kinematic objects intersecting the cursor
	for(int i = 0; i < this->kinematic_object.size(); i)
	{
		bounds = this->kinematic_object[i]->getSprite()->getGlobalBounds();

		if(Physics::boundsContainsPoint(bounds, mouse_pos) == true) //collision occurred
		{
			world->DestroyBody( this->kinematic_object[i]->getBody() ); //deletes box2d body
			this->kinematic_object.erase( this->kinematic_object.begin() + i ); //deletes the entire object in vector
		}

		else
		{
			i++;
		}
	}
}

void Editor::deleteAllObjects(b2World *world)
{
	while(0 < this->static_object.size() ) //deletes all static objects
	{
		world->DestroyBody( this->static_object[0]->getBody() );
		this->static_object.erase( this->static_object.begin() );
	}

	while(0 < this->dynamic_object.size() ) //deletes all dynamic objects
	{
		world->DestroyBody( this->dynamic_object[0]->getBody() );
		this->dynamic_object.erase( this->dynamic_object.begin() );
	}

	while(0 < this->kinematic_object.size() ) //deletes all kinematic objects
	{
		world->DestroyBody( this->kinematic_object[0]->getBody() );
		this->kinematic_object.erase( this->kinematic_object.begin() );
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

	fixture.filter.categoryBits = PLATFORM; //default
	fixture.filter.maskBits = FRIENDLY | WEAPON | DYNAMIC_OBJECT;
	
	//creates the static object at the mouse cursor
	if(this->current_index == STATIC::ORB) 
	{
		fixture.density = 1;
		fixture.restitution = 0.05;
		fixture.friction = 0.75;
		fixture.filter.categoryBits = WEAPON;
		fixture.filter.maskBits = FRIENDLY;

		temp_object = new Object(window, world, fixture, this->static_texture[this->current_index], this->current_index, BODY_TYPE::STATIC, CIRCLE_SHAPE);
		//set angle here
	}

	else if(this->current_index == STATIC::MED_PLATFORM1)
	{
		fixture.density = 1;
		fixture.restitution = 0.05;
		fixture.friction = 0.75;
		fixture.filter.categoryBits = PLATFORM; 
		fixture.filter.maskBits = FRIENDLY | WEAPON | DYNAMIC_OBJECT;

		temp_object = new Object(window, world, fixture, this->static_texture[this->current_index], this->current_index, BODY_TYPE::STATIC, POLY_SHAPE);
	}

	else if(this->current_index == STATIC::MED_PLATFORM2)
	{
		fixture.density = 1;
		fixture.restitution = 0.05;
		fixture.friction = 0.75;
		fixture.filter.categoryBits = PLATFORM; 
		fixture.filter.maskBits = FRIENDLY | WEAPON | DYNAMIC_OBJECT;

		temp_object = new Object(window, world, fixture, this->static_texture[this->current_index], this->current_index, BODY_TYPE::STATIC, POLY_SHAPE);
	}

	else if(this->current_index == STATIC::MED_PLATFORM3)
	{
		fixture.density = 1;
		fixture.restitution = 0.05;
		fixture.friction = 0.0;
		fixture.filter.categoryBits = BOUNDARY;
		fixture.filter.maskBits = DYNAMIC_OBJECT | WEAPON | FRIENDLY;

		temp_object = new Object(window, world, fixture, this->static_texture[this->current_index], this->current_index, BODY_TYPE::STATIC, POLY_SHAPE);
	}

	else if(this->current_index == STATIC::MED_PLATFORM4)
	{
		fixture.density = 1;
		fixture.restitution = 0.05;
		fixture.friction = 0.75;
		fixture.filter.categoryBits = PLATFORM; 
		fixture.filter.maskBits = FRIENDLY | WEAPON | DYNAMIC_OBJECT;

		temp_object = new Object(window, world, fixture, this->static_texture[this->current_index], this->current_index, BODY_TYPE::STATIC, POLY_SHAPE);
	}

	else if(this->current_index == STATIC::MED_PLATFORM5)
	{
		fixture.density = 1;
		fixture.restitution = 0.05;
		fixture.friction = 0.75;
		fixture.filter.categoryBits = PLATFORM; 
		fixture.filter.maskBits = FRIENDLY | WEAPON | DYNAMIC_OBJECT;

		temp_object = new Object(window, world, fixture, this->static_texture[this->current_index], this->current_index, BODY_TYPE::STATIC, POLY_SHAPE);
	}

	else if(this->current_index == STATIC::MED_PLATFORM6)
	{
		fixture.density = 1;
		fixture.restitution = 0.05;
		fixture.friction = 0.75;
		fixture.filter.categoryBits = PLATFORM; 
		fixture.filter.maskBits = FRIENDLY | WEAPON | DYNAMIC_OBJECT;

		temp_object = new Object(window, world, fixture, this->static_texture[this->current_index], this->current_index, BODY_TYPE::STATIC, POLY_SHAPE);
	}

	else if(this->current_index == STATIC::MED_PLATFORM7)
	{
		fixture.density = 1;
		fixture.restitution = 0.05;
		fixture.friction = 0.75;
		fixture.filter.categoryBits = PLATFORM; 
		fixture.filter.maskBits = FRIENDLY | WEAPON | DYNAMIC_OBJECT;

		temp_object = new Object(window, world, fixture, this->static_texture[this->current_index], this->current_index, BODY_TYPE::STATIC, POLY_SHAPE);
	}

	else if(this->current_index == STATIC::LARGE_PLATFORM) 
	{
		fixture.density = 1;
		fixture.restitution = 0.05;
		fixture.friction = 0.75;
		fixture.filter.categoryBits = PLATFORM; 
		fixture.filter.maskBits = FRIENDLY | WEAPON | DYNAMIC_OBJECT;

		temp_object = new Object(window, world, fixture, this->static_texture[this->current_index], this->current_index, BODY_TYPE::STATIC, POLY_SHAPE);
	}

	else if(this->current_index == STATIC::GLOW_PLATFORM)
	{
		fixture.density = 1;
		fixture.restitution = 0.05;
		fixture.friction = 0.75;
		fixture.filter.categoryBits = PLATFORM; 
		fixture.filter.maskBits = FRIENDLY | WEAPON | DYNAMIC_OBJECT;

		temp_object = new Object(window, world, fixture, this->static_texture[this->current_index], this->current_index, BODY_TYPE::STATIC, POLY_SHAPE);
	}

	else if(this->current_index == STATIC::V_BOUNDARY)
	{
		fixture.density = 1;
		fixture.restitution = 0.05;
		fixture.friction = 0.0;
		fixture.filter.categoryBits = BOUNDARY;
		fixture.filter.maskBits = DYNAMIC_OBJECT | WEAPON | FRIENDLY;

		temp_object = new Object(window, world, fixture, this->static_texture[this->current_index], this->current_index, BODY_TYPE::STATIC, POLY_SHAPE);
	}

	else if(this->current_index == STATIC::BOUNCE_PLATFORM)
	{
		fixture.density = 1;
		fixture.restitution = 0.0;
		fixture.friction = 0.75;
		fixture.filter.categoryBits = BOUNCE; 
		fixture.filter.maskBits = FRIENDLY | WEAPON | DYNAMIC_OBJECT;
		
		temp_object = new Object(window, world, fixture, this->static_texture[this->current_index], this->current_index, BODY_TYPE::STATIC, POLY_SHAPE);
	}

	else if(this->current_index == STATIC::LEVEL_BOUNDARY)
	{
		fixture.density = 1;
		fixture.restitution = 0.0;
		fixture.friction = 0.0;
		fixture.filter.categoryBits = BOUNDARY; 
		fixture.filter.maskBits = FRIENDLY | WEAPON | DYNAMIC_OBJECT;
		
		temp_object = new Object(window, world, fixture, this->static_texture[this->current_index], this->current_index, BODY_TYPE::STATIC, POLY_SHAPE);
	}

	temp_object->getBody()->SetTransform( b2Vec2( mouse_pos.x * PIXELS_TO_METERS, -mouse_pos.y * PIXELS_TO_METERS ), -this->angle * DEGTORAD );
	temp_object->getSprite()->setRotation(this->angle);
	temp_object->updateSpritePos();
	temp_object->getBody()->SetUserData(temp_object); //sets the user data

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
		fixture.filter.categoryBits = DYNAMIC_OBJECT; 
		fixture.filter.maskBits = FRIENDLY | WEAPON | DYNAMIC_OBJECT | BOUNDARY | BOUNCE | PLATFORM;

		temp_object = new Object(window, world, fixture, this->dynamic_texture[this->current_index], this->current_index, BODY_TYPE::DYNAMIC, POLY_SHAPE);
	}

	else if(this->current_index == DYNAMIC::CRATE2)
	{
		fixture.density = 1;
		fixture.friction = 0.5;
		fixture.restitution = 0.25;
		fixture.filter.categoryBits = DYNAMIC_OBJECT; 
		fixture.filter.maskBits = FRIENDLY | WEAPON | DYNAMIC_OBJECT | BOUNDARY | BOUNCE | PLATFORM;
		
		temp_object = new Object(window, world, fixture, this->dynamic_texture[this->current_index], this->current_index, BODY_TYPE::DYNAMIC, POLY_SHAPE);
	}

	temp_object->getBody()->SetTransform( b2Vec2( mouse_pos.x * PIXELS_TO_METERS, -mouse_pos.y * PIXELS_TO_METERS ), -this->angle * DEGTORAD );
	temp_object->getSprite()->setRotation(this->angle);
	temp_object->updateSpritePos();
	temp_object->getBody()->SetUserData(temp_object); //sets the user data

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
		fixture.filter.categoryBits = PLATFORM; 

		temp_object = new Object(window, world, fixture, this->kinematic_texture[this->current_index], this->current_index, BODY_TYPE::KINEMATIC, POLY_SHAPE);
		temp_object->getBody()->SetLinearVelocity( b2Vec2(200.0 * PIXELS_TO_METERS, 0.0) );
		//temp_object->getBody()->SetAngularVelocity( 90.0 * DEGTORAD );
	}

	else if(this->current_index == KINEMATIC::SPIKES)
	{
		fixture.friction = 1.0;
		fixture.density = 1;
		fixture.restitution = 0.05;
		fixture.filter.categoryBits = WEAPON; 
		fixture.filter.maskBits = FRIENDLY | WEAPON | DYNAMIC_OBJECT | BOUNDARY | BOUNCE | PLATFORM;

		temp_object = new Object(window, world, fixture, this->kinematic_texture[this->current_index], this->current_index, BODY_TYPE::KINEMATIC, POLY_SHAPE);
		temp_object->getBody()->SetLinearVelocity( b2Vec2(500.0 * PIXELS_TO_METERS, 0.0) );
		temp_object->getBody()->SetAngularVelocity(450.0 * ( (rand() % 2 == 0) ? 1 : (-1) ) * DEGTORAD );
		
	}

	else if(this->current_index == KINEMATIC::SPHERE)
	{
		fixture.friction = 1.0;
		fixture.density = 1;
		fixture.restitution = 0.05;
		fixture.filter.categoryBits = ITEM; 
		fixture.filter.maskBits = FRIENDLY;

		temp_object = new Object(window, world, fixture, this->kinematic_texture[this->current_index], this->current_index, BODY_TYPE::KINEMATIC, CIRCLE_SHAPE);
		temp_object->getBody()->SetAngularVelocity(-250.0 * DEGTORAD );
	}

	else if(this->current_index == KINEMATIC::PORTAL)
	{
		fixture.friction = 1.0;
		fixture.density = 1.0;
		fixture.restitution = 0.5;
		fixture.filter.categoryBits = TELEPORT;
		fixture.filter.maskBits = FRIENDLY | DYNAMIC_OBJECT;

		temp_object = new Object(window, world, fixture, this->kinematic_texture[this->current_index], this->current_index, BODY_TYPE::KINEMATIC, CIRCLE_SHAPE );
		temp_object->getBody()->SetAngularVelocity(-550.0 * DEGTORAD );
		temp_object->getSprite()->setColor( sf::Color(0, 200, 200, 30) );
	}

	else if(this->current_index == KINEMATIC::SAW_ITEM)
	{
		fixture.friction = 1.0;
		fixture.density = 1.0;
		fixture.restitution = 0.5;
		fixture.filter.categoryBits = WEAPON; 
		fixture.filter.maskBits = FRIENDLY | WEAPON | DYNAMIC_OBJECT | BOUNDARY | BOUNCE | PLATFORM;

		temp_object = new Object(window, world, fixture, this->kinematic_texture[this->current_index], this->current_index, BODY_TYPE::KINEMATIC, POLY_SHAPE );
		temp_object->getBody()->SetAngularVelocity(-550.0 * DEGTORAD );
	}

	temp_object->getBody()->SetTransform( b2Vec2( mouse_pos.x * PIXELS_TO_METERS, -mouse_pos.y * PIXELS_TO_METERS ), -this->angle * DEGTORAD );
	temp_object->getSprite()->setRotation(this->angle);
	temp_object->updateSpritePos();
	temp_object->getBody()->SetUserData(temp_object); //sets the user data

	this->kinematic_object.push_back(temp_object);

}

bool Editor::fileExists(string &filename)
{
	ifstream file_check(filename, ios::in);
	
	if(file_check.good())
	{
		file_check.close();
		return true;
	}

	file_check.close();
	return false;
}

string& Editor::getFileName()
{
	return( this->file_name );
}

void Editor::saveFile(Camera &view, Object &player, string &file_name)
{
	ofstream save_file(file_name, ios::trunc); //opens the save file for output
	vector<string> output;
	string temp_output = ""; //used to store the current string
	bool modified = false; //determines if any of the objects have been outputted to the file
	Object *o;

	//SOLUTION
	//SAVE EACH STRING TO A STRING VECTOR
	//THEN ONLY OUTPUT BETWEEN 0 - (i-1)

	//the last loop that runs is the one that's added to.
	//REMOVE THE ADDITIONAL LINE IN THE DEFAULT.TXT FILE AND IT WORKS
	//see if it's possible to delete the last line

	//saves the current camera settings
	save_file << view.getLevelSize().x << "," << view.getLevelSize().y << endl;
	

	//player current index is set to -1 to indicate it's the player
	save_file << Editor::BODY_TYPE::DYNAMIC << "," << -1 << "," << player.getSprite()->getPosition().x << "," << player.getSprite()->getPosition().y << "," << player.getSprite()->getRotation() << endl;

	for(int i = 0; i < this->getStaticObjects().size(); i++) //saves all static objects to file
	{
		o = this->static_object[i];

		if(i < this->static_object.size() - 1)
		{
			save_file << Editor::BODY_TYPE::STATIC << "," << o->getCurrentIndex() << "," << o->getSprite()->getPosition().x << "," << o->getSprite()->getPosition().y << "," << o->getSprite()->getRotation() << endl;
		}

		else
		{
			modified = true;
			save_file << Editor::BODY_TYPE::STATIC << "," << o->getCurrentIndex() << "," << o->getSprite()->getPosition().x << "," << o->getSprite()->getPosition().y << "," << o->getSprite()->getRotation();
		}
		//temp_output = Editor::BODY_TYPE::STATIC + "," + o->getCurrentIndex() + "," + o->getSprite()->getPosition().x + "," + o->getSprite()->getPosition().y + "," + o->getSprite()->getRotation() + '\n';
		//save_file << Editor::BODY_TYPE::STATIC << "," << o->getCurrentIndex() << "," << o->getSprite()->getPosition().x << "," << o->getSprite()->getPosition().y << "," << o->getSprite()->getRotation() << endl;
	}

	for(int i = 0; i < this->getDynamicObjects().size(); i++) //saves all static objects to file
	{
		o = this->dynamic_object[i];

		if(i == 0 && modified == true)
			save_file << endl;
		
		if(i < this->dynamic_object.size() - 1)
		{
			if(modified = true)
				save_file << endl;

			save_file << Editor::BODY_TYPE::DYNAMIC << "," << o->getCurrentIndex() << "," << o->getSprite()->getPosition().x << "," << o->getSprite()->getPosition().y << "," << o->getSprite()->getRotation() << endl;
		}

		else 
		{
			modified = true;
			save_file << Editor::BODY_TYPE::DYNAMIC << "," << o->getCurrentIndex() << "," << o->getSprite()->getPosition().x << "," << o->getSprite()->getPosition().y << "," << o->getSprite()->getRotation();
		}
		//save_file << Editor::BODY_TYPE::DYNAMIC << "," << o->getCurrentIndex() << "," << o->getSprite()->getPosition().x << "," << o->getSprite()->getPosition().y << "," << o->getSprite()->getRotation() << endl;
	}

	for(int i = 0; i < this->getKinematicObjects().size(); i++) //saves all static objects to file
	{
		o = this->kinematic_object[i];

		if(i == 0 && modified == true)
			save_file << endl;
		
		if(i < this->kinematic_object.size() - 1 )
		{
			save_file << Editor::BODY_TYPE::KINEMATIC << "," << o->getCurrentIndex() << "," << o->getSprite()->getPosition().x << "," << o->getSprite()->getPosition().y << "," << o->getSprite()->getRotation() << endl;
		}

		else
		{
			modified = true;
			save_file << Editor::BODY_TYPE::KINEMATIC << "," << o->getCurrentIndex() << "," << o->getSprite()->getPosition().x << "," << o->getSprite()->getPosition().y << "," << o->getSprite()->getRotation();
		}
		//save_file << Editor::BODY_TYPE::KINEMATIC << "," << o->getCurrentIndex() << "," << o->getSprite()->getPosition().x << "," << o->getSprite()->getPosition().y << "," << o->getSprite()->getRotation() << endl;
	}

	//save here
	

	save_file.clear();
	save_file.close();

}

bool Editor::loadFile(sf::RenderWindow &window, b2World *world, Camera &view, Object &player, string &file_name)
{
	//the last loop that runs is the one that's added to.

	ifstream load_file(file_name);

	string line;
	sf::Vector2f pos; //position of object
	sf::Vector2f level_size; //size of the current level
	float angle; //angle of object
	int body_type; //type of object(static/dynamic/kinematic)
	int index; //index of object
	char end_line;

	this->file_name = file_name; //successfully loaded file name

	load_file >> level_size.x >> end_line >> level_size.y;
	view.setLevelSize(level_size);

	while( !load_file.eof() ) //while the eof isn't reached
	{
		load_file >> body_type >> end_line >> index >> end_line >> pos.x >> end_line >> pos.y >> end_line >> this->angle;

		if(index != -1)
		{			
			if(body_type == BODY_TYPE::STATIC)
			{
				this->current_index = index; //used to determine which object to create
				this->createStaticBody(window, world, pos);
			}

			else if(body_type == BODY_TYPE::DYNAMIC)
			{
				this->current_index = index; //used to determine which object to create

				if(index != -1)
					this->createDynamicBody(window, world, pos);

				else
				{
					this->spawn_point = pos; //starting point of the character for the current level
					player.getBody()->SetTransform( b2Vec2( pos.x * PIXELS_TO_METERS, -pos.y * PIXELS_TO_METERS), this->angle * DEGTORAD );
					player.updateSpritePos();
				}
			}

			else if(body_type == BODY_TYPE::KINEMATIC)
			{
				this->current_index = index; //used to determine which object to create
				this->createKinematicBody(window, world, pos);
			} 
		}

		else //the player
		{
			this->spawn_point = pos; //starting point of the character for the current level
			player.getBody()->SetTransform( b2Vec2( pos.x * PIXELS_TO_METERS, -pos.y * PIXELS_TO_METERS), this->angle * DEGTORAD );
			player.updateSpritePos();
		}
		
	}

	load_file.clear();
	load_file.close();
	return true; //the file opened correctly
	
	
	return false; //the file didn't open	
}