#ifndef DRAW_H
#define DRAW_H

#include <SFML\Graphics.hpp>
#include <Box2D\Box2D.h>
#include <vector>
#include "Object.h"
#include "Editor.h"
using namespace std;

class Draw
{
private:

public:
//draws the current object to the screen
static void draw(sf::RenderWindow &window, Object &object);

//used to draw all the objects to the screen
static void draw(sf::RenderWindow &window, vector<Object*> &body);

//draws the objects to the screen when in editor mode
static void drawEditor(sf::RenderWindow &window, Editor &editor, sf::Vector2f &mouse_pos, sf::Text &editor_object_type, sf::Vector2i &editor_object_pos, int &game_state);

//draws the game mode text to the screen
static void drawText(sf::RenderWindow &window, sf::Text &text, sf::Vector2i &text_pos);

//draws the particles to the screen
static void drawParticles(sf::RenderWindow &window, sf::CircleShape &particle_shape, b2ParticleSystem *particle_system);

};

#endif