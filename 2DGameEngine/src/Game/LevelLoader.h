#ifndef LEVELLOADER_H
#define LEVELLOADER_H

#include "../ECS/ECS.h"
#include <SDL2/SDL.h>
#include "../AssetManager/AssetManager.h"
#include <memory>
#include <sol/sol.hpp>


class LevelLoader
{
public:
    LevelLoader() ;
    ~LevelLoader();
    void LoadLevel(sol::state& lua, SDL_Renderer* renderer, const std::unique_ptr<Registry>& registry, const std::unique_ptr<AssetManager>& assetManager, int level);
    

};



#endif