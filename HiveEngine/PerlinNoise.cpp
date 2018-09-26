#include "PerlinNoise.h"

namespace HiveEngine {

    PerlinImage::PerlinImage(unsigned width, unsigned height, unsigned depth, size_t seed, float ratio) {
        this->width = width + 2;
        this->height = height + 2;
        this->depth = depth + 2;
        this->seed = seed;
        this->ratio = ratio;

        gen = std::mt19937(seed);
        dist = std::uniform_real_distribution<float>(-1.0, 1.0);

        this->gradients_x.resize(this->width * this->height);
        this->gradients_y.resize(this->height * this->depth);
        this->gradients_z.resize(this->depth * this->width);

        for (size_t i = 0; i < gradients_x.size(); ++i) {
            gradients_x[i] = (char) (dist(gen) * 128.0f);
        }
        for (size_t i = 0; i < gradients_y.size(); ++i) {
            gradients_y[i] = (char) (dist(gen) * 128.0f);
        }
        for (size_t i = 0; i < gradients_z.size(); ++i) {
            gradients_z[i] = (char) (dist(gen) * 128.0f);
        }

        this->width = width;
        this->height = height;
        this->depth = depth;
    }

    float PerlinImage::clamp(float x, float ll, float ul) {
        if (x < ll) return ll;
        if (x > ul) return ul;
        return x;
    }

    float PerlinImage::smooth_step(float a0, float a1, float w) {
        float x = clamp((w - a0) / (a1 - a0), 0.0, 1.0);
        return x * x * (3 - 2 * x);
    }

    float PerlinImage::lerp(float a0, float a1, float w) {
        return a0 + smooth_step(0.0, 1.0, w) * (a1 - a0);
    }

    float PerlinImage::get_gradient(unsigned ix, unsigned iy, unsigned iz, float x, float y, float z) {
        float dx = x - (float) ix;
        float dy = y - (float) iy;
        float dz = z - (float) iz;

        return
                dx * gradients_x[ix * width + iy] / 128.0f +
                dy * gradients_y[iy * height + iz] / 128.0f +
                dz * gradients_z[iz * depth + ix] / 128.0f;
    }

    float fade(float t) { return t * t * t * (t * (t * 6 - 15) + 10); }

    float PerlinImage::perlin(float xn, float yn, float zn) {
        float x = width * xn;
        float y = height * yn;
        float z = depth * zn;
        int x0 = int(x);
        int x1 = x0 + 1;
        int y0 = int(y);
        int y1 = y0 + 1;
        int z0 = int(z);
        int z1 = z0 + 1;

        float sx = x - (float) x0;
        float sy = y - (float) y0;
        float sz = z - (float) z0;


        float n0, n1, ix0, ix1, ix2, ix3, value;

        n0 = get_gradient(x0, y0, z0, x, y, z);
        n1 = get_gradient(x1, y0, z0, x, y, z);
        ix0 = lerp(n0, n1, sx);

        n0 = get_gradient(x0, y1, z0, x, y, z);
        n1 = get_gradient(x1, y1, z0, x, y, z);
        ix1 = lerp(n0, n1, sx);
        float iy0 = lerp(ix0, ix1, sy);

        n0 = get_gradient(x0, y0, z1, x, y, z);
        n1 = get_gradient(x1, y0, z1, x, y, z);
        ix2 = lerp(n0, n1, sx);

        n0 = get_gradient(x0, y1, z1, x, y, z);
        n1 = get_gradient(x1, y1, z1, x, y, z);
        ix3 = lerp(n0, n1, sx);
        float iy1 = lerp(ix2, ix3, sy);

        value = lerp(iy0, iy1, sz);

        return value;
    }

    PerlinNoise::PerlinNoise(std::vector<PerlinDescriptor> pds) {
        for (size_t i = 0; i < pds.size(); i++) {
            images.push_back(PerlinImage(pds[i].width, pds[i].height, pds[i].depth, pds[i].seed, pds[i].ratio));
        }
    }

    float PerlinNoise::perlin(float x, float y, float z) {
        float fractal = 0.0;
        for (size_t i = 0; i < images.size(); i++) {
            fractal += fabs(images[i].ratio * images[i].perlin(x, y, z));
        }
        return fractal;
    }

    PerlinDescriptor::PerlinDescriptor(size_t seed, unsigned width, unsigned height, unsigned depth, float ratio) {
        this->width = width;
        this->height = height;
        this->depth = depth;
        this->ratio = ratio;
        this->seed = seed;
    }
}
