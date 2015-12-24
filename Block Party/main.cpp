#include <SFML/Graphics.hpp>
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <vector>
#include <cmath>   

const int ANIMATION_COUNT = 6;
const int BLOCK_COUNT = 6;
const int COLOR_COUNT = 7;
const int CHANGE_COLOR_COUNT = 12;

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
	float MAX_DEVIATION = 80.f;
	float ANGLE_SPEED = 0.1f;
	float MAX_ANGLE = 90.f;
	float FOUR_MAX_TIME = 1.f;
	float DIVIDER = 1000.f;
	float FIRST_MAX_TIME = 2.f;
	float ANGLE_CHANGE_COLOR = 30.f;
} PARAMS;


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
	float angle_speed;

	Block() {}

	void setStart(sf::Vector2f position)
	{
		params.color = sf::Color::Blue;
		params.size = { BLOCK.SIZE, BLOCK.SIZE };
		params.position = position;
		params.origin = { 0.f, 0.f };
		params.rotation = 0.f;
		setRectangle(block, params);
	}

	void updatePosition()
	{
		params.position += speed;
		block.setPosition(params.position);
	}

	void updateRotation()
	{
		params.rotation += angle_speed;
		block.setRotation(params.rotation);
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
	void(*func)(std::vector<Block> &blocks))
{
	if (is_not_set)
	{
		(*func)(blocks);
		is_not_set = false;
	}
}
void setStep(bool &is_not_set, std::vector<Block> &blocks,
	void(*set)(std::vector<Block> &blocks), int &repeats,
	void(*update)(std::vector<Block> &blocks, int &repeats))
{
	setAnimation(is_not_set, blocks, set);
	update(blocks, repeats);
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
		blocks[i].params.rotation = 0.f;
		blocks[i].block.setRotation(0.f);
	}
}

void setNull(std::vector<Block> &blocks) {}
void setFirst(std::vector<Block> &blocks)
{
	for (int i = 0; i < blocks.size(); i++)
		if (i % 2 == 0)
			blocks[i].speed = { 0.f, PARAMS.SPEED };
		else
			blocks[i].speed = { 0.f, -PARAMS.SPEED };
}
void updateFirst(std::vector<Block> &blocks, int &repeats)
{
	for (int i = 0; i < BLOCK_COUNT; i++)
	{
		if (abs(blocks[i].params.position.y - blocks[i].start.y) >= PARAMS.MAX_DEVIATION)
		{
			blocks[i].speed.y = -blocks[i].speed.y;
			if (i == blocks.size() - 1)
				repeats -= 1;
		}
		blocks[i].updatePosition();
	}
}

void goRectangle(sf::Vector2f left_up, sf::Vector2f size, sf::Vector2f position, sf::Vector2f &move)
{
	if (position.x <= left_up.x && position.y <= left_up.y)
		move = { PARAMS.SPEED, 0.f };
	else if (position.x >= left_up.x + size.x && position.y <= left_up.y)
		move = { 0.f, PARAMS.SPEED };
	else if (position.x >= left_up.x + size.x && position.y >= left_up.y + size.y)
		move = { -PARAMS.SPEED, 0.f };
	else if (position.x <= left_up.x && position.y >= left_up.y + size.y)
		move = { 0.f, -PARAMS.SPEED };
}
void updateSecond(std::vector<Block> &blocks, int &repeats)
{
	for (int i = 0; i < blocks.size(); i++)
	{
		if (i % 2 == 0)
			goRectangle(sf::Vector2f(blocks[i].start.x, blocks[i].start.y - PARAMS.MAX_DEVIATION),
				sf::Vector2f(BLOCK.SIZE + BLOCK.DISTANCE, 2 * PARAMS.MAX_DEVIATION), blocks[i].params.position,
				blocks[i].speed);
		else
			goRectangle(sf::Vector2f(blocks[i - 1].start.x, blocks[i].start.y - PARAMS.MAX_DEVIATION),
				sf::Vector2f(BLOCK.SIZE + BLOCK.DISTANCE, 2 * PARAMS.MAX_DEVIATION), blocks[i].params.position,
				blocks[i].speed);
		blocks[i].updatePosition();
	}
	if (blocks[0].params.position.x <= blocks[0].start.x &&
		blocks[0].params.position.y <= blocks[0].start.y - PARAMS.MAX_DEVIATION)
		repeats -= 1;
}

