// Initialize Chip 8
// Load Chip 8
// Emulate Cycle

#include <stdio.h>
#include <stdint.h>
#include <unistd.h>

const int totalMemory =  4096;
const int registers = 16;
const int stackSize = 16;
const int totalButtons =  16;
const int totalPixels = 2048;
const int totalSprites = 80;
const int programCounter = 0x200;
// Note: integers are treated equally across systems
// using uint8_t and uint16_t will give equal needed bytes across systems
class Chip
{
	private:
		//variables
		uint16_t stack[stackSize];
		uint16_t sp;
		
		uint8_t V[registers];
		uint16_t pc;
		uint16_t opcode;
		uint16_t I;
		uint8_t delayTimer;
		uint8_t soundTimer;
		// methods
		void init();
		void OP_00E0();
		void OP_00EE();
		void OP_1nnn();
		void OP_2nnn();
		void OP_3xkk();
		void OP_4xkk();
		void OP_5xy0();
		void OP_6xkk();
		void OP_7xkk();
		void OP_8xy0();
		void OP_8xy1();
		void OP_8xy2();
		void OP_8xy3();
		void OP_8xy4();
		void OP_8xy5();
		void OP_8xy6();
		void OP_8xy7();
		void OP_8xyE();
		void OP_9xy0();
		void OP_Annn();
		void OP_Bnnn();
		void OP_Cxkk();
		void OP_Dxyn();
		void OP_Ex9E();
		void OP_ExA1();
		void OP_Fx07();
		void OP_Fx0A();
		void OP_Fx15();
		void OP_Fx18();
		void OP_Fx1E();
		void OP_Fx29();
		void OP_Fx33();
		void OP_Fx55();
		void OP_Fx65();

	public:
		// variables
		uint8_t memory[totalMemory];
		
		uint8_t graphics[totalPixels];
		uint8_t keys[totalButtons];
		bool drawFlag;
		// methods
		bool load(const char *file_path);
		void emulateCycle();

		unsigned char sprites[totalSprites]
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

		Chip()
		{
			pc = programCounter;
			sp = 0;
			opcode = 0;
			I = 0;
			delayTimer = 0;
			soundTimer = 0;

			int i;
			for (i = 0; i < 16; i++)
			{
				stack[i] = 0;
				V[i] = 0;
				keys[i] = 0;
			}

			for (i = 0; i < totalPixels; i++)
			{
				graphics[i] = 0;
			}

			for(i = 0; i < totalMemory; i++)
			{
				memory[i] = 0;
			}

			for(i = 0; i < totalSprites; i++)
			{
				memory[i] = sprites[i];
			}
		}
};