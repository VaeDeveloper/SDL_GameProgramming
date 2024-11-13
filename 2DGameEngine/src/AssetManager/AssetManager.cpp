#include "AssetManager.h"
#include "SDL2/SDL_image.h"
#include "../Logger/Logger.h"

AssetManager::AssetManager()
{
    Logger::Log("Asset Manager constructor called ");
}

AssetManager::~AssetManager()
{
    ClearAssets();
    Logger::Log("Asset Manager destructor called");
}

void AssetManager::ClearAssets()
{
    for (auto texture : textures)
    {
        SDL_DestroyTexture(texture.second);
    }

    textures.clear();
}

void AssetManager::AddTexture(SDL_Renderer* renderer, const std::string &assetID, const std::string &filePath)
{
    SDL_Surface* surface = IMG_Load(filePath.c_str());
    if (!surface) 
    {
        Logger::Err("Error load image file path ");
        return;
    }

    SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
    if (!texture)
    {
        Logger::Err("Error from create texture surface ");
        return;
    }

    SDL_FreeSurface(surface);

    textures.emplace(assetID, texture);
    Logger::Log("New texture added to the asset manager with id = " + assetID);
}

SDL_Texture* AssetManager::GetTexture(const std::string &assetID) 
{
    return textures[assetID];
}
