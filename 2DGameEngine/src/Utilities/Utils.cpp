#include "Utils.h"
#include <random>

int GEngine::Math::randomInt(int min, int max)
{
    std::random_device random_device;
    std::mt19937 gen(random_device());

    std::uniform_int_distribution<> distribution(min, max);
    return distribution(gen);
}