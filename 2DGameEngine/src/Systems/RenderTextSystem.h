#ifndef RENDERTEXTSYSTEM_H
#define RENDERTEXTSYSTEM_H


#include "../ECS/ECS.h"
#include "../Components/TextRenderComponent.h"
#include "../AssetManager/AssetManager.h"

class RenderTextSystem : public System
{
public:
    RenderTextSystem()
    {
        RequireComponent<TextRenderComponent>();
    }


    void Update(SDL_Renderer* renderer, std::unique_ptr<AssetManager>& assetManager, const SDL_Rect& camera)
    {
        for (auto entity : GetSystemEntity())
        {
            const auto textLabel = entity.GetComponent<TextRenderComponent>();

            SDL_Surface* surface = TTF_RenderText_Blended(
                assetManager->GetFont(textLabel.assetId), 
                textLabel.text.c_str(),
                textLabel.color);

            SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
            SDL_FreeSurface(surface);

            int labelWidth = 0;
            int labelHeight = 0;

            SDL_QueryTexture(texture, NULL, NULL, &labelWidth, &labelHeight);

            SDL_Rect dstRect =
            {
                static_cast<int>(textLabel.position.x - (textLabel.isFixed ? 0 : camera.x)),
                static_cast<int>(textLabel.position.y - (textLabel.isFixed ? 0 : camera.y)),
                labelWidth,
                labelHeight
            };

            SDL_RenderCopy(renderer, texture, NULL, &dstRect);
            SDL_DestroyTexture(texture);
        }
    }
};


#endif