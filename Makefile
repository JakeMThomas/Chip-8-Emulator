all:
	g++ Chip8.cpp opcode.cpp -o sfml-app -lsfml-graphics -lsfml-window -lsfml-system
	./sfml-app
