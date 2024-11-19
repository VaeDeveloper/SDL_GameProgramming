#include "./Game/Game.h"


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




