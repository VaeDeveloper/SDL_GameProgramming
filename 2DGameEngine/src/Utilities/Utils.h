#ifndef UTILS_H
#define UTILS_H

#include "../Logger/Logger.h"
#include <cmath>

namespace GEngine
{
    namespace Math
    {
        /**
         * Random interger get
         * @param min 
         * @param max
         */
        inline int randomInt(int min, int max);

        inline float Approach(float goal, float current, float dt)
        {
            float difference = goal - current;

            if(difference > dt) return current + dt;
            if(difference < -dt) return current - dt;

            return goal;
        }

        inline float Remap(float x, float t1, float t2, float s1, float s2) 
        {
            // "Yellow" is a "normalized" value between 0 and 1
            float yellow = (x - t1) / (t2 - t1);

            // "Green" is the result!
            float green = yellow * (s2 - s1) + s1;

            return green;
        }
        
        inline float RemapClamp(float x, float t1, float t2, float s1, float s2) 
        {
            if (x < t1) return s1;
            if (x > t2) return s2;

            return Remap(x, t1, t2, s1, s2);
        }            
    }
    
    /**
     * Tools class 
     */
    class Tools
    {
    public:
        static std::string getFileNameExt(std::string fileName);
    };
}










#endif