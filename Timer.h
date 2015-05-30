#ifndef TIMER_H
#define TIMER_H

#include <SFML\Graphics.hpp>

class Timer
{
	//fix pause and resume functions
private:
	sf::Clock clock; //current clock
	bool paused; //determines if the clock is paused
	float elapsed_time; //clock elapsed time

public:
	Timer();

	void restart(); //restarts the current clock
	void pause(); //pauses the current clock
	void resume(); //unpauses the current clock

	float update(); //updates the clock. Must be called every iteration
	float getElapsedTime(); //returns the current elapsed time
};

#endif