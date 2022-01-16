#pragma once

#include <cstddef>

class SimplexNoise {
private:
    float mFrequency;
    float mAmplitude;
    float mLacunarity;
    float mPersistence;
public:
    static float noise(float x);
    static float noise(float x, float y);
    static float noise(float x, float y, float z);

    float fractal(size_t octaves, float x) const;
    float fractal(size_t octaves, float x, float y) const;
    float fractal(size_t octaves, float x, float y, float z) const;

    explicit SimplexNoise(float frequency = 1.0f,
        float amplitude = 1.0f,
        float lacunarity = 2.0f,
        float persistence = 0.5f) :
        mFrequency(frequency),
        mAmplitude(amplitude),
        mLacunarity(lacunarity),
        mPersistence(persistence) {
    }
};