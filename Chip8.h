//initialize chip 8
//load chip 8
//emulate cycle

#include <stdio.h>
#include <stdint.h>

const int totalMemory = 4096;
const int totalRegisters = 16;
const int stackSize = 16;
const int totalButtons = 16;
const int totalPixels = 2048;
const unsigned int totalSprites = 80;
const unsigned int programcounter = 0x200;
const unsigned int fontset_start_addr = 0x50;
const int videowidth = 64;
const int videoheight = 32;
const int pixelSize = 13;
// note: integers are treated equally across systems
// using uint8_t and uint16_t will give equal needed bytes across systems
class Chip
{
private:
	//variables
	uint16_t stack[stackSize];
	uint8_t sp;
	uint8_t registers[totalRegisters];
	uint16_t pc;
	// uint16_t opcode;
	uint16_t I;
	uint8_t delayTimer;
	uint8_t soundTimer;
	// methods
	void OP_00E0();
	void OP_00EE();
	void OP_1NNN();
	void OP_2NNN();
	void OP_3XKK();
	void OP_4XKK();
	void OP_5XY0();
	void OP_6XKK();
	void OP_7XKK();
	void OP_8XY0();
	void OP_8XY1();
	void OP_8XY2();
	void OP_8XY3();
	void OP_8XY4();
	void OP_8XY5();
	void OP_8XY6();
	void OP_8XY7();
	void OP_8XYE();
	void OP_9XY0();
	void OP_ANNN();
	void OP_BNNN();
	void OP_CXKK();
	void OP_DXYN();
	void OP_EX9E();
	void OP_EXA1();
	void OP_FX07();
	void OP_FX0A();
	void OP_FX15();
	void OP_FX18();
	void OP_FX1E();
	void OP_FX29();
	void OP_FX33();
	void OP_FX55();
	void OP_FX65();

public:
	// variables
	uint8_t memory[totalMemory];
	uint32_t graphics[videowidth * videoheight];
	uint8_t keys[totalButtons];
	bool drawFlag;
	// methods
	bool load(const char* file_path);
	uint16_t opcode;
	void emulatecycle();

	unsigned char sprites[totalSprites]
	{
		0xf0, 0x90, 0x90, 0x90, 0xf0, // 0
		0x20, 0x60, 0x20, 0x20, 0x70, // 1
		0xf0, 0x10, 0xf0, 0x80, 0xf0, // 2
		0xf0, 0x10, 0xf0, 0x10, 0xf0, // 3
		0x90, 0x90, 0xf0, 0x10, 0x10, // 4
		0xf0, 0x80, 0xf0, 0x10, 0xf0, // 5
		0xf0, 0x80, 0xf0, 0x90, 0xf0, // 6
		0xf0, 0x10, 0x20, 0x40, 0x40, // 7
		0xf0, 0x90, 0xf0, 0x90, 0xf0, // 8
		0xf0, 0x90, 0xf0, 0x10, 0xf0, // 9
		0xf0, 0x90, 0xf0, 0x90, 0x90, // a
		0xe0, 0x90, 0xe0, 0x90, 0xe0, // b
		0xf0, 0x80, 0x80, 0x80, 0xf0, // c
		0xe0, 0x90, 0x90, 0x90, 0xe0, // d
		0xf0, 0x80, 0xf0, 0x80, 0xf0, // e
		0xf0, 0x80, 0xf0, 0x80, 0x80  // f
	};

	Chip()
	{
		pc = programcounter;
		sp = 0;
		opcode = 0;
		I = 0;
		delayTimer = 0;
		soundTimer = 0;
		drawFlag = true;

		int i;

		// clear stack, registers, and 
		for (i = 0; i < 16; i++)
		{
			stack[i] = 0;
			registers[i] = 0;
			keys[i] = 0;
		}

		// clear display
		for (i = 0; i < totalPixels; i++)
		{
			graphics[i] = 0;
		}

		// clear memory
		for (i = 0; i < totalMemory; i++)
		{
			memory[i] = 0;
		}


		// load font set
		for (int i = 0; i < totalSprites; i++)
		{
			memory[fontset_start_addr + i] = sprites[i];
		}
	}
};

