#pragma once
#include "../ew/ewMath/ewMath.h"
#include "math.h"
namespace ir {
	class PerlinNoise {
	private:
		float gradient(float x, float y, float z, int hash);
		float fade(float t);
	public:
		PerlinNoise(); // Default constructor
		PerlinNoise(unsigned int seed);
		float noiseGen(float x, float y, unsigned int seed);
		float interpolate(float a, float b, float w);
		ew::Vec2 randomGrad(int ix, int iy, unsigned int seed);
		float dotGridGrad(int ix, int iy, float x, float y, unsigned int seed);
	};
}