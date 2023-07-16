#include "Chip8.h"
#include <stdlib.h>
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <SFML/Window/Event.hpp>
#include <stdio.h>
#include <iostream>
#include <Windows.h>
#include <stdint.h>
#include <fstream>

constexpr int MEMORY_ROM_OFFSET = 512;

bool Chip::load(const char* file_path)
{
	int i;
	FILE* f = fopen(file_path, "rb");
	if (f == NULL) {
		printf("Failed to open ROM file: %s\n", file_path);
		return false;
	}
	else {
		printf("Game successfully loaded: %s\n", file_path);
	}

	fseek(f, 0L, SEEK_END);
	int filesize = ftell(f);
	rewind(f);

	std::vector<unsigned char> buffer(filesize);
	fread(buffer.data(), 1, filesize, f);

	for (int i = 0; i < filesize; i++) {
		memory[i + MEMORY_ROM_OFFSET] = buffer[i];
	}

	fclose(f);

	return true;
}


void Chip::emulatecycle()
{
	// fetch opcode
	opcode = (memory[pc] << 8) | memory[pc + 1];

	// increment pc
	pc += 2;

	// decode opcode
	switch (opcode & 0xf000)
	{
		case(0x0000):
		{
			switch(opcode & 0x00ff)
			{
				case 0x00e0:
					OP_00E0();
					break;
				case 0x00ee:
					OP_00EE();
					break;
				default:
					uint8_t n = opcode & 0x000F;
					printf("\nunknown 1 op code: %u \n", opcode);
					exit(3);
			}
			break;
		}

		case(0x1000):
			OP_1NNN();
			break;
		case(0x2000):
			OP_2NNN();
			break;
		case(0x3000):
			OP_3XKK();
			break;
		case(0x4000):
			OP_4XKK();
			break;
		case(0x5000):
			OP_5XY0();
			break;
		case(0x6000):
			OP_6XKK();
			break;
		case(0x7000):
			OP_7XKK();
			break;
		case (0x8000):
		{
			switch(opcode & 0x000f)
			{
				case(0x0000):
					OP_8XY0();
					break;
				case(0x0001):
					OP_8XY1();
					break;
				case(0x0002):
					OP_8XY2();
					break;
				case(0x0003):
					OP_8XY3();
					break;
				case(0x0004):
					OP_8XY4();
					break;
				case(0x0005):
					OP_8XY5();
					break;
				case(0x0006):
					OP_8XY6();
					break;
				case(0x0007):
					OP_8XY7();
					break;
				case(0x000e):
					OP_8XYE();
					break;
				default:
					printf("\n unknown 2 op code %u \n", opcode);
					exit(3);
			}
			break;
		}
		case(0x9000):
			OP_9XY0();
			break;
		case(0xa000):
			OP_ANNN();
			break;
		case(0xb000):
			OP_BNNN();
			break;
		case(0xc000):
			OP_CXKK();
			break;
		case(0xd000):
			OP_DXYN();
			break;
		case(0xe000):
		{
			switch(opcode & 0x00ff)
			{
				case(0x009e):
					OP_EX9E();
					break;
				case(0x00a1):
					OP_EXA1();
					break;
				default:
					printf("\n unknown 3 op code: %u\n", opcode);
					exit(3);
					break;
			}
			break;
		}
		case(0xf000):
		{
			switch(opcode & 0x00ff)
			{
				case(0x0007):
					OP_FX07();
					break;
				case(0x000a):
					OP_FX0A();
					break;
				case(0x0015):
					OP_FX15();
					break;
				case(0x0018):
					OP_FX18();
					break;
				case(0x001e):
					OP_FX1E();
					break;
				case(0x0029):
					OP_FX29();
					break;
				case(0x0033):
					OP_FX33();
					break;
				case(0x0055):
					OP_FX55();
					break;
				case(0x0065):
					OP_FX65();
					break;
				default:
					printf("\nerror unknown opcode in chip8.cpp %u\n", opcode);
					exit(3);
			}
			break;
		}
	}

	if (delayTimer > 0)
	{
		--delayTimer;
	}

	// decrement the sound timer if it's been set
	if (soundTimer > 0)
	{
		if (soundTimer == 1)
			printf("beef!\n");
		--soundTimer;
	}
}

void updatekeystate(Chip& chip, sf::Event event)
{
	const sf::Keyboard::Key key = event.key.code;
	bool state = event.type == sf::Event::KeyPressed;

	// Key mapping using std::map
	static std::map<sf::Keyboard::Key, uint8_t> keyMap = {
		{sf::Keyboard::Num1, 0x1},
		{sf::Keyboard::Num2, 0x2},
		{sf::Keyboard::Num3, 0x3},
		{sf::Keyboard::Num4, 0xC},
		{sf::Keyboard::Q, 0x4},
		{sf::Keyboard::W, 0x5},
		{sf::Keyboard::E, 0x6},
		{sf::Keyboard::R, 0xD},
		{sf::Keyboard::A, 0x7},
		{sf::Keyboard::S, 0x8},
		{sf::Keyboard::D, 0x9},
		{sf::Keyboard::F, 0xE},
		{sf::Keyboard::Z, 0xA},
		{sf::Keyboard::X, 0x0},
		{sf::Keyboard::C, 0xB},
		{sf::Keyboard::V, 0xF}
	};

	// Check if the key exists in the keyMap
	if (keyMap.find(key) != keyMap.end())
	{
		uint8_t chipKey = keyMap[key];
		chip.keys[chipKey] = state;
	}
}


void drawdisplay(Chip& chip, sf::RenderWindow& window)
{
	sf::RectangleShape rectangle(sf::Vector2f(pixelSize, pixelSize));
	rectangle.setFillColor(sf::Color::White);

	int x, y;
	for(y = 0; y < videoheight; y++)
	{
		for(x = 0; x < videowidth; x++)
		{
			uint8_t gfx = chip.graphics[x+(videowidth*y)];
			
            if(gfx > 0)
			{
                rectangle.setPosition(static_cast<float>(x*pixelSize), static_cast<float>(y* pixelSize));
				window.draw(rectangle);
            }
		}
	}
}

int main()
{
	Chip chip8;
	bool flag = chip8.load("test_opcode.ch8");

	if(!flag)
	{
		printf("game could not open\n");
		return EXIT_FAILURE;
	}

	// loads window
	sf::RenderWindow window(sf::VideoMode(1280, 720), "chip 8");

	// limits framerate
	window.setFramerateLimit(60);

	sf::Clock clock;
	
	// loops while window is open
	while(window.isOpen())
	{
		sf::Event event;
		while(window.pollEvent(event))
		{

			// If the window is closed or escape is pressed, close the window
			if (event.type == sf::Event::Closed || (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Escape)) {
				window.close();
			}

			if (event.type == sf::Event::KeyPressed)
			{
				updatekeystate(chip8, event);
			}

			else if (event.type == sf::Event::KeyReleased)
			{
				updatekeystate(chip8, event);
			}
		}

		chip8.emulatecycle();

		if(chip8.drawFlag)
		{
			window.clear();
			drawdisplay(chip8, window);
			window.display();
			chip8.drawFlag = false;	
		}
	}
	window.close();
	return EXIT_SUCCESS;
}

