#include <stdio.h>
#include <stdlib.h>
#include "Chip8.h"

// opcode 00E0
// clear display
void Chip::OP_00E0()
{
	int i;
	for (i = 0; i < 2048; i++)
	{
		graphics[i] = 0;
	}
	drawFlag = true;
	pc += 2;
}

// opcode OOEE
// return froma subroutine
void Chip::OP_00EE()
{
	sp--;
	pc = stack[sp];
	pc += 2;
}

// opcode 1nnn
// jump to location nnn
void Chip::OP_1nnn()
{
	uint16_t address = opcode & 0x0FFF;
	pc = address;
}

// opcode 2nnn
// call subroutine at nnn
void Chip::OP_2nnn()
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
void Chip::OP_3xkk()
{
	uint8_t Vx = (opcode & 0x0F00) >> 8;
	uint16_t byte = opcode & 0x00FF;

	if (V[Vx] == byte)
	{
		pc += 4;
	}
	else
	{
		pc += 2;
	}
	
}

void Chip::OP_4xkk()
{
	uint8_t Vx = (opcode & 0x0F00) >> 8;
	uint16_t byte = (opcode & 0x00FF);

	if (V[Vx] != byte)
	{
		pc += 4;
	}
	else
	{
		pc += 2;
	}
	
}

void Chip::OP_5xy0()
{
	uint8_t Vx = (opcode & 0x0F00u) >> 8;
	uint8_t Vy = (opcode & 0x00F0u) >> 4;

	if (V[Vx] != V[Vy])
	{
		pc += 4;
	}
	else
	{
		pc += 2;
	}
	
}

void Chip::OP_6xkk()
{
	uint8_t Vx = (opcode & 0x0F00) >> 8;
	uint16_t byte = (opcode & 0x00FF);

	V[Vx] = byte;
	pc += 2;
}

void Chip::OP_7xkk()
{
	uint8_t Vx = (opcode & 0x0F00) >> 8;
	uint16_t byte = (opcode & 0x00FF);

	V[Vx] += byte;
	pc += 2;
}

void Chip::OP_8xy0()
{
	uint8_t Vx = (opcode && 0x0F00) >> 8;
	uint8_t Vy = (opcode && 0x00F0) >> 4;

	V[Vx] = V[Vy];
	pc += 2;
}

void Chip::OP_8xy1()
{
	uint8_t Vx = (opcode && 0x0F00) >> 8;
	uint8_t Vy = (opcode && 0x00F0) >> 4;

	V[Vx] |= V[Vy];
	pc += 2;
}

void Chip::OP_8xy2()
{
	uint8_t Vx = (opcode & 0x0F00) >> 8;
	uint8_t Vy = (opcode & 0x00F0) >> 4;

	V[Vx] &= V[Vy];
	pc += 2;
}

void Chip::OP_8xy3()
{
	uint8_t Vx = (opcode & 0x0F00) >> 8;
	uint8_t Vy = (opcode & 0x00F0) >> 4;

	V[Vx] ^= V[Vy];
	pc += 2;
}

void Chip::OP_8xy4()
{
	uint8_t Vx = (opcode & 0x0F00) >> 8;
	uint8_t Vy = (opcode & 0x00F0) >> 4;

	uint16_t sum = V[Vx] + V[Vy];

	if(sum > 255)
		V[0xF] = 1;
	else
		V[0xF] = 0;

	V[Vx] = sum & 0xFF;

	pc += 2;
}

void Chip::OP_8xy5()
{
	uint8_t Vx = (opcode & 0x0F00) >> 8;
	uint8_t Vy = (opcode & 0x00F0) >> 4;

	if(V[Vx] > V[Vy])
		V[0xF] = 1;
	else
		V[0xF] = 0;

	V[Vx] -= V[Vy];
	pc += 2;
}

void Chip::OP_8xy6()
{
	uint8_t Vx = (opcode & 0x0F00) >> 8;
	
	V[0xF] = (V[Vx] & 0x1);

	V[Vx] >>= 1;
	pc += 2;
}

void Chip::OP_8xy7()
{
	uint8_t Vx = (opcode & 0x0F00) >> 8;
	uint8_t Vy = (opcode & 0x00F0) >> 4;

	if(V[Vx] > V[Vy])
		V[0xF] = 1;
	else
		V[0xF] = 0;

	V[Vx] = V[Vy] - V[Vx];
	pc += 2;
}

void Chip::OP_8xyE()
{
	uint8_t Vx = (opcode & 0x0F00) >> 8;
	
	V[0xF] = (V[Vx] & 0x80) >> 7;

	V[Vx] <<= 1;
	pc += 2;
}

void Chip::OP_9xy0()
{
	uint8_t Vx = (opcode & 0x0F00) >> 8;
	uint8_t Vy = (opcode & 0x00F0) >> 4;

	if(V[Vx] !=  V[Vy])
		pc += 4;
	else
		pc += 2;
	
}

