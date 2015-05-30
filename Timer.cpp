#include "Timer.h"
#include <SFML\Graphics.hpp>

Timer::Timer()
{
	this->elapsed_time = 0.0;
	this->clock = sf::Clock();
	this->paused = false;	
}

float Timer::update() //must be used every iteration to update the clock
{
	if(this->paused == false) //only updates if the clock is not paused
	{
		this->elapsed_time += this->clock.getElapsedTime().asSeconds();
		this->clock.restart();
	}

	return( this->elapsed_time );
}

void Timer::restart() //restarts the clock and updates the elapsed time
{
	this->clock.restart();
	this->elapsed_time = 0.0;
}

void Timer::pause() //pauses the clock
{
	this->paused = true; 
}

void Timer::resume() //unpauses the clock
{
	this->clock.restart();
	this->paused = false; 
}

float Timer::getElapsedTime() //returns the clock elapsed time
{
	return this->elapsed_time;
}