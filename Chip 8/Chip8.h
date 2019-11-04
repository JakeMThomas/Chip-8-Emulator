#pragma once
// Initialize Chip 8
// Load Chip 8
// Emulate Cycle

#include <stdint.h>

#define totalMemory 4096
#define registers 16
#define totalPixels 2048
#define stackSize 16
#define totalButtons 16

// Note: integers are treated equally across systems
// using uint8_t and uint16_t will give equal needed bytes across systems
class Chip
{
	private:
		//variables
		uint16_t stack[stackSize];
		uint16_t sp;
		uint8_t memory[totalMemory];
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
		void OP_Fx64();
	public:
		// variables
		uint8_t graphics[totalPixels];
		uint8_t key[totalButtons];
		bool flag;
		// methods
		void load(const char *file_path);
		void emulateCycle();
};