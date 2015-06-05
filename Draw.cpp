#include "Draw.h"

enum {STATIC, DYNAMIC, KINEMATIC}; //determines what type of box2d body to create

void Draw::draw(sf::RenderWindow &window, Object &object) //draws object to the screen
{
	window.draw( *object.getSprite() );
}

void Draw::draw(sf::RenderWindow &window, vector<Object*> &body) //draws all objects in vector to the screen
{
	for(int i = 0; i < body.size(); i++)
	{
		window.draw( *body[i]->getSprite() );
	}
}

void Draw::drawBackgroundGrid(sf::RenderWindow &window, Camera &view, sf::Sprite &background_sprite, sf::Texture &background_texture)
{
	sf::Vector2f reset_pos;
	reset_pos.x = view.getView()->getCenter().x - (view.getView()->getSize().x / 2.0); //reset position for the x-axis when each row is finished
	reset_pos.y = view.getView()->getCenter().y - (view.getView()->getSize().y / 2.0); //just used to start the first y-axis row

	sf::Vector2f max_position;
	max_position.x = view.getView()->getCenter().x + (view.getView()->getSize().x / 2.0);
	max_position.y = view.getView()->getCenter().y + (view.getView()->getSize().y / 2.0);
	 
	sf::Vector2f source_position(reset_pos.x, reset_pos.y); //the current starting position to add a new sprite

	for(int i = 0; source_position.y < max_position.y; i++) //while there is at least one more row
	{
		for(int j = 0; source_position.x < max_position.x; j++) //while there is at least one more column
		{

			background_sprite.setPosition(source_position);
			window.draw( background_sprite );

			source_position.x += background_texture.getSize().x; //increases the source to the next x-axis starting position
		}

		source_position.x = reset_pos.x; //reset
		source_position.y += background_texture.getSize().y; //increases the source to the next y-axis starting position
	}
}

void Draw::drawEditor(sf::RenderWindow &window, Editor &editor, sf::Vector2f &mouse_pos, sf::Text &editor_object_type, sf::Vector2i &editor_object_pos, int &game_state)
{
	if(editor.getObjectType() == STATIC) //if you're looking at static objects
	{
		if(editor.getStaticSprites().size() > 0) //if there are static sprites
		{
			sf::Sprite& temp = editor.getStaticSprites()[editor.getCurrentIndex()];
			temp.setTexture( editor.getStaticTextures()[editor.getCurrentIndex()] );
			temp.setPosition(mouse_pos.x, mouse_pos.y);
			temp.setRotation(editor.getAngle());
			temp.setColor( sf::Color(255, 255, 255, 170) );
			window.draw(editor.getStaticSprites()[editor.getCurrentIndex()]);
		}
	}

	else if(editor.getObjectType() == DYNAMIC) //if you're looking at dynamic objects
	{
		if(editor.getDynamicSprites().size() > 0) //if there are dynamic sprites
		{ 
			
			sf::Sprite& temp = editor.getDynamicSprites()[editor.getCurrentIndex()];
			temp.setTexture( editor.getDynamicTextures()[editor.getCurrentIndex()] );
			temp.setPosition(mouse_pos.x, mouse_pos.y);
			temp.setRotation(editor.getAngle());
			temp.setColor( sf::Color(255, 255, 255, 170) );
			window.draw(temp);			
		}
	}

	else if(editor.getObjectType() == KINEMATIC) //if you're looking at kinematic objects
	{
		if(editor.getKinematicSprites().size() > 0) //if there are kinematic sprites
		{
			sf::Sprite& temp = editor.getKinematicSprites()[editor.getCurrentIndex()];
			temp.setTexture( editor.getKinematicTextures()[editor.getCurrentIndex()] );
			temp.setPosition(mouse_pos.x, mouse_pos.y);
			temp.setRotation(editor.getAngle());
			temp.setColor( sf::Color(255, 255, 255, 170) );
			window.draw(temp);
		}
	}

	drawText(window, editor_object_type, editor_object_pos);
}

void Draw::drawText(sf::RenderWindow &window, sf::Text &text, sf::Vector2i &text_pos) //draws the game mode text to the screen
{
	//update text position here
	text.setPosition( window.mapPixelToCoords(text_pos) );
	window.draw(text);
}

void Draw::drawParticles(sf::RenderWindow &window, b2World *world, Particle &particle, int type)
{
	b2ParticleSystem *particle_system = particle.getParticleSystems()[type]; //&world->GetParticleSystemList()[type]; 
	int transparency = 0;
	for(int i = 0; i < particle_system->GetParticleCount(); i++) //loops through all the particles
	{
		b2Vec2 pos = particle_system->GetPositionBuffer()[i]; //gets the position of the current particle

		if( &pos != NULL ) //if the particle exists
		{
			//particle groups should fade out proportional to their lifetime
			particle.getShape(type).setPosition( pos.x, pos.y );
			window.draw( particle.getShape(type) );
		}
	}
}