// Chip 8.cpp : This file contains the 'main' function. Program execution begins and ends there.

#include "Chip8.h"
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>

bool Chip::load(const char *file_path)
{
	
	FILE *f = fopen("PONG", "rb");
	if(f == NULL)
		return false;
	else
		printf("Game has successfully loaded\n");

	fseek(f, 0L, SEEK_END);
	int filesize = ftell(f);
	fseek(f, 0L, SEEK_SET);

    unsigned char *buffer=(unsigned char*)malloc(filesize);
    fread(buffer, 1, filesize, f);
    
    for(int i=0; i < filesize; i++){
        memory[i + 512] = buffer[i];
    }
    
    fclose(f);
    
    return true;
	
}

void Chip::emulateCycle()
{
	// fetch opcode
	int i;
	opcode = memory[pc] << 8 | memory[pc + 1]; 
	FILE *fp;
	fp = fopen("test.txt", "a+");
	fprintf(fp,"\n %u | %u = %u\n", memory[pc] << 8, memory[pc + 1], opcode );
	fclose(fp);


	switch (opcode & 0xF000)
	{
		case(0x0000):
		{
			switch(opcode & 0x00FF)
			{
				case 0x00E0:
					OP_00E0();
					break;
				case 0x00EE:
					OP_00EE();
					break;
				default:
					printf("\nUnknown 1 op code: %u \n", opcode);
					exit(3);
					
			}
			break;
		}

		case(0x1000):
			OP_1nnn();
			break;
		case(0x2000):
			OP_2nnn();
			break;
		case(0x3000):
			OP_3xkk();
			break;
		case(0x4000):
			OP_4xkk();
			break;
		case(0x5000):
			OP_5xy0();
			break;
		case(0x6000):
			OP_6xkk();
			break;
		case(0x7000):
			OP_7xkk();
			break;
		case (0x8000):
		{
			switch(opcode & 0x000F)
			{
				case(0x0000):
					OP_8xy0();
					break;
				case(0x0001):
					OP_8xy1();
					break;
				case(0x0002):
					OP_8xy2();
					break;
				case(0x0003):
					OP_8xy3();
					break;
				case(0x0004):
					OP_8xy4();
					break;
				case(0x0005):
					OP_8xy5();
					break;
				case(0x0006):
					OP_8xy6();
					break;
				case(0x0007):
					OP_8xy7();
					break;
				case(0x000E):
					OP_8xyE();
					break;
				default:
					printf("\n Unknown 2 op code %u \n", opcode);
					exit(3);
			}
			break;
		}
		case(0x9000):
			OP_9xy0();
			break;
		case(0xA000):
			OP_Annn();
			break;
		case(0xB000):
			OP_Bnnn();
			break;
		case(0xC000):
			OP_Cxkk();
			break;
		case(0xD000):
			OP_Dxyn();
			break;
		case(0xE000):
		{
			switch(opcode & 0x00FF)
			{
				case(0x009E):
					OP_Ex9E();
					break;
				case(0x00A1):
					OP_ExA1();
					break;
				default:
					printf("\n Unknown 3 op code: %u\n", opcode);
					exit(3);
					break;
			}
			break;
		}
		case(0xF000):
		{
			switch(opcode & 0x00FF)
			{
				case(0x0007):
					OP_Fx07();
					break;
				case(0x000A):
					OP_Fx0A();
					break;
				case(0x0015):
					OP_Fx15();
					break;
				case(0x0018):
					OP_Fx18();
					break;
				case(0x001E):
					OP_Fx1E();
					break;
				case(0x0029):
					OP_Fx29();
					break;
				case(0x0033):
					OP_Fx33();
					break;
				case(0x0055):
					OP_Fx55();
					break;
				case(0x0065):
					OP_Fx65();
					break;
				default:
					printf("\nError unknown opcode in Chip8.cpp %u\n", opcode);
					exit(3);
			}
			break;
		}
	}

	if (delayTimer > 0)
	{
		--delayTimer;
	}

	// Decrement the sound timer if it's been set
	if (soundTimer > 0)
	{
		--soundTimer;
	}
}

void setKeyState(Chip chip, int key, bool state)
{
	chip.keys[key] = state;
}

void updateKeyState(Chip chip)
{
    setKeyState(chip, 0x1, sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Num1));
    setKeyState(chip, 0x2, sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Num2));
    setKeyState(chip, 0x3, sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Num3));
    setKeyState(chip, 0xC, sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Num4));

	setKeyState(chip, 0x4, sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Q));
    setKeyState(chip, 0x5, sf::Keyboard::isKeyPressed(sf::Keyboard::Key::W));
    setKeyState(chip, 0x6, sf::Keyboard::isKeyPressed(sf::Keyboard::Key::E));
    setKeyState(chip, 0xD, sf::Keyboard::isKeyPressed(sf::Keyboard::Key::R));

	setKeyState(chip, 0x7, sf::Keyboard::isKeyPressed(sf::Keyboard::Key::A));
    setKeyState(chip, 0x8, sf::Keyboard::isKeyPressed(sf::Keyboard::Key::S));
    setKeyState(chip, 0x9, sf::Keyboard::isKeyPressed(sf::Keyboard::Key::D));
    setKeyState(chip, 0xE, sf::Keyboard::isKeyPressed(sf::Keyboard::Key::F));

	setKeyState(chip, 0xA, sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Z));
    setKeyState(chip, 0x0, sf::Keyboard::isKeyPressed(sf::Keyboard::Key::X));
    setKeyState(chip, 0xB, sf::Keyboard::isKeyPressed(sf::Keyboard::Key::C));
    setKeyState(chip, 0xF, sf::Keyboard::isKeyPressed(sf::Keyboard::Key::V));
}
void drawDisplay(Chip chip, sf::RenderWindow *window)
{
	sf::RectangleShape rectangle(sf::Vector2f(13,13));
	int x, y;
	for(y = 0; y < 32; y++)
	{
		for(x = 0; x < 64; x++)
		{
			int8_t gfx = chip.graphics[x+(64*y)];
			
            if(gfx > 0)
			{
                rectangle.setPosition((float)(x*13), (float)(y*13));
                window->draw(rectangle);
            }
		}
	}
}

int main()
{
	Chip chip8;
	bool flag = chip8.load("PONG");

	if(!flag)
	{
		printf("Game could not open\n");
		return EXIT_FAILURE;
	}

	// loads window
	sf::RenderWindow *window = new sf::RenderWindow(sf::VideoMode(1280,720), "Chip 8");

	// limits framerate
	window->setFramerateLimit(60);

	sf::Clock clock;
	static float refreshSpeed = 1.f/180.f;
	
	// loops while window is open
	while(window->isOpen())
	{
		sf::Event event;
		while(window->pollEvent(event))
		{
			// if closed the window or pressed escape, the window closes

			if(event.type == sf::Event::Closed)
			{
				window->close();
			}

			if(event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Escape)
			{
				window->close();
			}
		}
		
		if(clock.getElapsedTime().asSeconds() >= refreshSpeed)
		{
			updateKeyState(chip8);

			chip8.emulateCycle();

			if(chip8.drawFlag)
			{
				window->clear();

				drawDisplay(chip8, window);
				// for(int z = 0; z < 2048; z++)
				// 	printf(" %u ", chip8.graphics[z]);
				window->display();
				chip8.drawFlag = false;	
			}
			clock.restart();
		}
	}

	delete window;
	return EXIT_SUCCESS;
}