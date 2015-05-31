#include "Camera.h"

Camera::Camera(sf::Vector2f &center_pos, sf::Vector2f &view_size, sf::Vector2f &level_size)
{
	this->view.setCenter( center_pos ); //centers the view at this position
	this->view.setSize( view_size ); //how much the view shows
	this->level_size = level_size; //size of the level. Used in boundaryControl()
}

sf::View* Camera::getView()
{
	return( &(this->view) );
} 

sf::Vector2f& Camera::getLevelSize()
{
	return( this->level_size );
}

void Camera::setLevelSize(sf::Vector2f &size)
{
	this->level_size = size;
}

void Camera::boundaryControl(const sf::Vector2f &pos) //makes sure the camera stays with the player and doesn't go off screen
{
	//check after a specified amount of time

	sf::Vector2f current_center = this->view.getCenter(); //gets the current center(world coordinates) for comparison
	sf::Vector2f view_size = this->view.getSize();
	view_size.x /= 2.0;
	view_size.y /= 2.0;
	bool modified = false;

	this->view.setCenter( pos );	
}

void Camera::cursorBoundaryControl(const sf::Vector2i &pos)
{
	sf::Vector2f current_center = this->view.getCenter(); //gets the current center for comparison
	sf::Vector2f view_size = this->view.getSize();
	view_size.x -= 15.0;
	view_size.y -= 15.0;
	bool modified = false;

	if(pos.x > view_size.x && pos.x < (this->level_size.x - view_size.x) ) //if the view on the x axis should be adjusted
	{
		current_center.x = pos.x; //modifies the current center to be at the position
		modified = true;
	}

	if(pos.y > view_size.y && pos.y < (this->level_size.y - view_size.y) ) //if the view on the y axis should be adjusted
	{
		current_center.y = pos.y; //modifies the current center to be at the position
		modified = true;
	}
	
	if(modified) //only updates view if it has been modified
		this->view.setCenter(current_center);
}

void Camera::zoomControl(sf::RenderWindow &window, sf::Event &e)
{
	if( e.mouseWheel.delta > 0 ) //forward mouse wheel to zoom in
	{
		if( this->view.getSize().x * 0.95 > (window.getSize().x / 1.5) )
			this->view.zoom(0.95f);
	}

	else if( e.mouseWheel.delta < 0 )
	{
		//if( this->view.getSize().x * 1.05 < (window.getSize().x * 1.1) )
			this->view.zoom(1.05f);
	}	
}