void setThird(std::vector<Block> &blocks)
{
	for (int i = 0; i < blocks.size(); i++)
	{
		float dist = (i * 2 % 3) * (BLOCK.DISTANCE + BLOCK.SIZE);
		if (i % 2)
			dist = ((i + 1) % (blocks.size() / 2)) * (BLOCK.DISTANCE + BLOCK.SIZE);;
		float temp = sqrt(2 * dist * dist) / PARAMS.DIVIDER;
		blocks[i].speed = { -temp, temp };
		if (i % 2)
			blocks[i].speed = { temp, -temp };
	}
}
void updateThird(std::vector<Block> &blocks, int &repeats)
{
	for (int i = 0; i < blocks.size(); i++)
		blocks[i].updatePosition();
	if (blocks[2].params.position.x <= blocks[0].start.x)
		repeats--;
}

void updateFourth(std::vector<Block> &blocks, int &repeats)
{	
	sf::Color colors[COLOR_COUNT]{ sf::Color::Black, sf::Color::Blue, sf::Color::Cyan,
		sf::Color::Green, sf::Color::Magenta, sf::Color::Red, sf::Color::Yellow };
	for (int i = 0; i < blocks.size(); i++)
		if (blocks[i].params.rotation < 360.f)
		{
			if (blocks[i].params.rotation >= PARAMS.ANGLE_CHANGE_COLOR * abs(repeats - CHANGE_COLOR_COUNT))
			{
				blocks[i].block.setFillColor(colors[rand() % COLOR_COUNT]);
				if (i == blocks.size() - 1)
					repeats++;
			}
			blocks[i].angle_speed = PARAMS.ANGLE_SPEED;
			blocks[i].updateRotation();
		}
		else
			repeats -= 2;
}

bool isAround(sf::Vector2f one, sf::Vector2f two)
{
	float deviation = 1.f;
	return (one.x <= two.x + deviation && one.x >= two.x - deviation &&
		one.y <= two.y + deviation && one.y >= two.y - deviation);
}
void setFifth(std::vector<Block> &blocks)
{
	for (int i = 0; i < blocks.size(); i++)
		blocks[i].speed = { (-blocks[i].params.position.x + blocks[i].start.x) / PARAMS.DIVIDER,
		(-blocks[i].params.position.y + blocks[i].start.y) / PARAMS.DIVIDER };
}
void updateFifth(std::vector<Block> &blocks, int &repeats)
{
	for (int i = 0; i < blocks.size(); i++)
		if (!isAround(blocks[i].params.position, blocks[i].start))
			blocks[i].updatePosition();
		else if (i == 4)
			repeats--;
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
	int repeats[ANIMATION_COUNT] = { 0, 4, 3, 1, 12, 1 };

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

		time = clock.getElapsedTime().asSeconds();

		if (time <= PARAMS.FIRST_MAX_TIME)
			setAnimation(is_not_set[0], blocks, start);
		else if (repeats[1] > 0)
			setStep(is_not_set[1], blocks, setFirst, repeats[1], updateFirst);
		else if (repeats[2] > 0)
			setStep(is_not_set[2], blocks, setNull, repeats[2], updateSecond);
		else if (repeats[3] > 0)
			setStep(is_not_set[3], blocks, setThird, repeats[3], updateThird);
		else if (repeats[4] > 0)
			setStep(is_not_set[4], blocks, setNull, repeats[4], updateFourth);
		else if (repeats[5] > 0)
			setStep(is_not_set[5], blocks, setFifth, repeats[5], updateFifth);
		else
		{
			clock.restart();
			resetBoolList(is_not_set);
			repeats[1] = 4;
			repeats[2] = 3;
			repeats[3] = 1;
			repeats[4] = 12;
			repeats[5] = 1;
		}

		window.clear(sf::Color::White);
		drawBlocks(window, blocks);
		window.display();
	}
	return 0;
}