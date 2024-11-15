#ifndef RENDERSYSTEM_H
#define RENDERSYSTEM_H

#include "../ECS/ECS.h"
#include "../Components/SpriteComponent.h"
#include "../Components/TransformComponent.h"
#include "../AssetManager/AssetManager.h"
#include <algorithm>
#include <SDL2/SDL.h>

/**
 * RenderSystem is responsible for rendering entities with the required components.
 * It filters entities to those that contain both TransformComponent and SpriteComponent,
 * sorts them by their z-index, and renders them to the screen using SDL.
 */
class RenderSystem : public System
{
public:
    /**
     * Constructs a RenderSystem and specifies required components.
     * Entities must have both TransformComponent and SpriteComponent
     * to be processed by this system.
     */
    RenderSystem()
    {
        RequireComponent<TransformComponent>();
        RequireComponent<SpriteComponent>();
    }

    /**
     * Updates and renders all entities managed by this RenderSystem.
     * Filters entities by the required components, sorts them based on their
     * z-index in ascending order, and renders each entity on the SDL_Renderer.
     *
     * @param renderer Pointer to SDL_Renderer used for rendering entities.
     * @param assetManager Unique pointer to AssetManager for managing and accessing textures.
     */
    void Update(SDL_Renderer* renderer,std::unique_ptr<AssetManager>& assetManager)
    {
        struct RenderableEntity
        {
            TransformComponent transformComponent;
            SpriteComponent spriteComponent;
        };

        std::vector<RenderableEntity> renderableEntities;

        for (auto entity : GetSystemEntity())
        {
            RenderableEntity renderableEntity;
            renderableEntity.spriteComponent = entity.GetComponent<SpriteComponent>();
            renderableEntity.transformComponent = entity.GetComponent<TransformComponent>();
            renderableEntities.emplace_back(renderableEntity);
        }

        // Sort entities by z-index to render in correct order
        std::sort(renderableEntities.begin(), renderableEntities.end(), 
        [](const RenderableEntity& a, const RenderableEntity& b)
        {
            return a.spriteComponent.zIndex < b.spriteComponent.zIndex;            
        });

        // Render each entity
        for (auto entity : renderableEntities)
        {
            const auto transform = entity.transformComponent;
            const auto sprite = entity.spriteComponent;

            const SDL_Rect srcRect = sprite.srcRect;

            const SDL_Rect dstRect = 
            {
                static_cast<int>(transform.position.x),
                static_cast<int>(transform.position.y),
                static_cast<int>(sprite.width * transform.scale.x),
                static_cast<int>(sprite.height * transform.scale.y)
            };

            const auto texture = assetManager->GetTexture(sprite.assetID);
            if (!texture)
            {
                Logger::Err("Error loading texture " + sprite.assetID);
                continue;
            }

            SDL_RenderCopyEx(
                renderer, 
                texture,
                &srcRect,
                &dstRect,
                transform.rotation,
                NULL,
                SDL_FLIP_NONE);
        }
    }
};


#endif
