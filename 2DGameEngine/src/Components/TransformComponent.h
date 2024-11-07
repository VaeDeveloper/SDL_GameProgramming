#ifndef TRANSFORMCOMPONENT_H
#define TRANSFORMCOMPONENT_H

#include <glm/glm.h>

struct TransformComponent
{
	double rotation;
	glm::vec2 position;
	glm::vec2 scale;
};


#endif
