// Chip 8.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include "pch.h"
#include "Chip8.h"
#include <stdint.h>
#include <iostream>

#define programCounter 0x200
#define totalPixels 2048
#define totalSprites 80

char sprites[totalSprites]
{
	0xF0, 0x90, 0x90, 0x90, 0xF0,
	0x20, 0x60, 0x20, 0x20, 0x70,
	0xF0, 0x10, 0xF0, 0x80, 0xF0,
	0xF0, 0x10, 0xF0, 0x10, 0xF0,
	0x90, 0x90, 0xF0, 0x10, 0x10,
	0xF0, 0x80, 0xF0, 0x10, 0xF0,
	0xF0, 0x80, 0xF0, 0x90, 0xF0,
	0xF0, 0x10, 0x20, 0x40, 0x40,
	0xF0, 0x90, 0xF0, 0x90, 0xF0,
	0xF0, 0x90, 0xF0, 0x10, 0xF0,
	0xF0, 0x90, 0xF0, 0x90, 0x90,
	0xE0, 0x90, 0xE0, 0x90, 0xE0,
	0xF0, 0x80, 0x80, 0x80, 0xF0,
	0xE0, 0x90, 0x90, 0x90, 0xE0,
	0xF0, 0x80, 0xF0, 0x80, 0xF0,
	0xF0, 0x80, 0xF0, 0x80, 0x80
};

void Chip::init()
{
	int i;
	for (i = 0; i < 16; i++)
	{
		stack[i] = 0;
		V[i] = 0;
		key[i] = 0;
	}

	for (i = 0; i < totalPixels; i++)
	{
		graphics[i] = 0;
	}

	pc = programCounter;
	sp = 0;
	opcode = 0;
	I = 0;
	delayTimer = 0;
	soundTimer = 0;
}

void Chip::load(const char *file_path)
{
	if (file_path == NULL)
		return;

	FILE *game;
}

void Chip::emulateCycle()
{
	// fetch opcode
	int i;
	opcode = memory[pc] << 8 | memory[pc + 1];

	switch (opcode & 0xF000)
	{
		case(0xA000):
			for (i = 0; i < 2048; i++)
			{
				graphics[i] = 0;
			}
	}
}

int main()
{
	return 0;
}

// opcode 00E0
// clear display
void Chip::OP_00E0()
{
	int i;
	for (i = 0; i < 2048; i++)
	{
		graphics[i] = 0;
	}
}

// opcode OOEE
// return froma subroutine
void Chip::OP_00EE()
{
	--sp;
	pc = stack[sp];
}

// opcode 1nnn
// jump to location nnn
void Chip::OP_1nnn()
{
	uint16_t address = opcode & 0x0FFFu;

	pc = address;
}

// opcode 2nnn
// call subroutine at nnn
void Chip::OP_2nnn()
{
	uint16_t address = opcode & 0x0FFFu;

	stack[sp] = pc;
	++sp;
	pc = address;
}

// opcode 3xkk
// skip next instruction if Vx = kk
// Note: PC is incremented by 2 in cycle. will increment by 2 again 
// jump to the next set of instructions
void Chip::OP_3xkk()
{
	uint8_t Vx = (opcode & 0x0F00u) >> 8u;
	uint8_t byte = opcode & 0x00FFu;

	if (V[Vx] == byte)
	{
		pc += 2;
	}
}

void Chip::OP_4xkk()
{
	uint8_t Vx = (opcode & 0x0F00u) >> 8u;
	uint8_t byte = (opcode & 0x00FFu);

	if (V[Vx] != byte)
	{
		pc += 2;
	}
}

void Chip::OP_5xy0()
{
	uint8_t Vx = (opcode & 0x0F00u) >> 8u;
	uint8_t Vy = (opcode & 0x00F0u) >> 4u;

	if (V[Vx] != V[Vy])
	{
		pc += 2;
	}
}

void Chip::OP_6xkk()
{
	uint8_t Vx = (opcode & 0x0F00u) >> 8u;
	uint8_t byte = (opcode & 0x00FFu);

	V[Vx] = byte;
}

void Chip::OP_7xkk()
{
	uint8_t Vx = (opcode & 0x0F00u) >> 8u;
	uint8_t byte = (opcode & 0x00FFu);

	V[Vx] += byte;
}

void Chip::OP_8xy0()
{
	uint8_t Vx = (opcode && 0x0F00u) >> 8u;
}