#ifndef HIVEENGINE_PERLIN_NOISE_H
#define HIVEENGINE_PERLIN_NOISE_H

#include <cmath>
#include <random>

namespace HiveEngine {

    class PerlinImage {
    public:
        unsigned width;
        unsigned height;
        unsigned depth;
        float ratio;
        size_t seed;
        std::mt19937 gen;
        std::uniform_real_distribution<float> dist;

        std::vector<std::vector<std::vector<std::vector<char>>>> gradients;
        std::vector<char> gradients_x;
        std::vector<char> gradients_y;
        std::vector<char> gradients_z;

        PerlinImage(unsigned width, unsigned height, unsigned depth, size_t seed, float ratio);

        float get_gradient(unsigned ix, unsigned iy, unsigned iz, float x, float y, float z);

        float lerp(float a0, float a1, float w);

        float smooth_step(float a0, float a1, float w);

        float clamp(float x, float ll, float ul);

        // expects a normalized value from 0 to 1 for each dim
        float perlin(float x, float y, float z);
    };

    class PerlinDescriptor {
    public:
        size_t seed;
        unsigned width;
        unsigned height;
        unsigned depth;
        float ratio;

        PerlinDescriptor(size_t seed, unsigned width, unsigned height, unsigned depth, float ratio);
    };

    class PerlinNoise {
    public:
        std::vector<PerlinImage> images;

        PerlinNoise(std::vector<PerlinDescriptor> pds);

        // expects a normalized value from 0 to 1 for each dim
        float perlin(float x, float y, float z);
    };

}


#endif //HIVEENGINE_PERLIN_NOISE_H
