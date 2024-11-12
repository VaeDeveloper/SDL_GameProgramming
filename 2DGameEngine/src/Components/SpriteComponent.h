#ifndef SPRITECOMPONENT_H
#define SPRITECOMPONENT_H

#include <string>

struct SpriteComponent
{
    int width;
    int height;
    std::string assetID;

    SpriteComponent(std::string assetID = "", int width = 0, int height = 0)
    {
        this->assetID = assetID;
        this->width = width;
        this->height = height;
    }
};

#endif