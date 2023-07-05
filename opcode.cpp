#include <stdio.h>
#include <stdlib.h>
#include "Chip8.h"

// opcode 00E0
// clear display
void Chip::OP_00E0()
{
	int i;
	for (i = 0; i < totalPixels; i++)
	{
		graphics[i] = 0;
	}
	drawFlag = true;
}

// opcode OOEE
// return froma subroutine
void Chip::OP_00EE()
{
	sp--;
	pc = stack[sp];
}

// opcode 1nnn
// jump to location nnn
void Chip::OP_1NNN()
{
	uint16_t address = opcode & 0x0FFF;
	pc = address;
}

// opcode 2nnn
// call subroutine at nnn
void Chip::OP_2NNN()
{
	uint16_t address = opcode & 0x0FFF;

	stack[sp] = pc;
	sp++;
	pc = address;
}

// opcode 3xkk
// skip next instruction if Vx = kk
// Note: PC is incremented by 2 in cycle. will increment by 2 again 
// jump to the next set of instructions
void Chip::OP_3XKK()
{
	uint8_t Vx = (opcode & 0x0F00) >> 8;
	uint8_t byte = opcode & 0x00FF;

	if (registers[Vx] == byte)
	{
		pc += 2;
	}
}

void Chip::OP_4XKK()
{
	uint8_t Vx = (opcode & 0x0F00) >> 8;
	uint8_t byte = (opcode & 0x00FF);

	if (registers[Vx] != byte)
	{
		pc += 2;
	}
}

void Chip::OP_5XY0()
{
	uint8_t Vx = (opcode & 0x0F00u) >> 8;
	uint8_t Vy = (opcode & 0x00F0u) >> 4;

	if (registers[Vx] == registers[Vy])
	{
		pc += 2;
	}
}

void Chip::OP_6XKK()
{
	uint8_t Vx = (opcode & 0x0F00) >> 8;
	uint8_t byte = (opcode & 0x00FF);

	registers[Vx] = byte;
}

void Chip::OP_7XKK()
{
	uint8_t Vx = (opcode & 0x0F00) >> 8;
	uint8_t byte = (opcode & 0x00FF);

	registers[Vx] += byte;
}

void Chip::OP_8XY0()
{
	uint8_t Vx = (opcode & 0x0F00) >> 8;
	uint8_t Vy = (opcode & 0x00F0) >> 4;

	registers[Vx] = registers[Vy];
}

void Chip::OP_8XY1()
{
	uint8_t Vx = (opcode & 0x0F00) >> 8;
	uint8_t Vy = (opcode & 0x00F0) >> 4;

	registers[Vx] |= registers[Vy];
	pc += 2;
}


void Chip::OP_8XY2()
{
	uint8_t Vx = (opcode & 0x0F00) >> 8;
	uint8_t Vy = (opcode & 0x00F0) >> 4;

	registers[Vx] &= registers[Vy];
}

void Chip::OP_8XY3()
{
	uint8_t Vx = (opcode & 0x0F00) >> 8;
	uint8_t Vy = (opcode & 0x00F0) >> 4;

	registers[Vx] ^= registers[Vy];
}

void Chip::OP_8XY4()
{
	uint8_t Vx = (opcode & 0x0F00) >> 8;
	uint8_t Vy = (opcode & 0x00F0) >> 4;

	uint16_t sum = registers[Vx] + registers[Vy];

	if(sum > 255)
		registers[0xF] = 1;
	else
		registers[0xF] = 0;

	registers[Vx] = sum & 0xFF;
}

void Chip::OP_8XY5()
{
	uint8_t Vx = (opcode & 0x0F00) >> 8;
	uint8_t Vy = (opcode & 0x00F0) >> 4;

	if(registers[Vx] > registers[Vy])
		registers[0xF] = 1;
	else
		registers[0xF] = 0;

	registers[Vx] -= registers[Vy];
}

void Chip::OP_8XY6()
{
	uint8_t Vx = (opcode & 0x0F00) >> 8;
	
	registers[0xF] = (registers[Vx] & 0x1);

	registers[Vx] >>= 1;
}

void Chip::OP_8XY7()
{
	uint8_t Vx = (opcode & 0x0F00) >> 8;
	uint8_t Vy = (opcode & 0x00F0) >> 4;

	if(registers[Vx] > registers[Vy])
		registers[0xF] = 1;
	else
		registers[0xF] = 0;

	registers[Vx] = registers[Vy] - registers[Vx];
}

