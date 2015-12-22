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


const int ANIMATION_COUNT = 3;
const int BLOCK_COUNT = 6;
const float TIME_DIVIDER = 800.f;


//        left block        ////////////////////////////////////////
struct Rectangle_params
{
	sf::Color color;
	sf::Vector2f size;
	sf::Vector2f position;
	sf::Vector2f origin;
	float rotation;
};

void setRectangle(sf::RectangleShape &rectangle, Rectangle_params params)
{
	rectangle.setFillColor(params.color);
	rectangle.setSize(params.size);
	rectangle.setPosition(params.position);
	rectangle.setOrigin(params.origin);
	rectangle.setRotation(params.rotation);
}

struct Block
{
	Rectangle_params params;
	sf::RectangleShape block;

	Block() {}
	
	void setStart(sf::Vector2f position)
	{
		params.color = sf::Color::Blue;
		params.size = {BLOCK.SIZE, BLOCK.SIZE};
		params.position = position;
		params.origin = { 0.f, 0.f };
		params.rotation = 0.f;
		setRectangle(block, params);
	}
};
////////////////////////////////////////////////////////////////////

void resetBoolList(bool is_not_set[ANIMATION_COUNT])
{
	for (int i = 0; i < ANIMATION_COUNT; i++)
		is_not_set[i] = true;
}

void setStart(std::vector<Block> &blocks)
{
	sf::Vector2f start = { (WINDOW.SIZE.x - BLOCK.DISTANCE * (BLOCK_COUNT - 1) - BLOCK.SIZE * BLOCK_COUNT) / 2.f,
		(WINDOW.SIZE.y - BLOCK.SIZE) / 2.f };
	for (int i = 0; i < BLOCK_COUNT; i++)
	{
		sf::Vector2f temp_start = start;
		Block block;
		temp_start.x = start.x + i * (BLOCK.SIZE + BLOCK.DISTANCE);
		block.setStart(temp_start);
		blocks.push_back(block);
	}
}

void drawBlocks(sf::RenderWindow &window, std::vector<Block> &blocks)
{
	for (int i = 0; i < blocks.size(); i++)
		window.draw(blocks[i].block);
}

void setAnimation(bool &is_not_set, std::vector<Block> &blocks,
				 void (*func)(std::vector<Block> &blocks))
{
	if (is_not_set)
	{
		(*func)(blocks);
		is_not_set = false;
	}
}

int main()
{	
	std::vector<Block> blocks;
	bool is_not_set[ANIMATION_COUNT];
	resetBoolList(is_not_set);

	sf::Clock clock;
	float time = 0.f;

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
		
		if (time <= 5.f)
		{
			setAnimation(is_not_set[0], blocks, setStart);
			drawBlocks(window, blocks);
		}
		else
		{
			clock.restart();
			resetBoolList(is_not_set);
		}
		
		std::cout << time << std::endl;
		window.display();
	}
	return 0;
}