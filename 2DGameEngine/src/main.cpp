#include "./Game/Game.h"
#include <sol/sol.hpp>



#if defined(_WIN32)
    int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nShowCmd)
    {		
		Game game;
	
		game.Initialize();
		game.Run();
		game.Destroy();
	
        return 0;
    }
#endif


void TestLua()
{
	sol::state lua;
	lua.open_libraries(sol::lib::base);
	lua.script_file("./assets/scripts/myscripts.lua");

	int some_variableInsideCPP = lua["some_variable"];
	std::cout << "the value of some_variable : " << some_variableInsideCPP << std::endl;
}

#if defined(__linux__) || defined(__APPLE__)
    int main(int argc, char* argv[])
    {
		// unused params 
		(void)argc;
		(void)argv;

		Game game;
	
		game.Initialize();
		game.Run();
		game.Destroy();

	
        return 0;
    }
#else
    #error "Unsupported platform"
#endif




