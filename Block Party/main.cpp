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

const struct
{
	float SPEED = 0.2f;
	float MAX_DEVIATION = 100.f;
} FIRST;


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
	sf::Vector2f start;
	Rectangle_params params;
	sf::RectangleShape block;
	sf::Vector2f speed;

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

void start(std::vector<Block> &blocks)
{
	sf::Vector2f start = { (WINDOW.SIZE.x - BLOCK.DISTANCE * (blocks.size() - 1) - BLOCK.SIZE * blocks.size()) / 2.f,
		(WINDOW.SIZE.y - BLOCK.SIZE) / 2.f };
	for (int i = 0; i < blocks.size(); i++)
	{
		sf::Vector2f temp_start = start;
		temp_start.x = start.x + i * (BLOCK.SIZE + BLOCK.DISTANCE);
		blocks[i].setStart(temp_start);
		blocks[i].start = temp_start;
	}
}

void setFirst(std::vector<Block> &blocks)
{
	for (int i = 0; i < blocks.size(); i++)
		if (i % 2 == 0)
			blocks[i].speed = { 0.f, FIRST.SPEED };
		else
			blocks[i].speed = { 0.f, -FIRST.SPEED };
}
void updateFirst(std::vector<Block> &blocks, int &repeats)
{
	for (int i = 0; i < BLOCK_COUNT; i++)
	{
		if (abs(blocks[i].params.position.y - blocks[i].start.y) >= FIRST.MAX_DEVIATION)
		{
			blocks[i].speed.y = -blocks[i].speed.y;
			if (i == blocks.size() - 1)
				repeats -= 1;
		}
		blocks[i].params.position += blocks[i].speed;
		blocks[i].block.setPosition(blocks[i].params.position);
	}
}


void goRectangle(sf::Vector2f left_up, sf::Vector2f size, sf::Vector2f position, sf::Vector2f &move)
{
	if (position.x <= left_up.x && position.y <= left_up.y)
		move = { FIRST.SPEED, 0.f };
	else if (position.x >= left_up.x + size.x && position.y <= left_up.y)
		move = { 0.f, FIRST.SPEED };
	else if (position.x >= left_up.x + size.x && position.y >= left_up.y + size.y)
		move = { -FIRST.SPEED, 0.f };
	else if (position.x <= left_up.x && position.y >= left_up.y + size.y)
		move = { 0.f, -FIRST.SPEED };
}
void updateSecond(std::vector<Block> &blocks, int &repeats)
{
	for (int i = 0; i < blocks.size(); i++)
	{
		if (i % 2 == 0)
			goRectangle(sf::Vector2f(blocks[i].start.x, blocks[i].start.y - FIRST.MAX_DEVIATION),
				sf::Vector2f(BLOCK.SIZE + BLOCK.DISTANCE, 2 * FIRST.MAX_DEVIATION), blocks[i].params.position,
				blocks[i].speed);
		else
			goRectangle(sf::Vector2f(blocks[i - 1].start.x, blocks[i].start.y - FIRST.MAX_DEVIATION),
				sf::Vector2f(BLOCK.SIZE + BLOCK.DISTANCE, 2 * FIRST.MAX_DEVIATION), blocks[i].params.position,
				blocks[i].speed);
		blocks[i].params.position += blocks[i].speed;
		blocks[i].block.setPosition(blocks[i].params.position);
		if (i == blocks.size() - 1 && blocks[i].params.position.x >= blocks[i].start.x &&
			blocks[i].params.position.y >= blocks[i].start.y + FIRST.MAX_DEVIATION)
			repeats -= 1;

	}
}




int main()
{	
	std::vector<Block> blocks;
	for (int i = 0; i < BLOCK_COUNT; i++)
	{
		Block block;
		blocks.push_back(block);
	}
	bool is_not_set[ANIMATION_COUNT];
	resetBoolList(is_not_set);
	int repeats[ANIMATION_COUNT] = { 6, 3, 6 };

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
		
		if (time <= 3.f)
		{
			setAnimation(is_not_set[0], blocks, start);
			drawBlocks(window, blocks);
		}
		else if (repeats[0] > 0)
		{
			setAnimation(is_not_set[1], blocks, setFirst);
			updateFirst(blocks, repeats[0]);
			drawBlocks(window, blocks);
		}
		else if (repeats[1] > 0)
		{
			updateSecond(blocks, repeats[1]);
			drawBlocks(window, blocks);
		}
		else
		{
			clock.restart();
			resetBoolList(is_not_set);
			repeats[0] = 6;
			repeats[1] = 3;
			repeats[2] = 6;
		}
		
		window.display();
	}
	return 0;
}