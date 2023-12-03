#pragma once
#include "../ew/ewMath/ewMath.h"
#include "math.h"
namespace ir {
	class PerlinNoise {
	private:
		float gradient(float x, float y, float z, int hash);
		float fade(float t);
	public:
		std::vector<int> perm;
		PerlinNoise(); // Default constructor
		PerlinNoise(unsigned int seed);
		float noiseGen(float x, float y);
		// void shuffle(int &arr);
		// std::vector<int> makePerm();
		ew::Vec2 getConstants(int val);
		float interpolate(float a, float b, float w);
		// float noise(float x, float y);
		ew::Vec2 randomGrad(int ix, int iy);
		float dotGridGrad(int ix, int iy, float x, float y);
	};
}