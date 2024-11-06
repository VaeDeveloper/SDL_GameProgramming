#include <iostream>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_mixer.h>
#include <glm/glm.hpp> 
#include <imgui/imgui.h>
#include <sol/sol.hpp> 

int main()
{
	SDL_Init(SDL_INIT_EVERYTHING);
	glm::vec2 velocity = glm::vec2(3.0f, 3.0f);
	sol::state lua;
	lua.open_libraries(sol::lib::base);
		
	
	
	std::cout << "Hello world!!!" << std::endl;
	return 0;
}