void Chip::OP_Annn()
{
	uint16_t address = opcode & 0x0FFF;

	I = address;
	pc += 2;
}

void Chip::OP_Bnnn()
{
	uint16_t address = opcode & 0x0FFF;

	pc = V[0] + address;
}

void Chip::OP_Cxkk()
{
	uint16_t Vx = (opcode & 0x0F00) >> 8;
	uint16_t byte = opcode & 0x00FF;

	uint16_t r = rand() % 255;

	V[Vx] = r & byte;
	pc += 2;
}

void Chip::OP_Dxyn()
{
	uint8_t x = V[(opcode & 0x0F00) >> 8]; 
	uint8_t y = V[(opcode & 0x0F00) >> 4]; 
	uint8_t h = opcode & 0x000F;
	V[0xF] = 0;

	uint8_t pixel;
	for(int yline = 0; yline < h; yline++)
	{
		pixel = memory[I + yline];
		for(int xline = 0; xline < 8; xline++)
		{
			if((pixel & (0x80 >> xline)) != 0)
			{
				if(graphics[(x + xline + ((y + yline) * 64))] == 1)
				{
					V[0xF] = 1;
				}

				graphics[x + xline + ((y + yline) * 64)] ^= 1;
			}
		}
	}
	drawFlag = true;
	pc += 2;
}

void Chip::OP_Ex9E()
{
	uint8_t Vx = (opcode & 0x0F00) >> 8;

	uint8_t key = V[Vx];

	if (keys[key] != 0)
	{
		pc += 4;
	}
	else
	{
		pc += 2;
	}
	
}

void Chip::OP_ExA1()
{
	uint8_t Vx = (opcode & 0x0F00) >> 8;

	uint8_t key = V[Vx];

	if (keys[key] == 0)
	{
		pc += 4;
	}
	else
	{
		pc += 2;
	}
}

void Chip::OP_Fx07()
{
	uint8_t Vx = (opcode & 0x0F00) >> 8;

	V[Vx] = delayTimer;
	pc += 2;
}

void Chip::OP_Fx0A()
{
	uint8_t Vx = (opcode & 0x0F00) >> 8;

	if (keys[0])
	{
		V[Vx] = 0;
	}
	else if (keys[1])
	{
		V[Vx] = 1;
	}
	else if (keys[2])
	{
		V[Vx] = 2;
	}
	else if (keys[3])
	{
		V[Vx] = 3;
	}
	else if (keys[4])
	{
		V[Vx] = 4;
	}
	else if (keys[5])
	{
		V[Vx] = 5;
	}
	else if (keys[6])
	{
		V[Vx] = 6;
	}
	else if (keys[7])
	{
		V[Vx] = 7;
	}
	else if (keys[8])
	{
		V[Vx] = 8;
	}
	else if (keys[9])
	{
		V[Vx] = 9;
	}
	else if (keys[10])
	{
		V[Vx] = 10;
	}
	else if (keys[11])
	{
		V[Vx] = 11;
	}
	else if (keys[12])
	{
		V[Vx] = 12;
	}
	else if (keys[13])
	{
		V[Vx] = 13;
	}
	else if (keys[14])
	{
		V[Vx] = 14;
	}
	else if (keys[15])
	{
		V[Vx] = 15;
	}
	else
	{
		pc += 2;
	}
}

void Chip::OP_Fx15()
{
	uint8_t Vx = (opcode & 0x0F00) >> 8;

	delayTimer = V[Vx];
	pc += 2;
}

void Chip::OP_Fx18()
{
	uint8_t Vx = (opcode & 0x0F00) >> 8;

	soundTimer = V[Vx];
	pc += 2;
}

void Chip::OP_Fx1E()
{
	uint8_t Vx = (opcode & 0x0F00) >> 8;

	I += V[Vx];
	pc += 2;
}

void Chip::OP_Fx29()
{
	uint8_t Vx = (opcode & 0x0F00) >> 8;
	uint8_t digit = V[Vx];

	I = (5 * digit); // 0x50 is fontset start address
	pc += 2;
}

void Chip::OP_Fx33()
{
	uint8_t Vx = (opcode & 0x0F00) >> 8;
	uint8_t value = V[Vx];

	// Ones-place
	memory[I + 2] = value % 10;
	value /= 10;

	// Tens-place
	memory[I + 1] = value % 10;
	value /= 10;

	// Hundreds-place
	memory[I] = value % 10;

	pc += 2;
}

void Chip::OP_Fx55()
{
	uint8_t Vx = (opcode & 0x0F00) >> 8;

	for (int i = 0; i <= Vx; i++)
	{
		memory[I + i] = V[i];
	}

	I = Vx + 1;

	pc += 2;
}

void Chip::OP_Fx65()
{
	uint8_t Vx = (opcode & 0x0F00) >> 8;

	for (uint8_t i = 0; i <= Vx; ++i)
	{
		V[i] = memory[I + i];
	}

	I = Vx + 1;

	pc += 2;
}
