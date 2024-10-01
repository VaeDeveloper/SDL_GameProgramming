#include "Textures.h"
#include "stdio.h"

static const char* _TextureFileNames[NUM_TEXTURES] = {
    "./images/redbrick.png",     // [0]
    "./images/purplestone.png",  // [1]
    "./images/mossystone.png",   // [2]
    "./images/graystone.png",    // [3]
    "./images/colorstone.png",   // [4]
    "./images/bluestone.png",    // [5]
    "./images/wood.png",         // [6]
    "./images/eagle.png",        // [7]
    "./images/pikuma.png",       // [8]
    "./images/barrel.png",       // [9]
    "./images/light.png",        // [10]
    "./images/table.png",        // [11]
    "./images/guard.png",        // [12]
    "./images/armor.png"         // [13]
};
void LoadTextures(void)
{
    for (int i = 0; i < NUM_TEXTURES; i++)
    {
        upng_t* upng = upng_new_from_file(_TextureFileNames[i]);
        if (upng != NULL)
        {
            upng_decode(upng);
            if (upng_get_error(upng) == UPNG_EOK)
            {
                _Textures[i] = upng;
            }
            else
            {
                PRINT_ERROR_LOADING("Error decode texture file %s \n", _TextureFileNames[i]);
            }
        }
        else
        {
            PRINT_ERROR_LOADING("Error loading texture file %s \n", _TextureFileNames[i]);
        }
    }
}

void FreeTextures(void)
{
    for (int i = 0; i < NUM_TEXTURES; i++)
    {
        upng_free(_Textures[i]);
    }
}
