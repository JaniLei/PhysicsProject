#include <SDL.h>
#include "core.h"


int main(int argc, char* args[])
{
	core::init();
	core::update();
	core::cleanUp();

	return 0;
}