#ifndef CAMERA_H
#define CAMERA_H

#include <SFML\Graphics.hpp>
#include <iostream>
using namespace std;

class Camera
{
private:
	sf::View view;
	sf::Vector2f level_size; //total size of the level

public:
	Camera(sf::Vector2f &center_pos, sf::Vector2f &view_size, sf::Vector2f &level_size); //used to set the center and size of the view
	sf::View* getView(); //returns the pointer to the view.
	sf::Vector2f& getLevelSize(); //returns the size of the level

	void setLevelSize(sf::Vector2f &size); //sets the current level size

	void boundaryControl(const sf::Vector2f &pos); //makes sure the camera stays with the player and doesn't go off screen
	void cursorBoundaryControl(const sf::Vector2i &pos); //defines how the mouse cursor moves the camera when in editor mode
	void zoomControl(sf::RenderWindow &window, sf::Event &e); //controls how much you can zoom in/out so it doesn't go off screen or too close. 
};

#endif