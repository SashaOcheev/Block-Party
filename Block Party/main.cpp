#include <SFML/Graphics.hpp>
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <vector>

const struct
{
	sf::Vector2f SIZE = { 800, 600 };
	int ANTIALIASING_LEVEL = 8;
	std::string TITLE = "Bloc Party";
} WINDOW;

const struct
{
	float SIZE = 30.f;
	float DISTANCE = 10.f;
} BLOCK;

struct
{
	sf::Vector2f FIRST = {0.f, 0.5f};
} SPEED;

const int BLOCK_COUNT = 6;
const float FIRST_MAX_DEVIATION = 100.f;

struct
{
	float START = 5.f;
	float FIRST = FIRST_MAX_DEVIATION / SPEED.FIRST.y * 10;
} STEP_TIME;


//        left block        ////////////////////////////////////////
struct Left
{
	sf::Vector2f start;
	sf::Vector2f speed;
	sf::Vector2f position;
	float angle;
	bool is_not_set[BLOCK_COUNT];
	sf::RectangleShape block;

	Left()
	{

	}

	void nullStat()
	{
		for (int i = 0; i < BLOCK_COUNT; i++)
			is_not_set[i] = true;
	}

	void setStart()
	{
		start = { WINDOW.SIZE.x / 2.f - BLOCK.DISTANCE * 2.5f - BLOCK.SIZE * 3, WINDOW.SIZE.y / 2.f - BLOCK.SIZE * 0.5f };
		block.setFillColor(sf::Color::Blue);
		block.setSize(sf::Vector2f(BLOCK.SIZE, BLOCK.SIZE));
		is_not_set[0] = false;
	}
	
	void setFirst()
	{
		speed = SPEED.FIRST;
		position = start;
		block.setPosition(start);
		is_not_set[1] = false;
	}
};
////////////////////////////////////////////////////////////////////
void start (Left &left, sf::RenderWindow &window)
{
	if (left.is_not_set[0])
		left.setStart();

	sf::Vector2f other_positions = left.start;
	for (int i = 1; i <= 6; i++)
	{
		left.block.setPosition(other_positions);
		window.draw(left.block);
		other_positions.x += BLOCK.DISTANCE + BLOCK.SIZE;
	}
}

void first(Left &left, sf::RenderWindow &window)
{
	if (left.is_not_set[1])
		left.setFirst();
	if (abs(left.position.y - left.start.y) >= FIRST_MAX_DEVIATION)
		left.speed.y = -left.speed.y;
	left.position += left.speed;
	for (int i = 0; i < BLOCK_COUNT; i++)
	{
		left.position.x = left.start.x + i * (BLOCK.SIZE + BLOCK.DISTANCE);
		if (i % 2 == 0)
			left.block.setPosition(left.position);
		else
		{
			float temp_position_y = left.start.y - left.position.y + left.start.y;
			left.block.setPosition(left.position.x, temp_position_y);
		}
		window.draw(left.block);
	}
}


int main()
{	
	Left left;
	left.nullStat();
	sf::Clock clock;
	float time;
	float one_step_time;

	sf::ContextSettings settings;
	settings.antialiasingLevel = WINDOW.ANTIALIASING_LEVEL;
	sf::RenderWindow window(sf::VideoMode(int(WINDOW.SIZE.x), int(WINDOW.SIZE.y)), WINDOW.TITLE, sf::Style::Default, settings);

	while (window.isOpen())
	{
		sf::Event event;
		while (window.pollEvent(event))
			if (event.type == sf::Event::Closed)
				window.close();

		window.clear(sf::Color::White);
		time = clock.getElapsedTime().asSeconds();
		
		if (time <= STEP_TIME.START)
			start(left, window);
		else if (time <= STEP_TIME.FIRST)
			first(left, window);
		else
		{
			left.nullStat();
			clock.restart();
		}
		std::cout << time << std::endl;
		window.display();
	}
	return 0;
}