void Chip::OP_8XYE()
{
	uint8_t Vx = (opcode & 0x0F00) >> 8;
	
	registers[0xF] = (registers[Vx] & 0x80) >> 7;

	registers[Vx] <<= 1;
}

void Chip::OP_9XY0()
{
	uint8_t Vx = (opcode & 0x0F00) >> 8;
	uint8_t Vy = (opcode & 0x00F0) >> 4;

	if (registers[Vx] != registers[Vy])
		pc += 2;
}

void Chip::OP_ANNN()
{
	uint16_t address = opcode & 0x0FFF;

	I = address;
}

void Chip::OP_BNNN()
{
	uint16_t address = opcode & 0x0FFF;

	pc = registers[0] + address;
}

void Chip::OP_CXKK()
{
	uint16_t Vx = (opcode & 0x0F00) >> 8;
	uint16_t byte = opcode & 0x00FF;

	uint16_t r = rand() % 255;

	registers[Vx] = r & byte;
}
void Chip::OP_DXYN()
{
	uint8_t Vx = (opcode & 0x0F00) >> 8;
	uint8_t Vy = (opcode & 0x00F0) >> 4;

	uint8_t h = opcode & 0x000F;

	registers[0xF] = 0;

	unsigned char spriteRow;
	unsigned char spritePixel;
	for (int row = 0; row < h; row++)
	{
		spriteRow = memory[I + row];
		for (int col = 0; col < 8; col++)
		{
			spritePixel = spriteRow & (0x80 >> col);

			int screenX = (registers[Vx] + col) % 64;
			int screenY = (registers[Vy] + row) % 32;

			unsigned char screenPixel = graphics[screenY * 64 + screenX];

			unsigned char newPixel = spritePixel ^ screenPixel;

			if (newPixel != screenPixel)
			{
				registers[0xF] = 1;
			}

			graphics[screenY * 64 + screenX] = newPixel;
		}
	}
	drawFlag = true;
}


void Chip::OP_EX9E()
{
	uint8_t Vx = (opcode & 0x0F00) >> 8;

	uint8_t key = registers[Vx];

	if (keys[key])
	{
		pc += 2;
	}
}

void Chip::OP_EXA1()
{
	uint8_t Vx = (opcode & 0x0F00) >> 8;

	uint8_t key = registers[Vx];

	if (!keys[key])
	{
		pc += 2;
	}
}

void Chip::OP_FX07()
{
	uint8_t Vx = (opcode & 0x0F00) >> 8;

	registers[Vx] = delayTimer;
}

void Chip::OP_FX0A()
{
	uint8_t Vx = (opcode & 0x0F00) >> 8;

	// Check if any key is pressed
	for (int i = 0; i < 16; i++) {
		if (keys[i]) {
			registers[Vx] = i;
			break;
		}
		else {
			pc -= 2;
		}
	}
}

void Chip::OP_FX15()
{
	uint8_t Vx = (opcode & 0x0F00) >> 8;

	delayTimer = registers[Vx];
}

void Chip::OP_FX18()
{
	uint8_t Vx = (opcode & 0x0F00) >> 8;

	soundTimer = registers[Vx];
}

void Chip::OP_FX1E()
{
	uint8_t Vx = (opcode & 0x0F00) >> 8;

	I += registers[Vx];
}

void Chip::OP_FX29()
{
	uint8_t Vx = (opcode & 0x0F00) >> 8;
	uint8_t digit = registers[Vx];

	I = fontset_start_addr + (5 * digit); // 0x50 is fontset start address
}


void Chip::OP_FX33()
{
	uint8_t Vx = (opcode & 0x0F00) >> 8;
	uint8_t value = registers[Vx];

	// Ones-place
	memory[I + 2] = value % 10;
	value /= 10;

	// Tens-place
	memory[I + 1] = value % 10;
	value /= 10;

	// Hundreds-place
	memory[I] = value % 10;
}


void Chip::OP_FX55()
{
	uint8_t Vx = (opcode & 0x0F00) >> 8;

	for (int i = 0; i <= Vx; i++) {
		memory[I + i] = registers[i];
	}
}

void Chip::OP_FX65()
{
	uint8_t Vx = (opcode & 0x0F00) >> 8;

	for (int i = 0; i <= Vx; i++) {
		registers[i] = memory[I + i];
	}
}