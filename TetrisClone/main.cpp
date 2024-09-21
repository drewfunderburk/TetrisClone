#include "Engine/Window.h"

int main(int argc, char* argv[])
{
	Window* window = new Window("Tetris Clone", 800, 600);
	while (!window->windowShouldClose())
	{
		window->update();
	}
	return 0;
}