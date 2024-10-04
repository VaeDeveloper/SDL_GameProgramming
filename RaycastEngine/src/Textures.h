#ifndef TEXTURES_H
#define TEXTURE_H

#include "stdint.h"
#include "Utilities/MacroFunction.h"
#include "upng.h"         

extern upng_t* _Textures[NUM_TEXTURES];

void LoadTextures(void);
void FreeTextures(void);

#endif  // TEXTURES_H
