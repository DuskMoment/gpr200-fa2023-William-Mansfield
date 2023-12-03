#include "perlinNoise.h"
ir::PerlinNoise::PerlinNoise(unsigned int seed) {
	perm.resize(seed);

}

//void ir::PerlinNoise::shuffle(int& arr) {
//	for (int i = sizeof(arr) - 1; i > 0; i--) {
//		int index = round(rand() * (i - 1));
//		int temp = arr[i];
//		arr[i] = arr[index];
//		arr[index] = temp;
//	}
//}

//std::vector<int> ir::PerlinNoise::makePerm() {
//	std::vector<int> perm;
//	for (int i = 0; i < 256; i++) {
//		perm.push_back(i);
//	}
//	shuffle(perm);
//	for (int i = 0; i < 256; i++) {
//		perm.push_back(perm[i]);
//	}
//	return perm;
//}

ew::Vec2 ir::PerlinNoise::getConstants(int v) {
	int h = v % 4;
	if (h == 0)
		return ew::Vec2(1.0, 1.0);
	else if (h == 1)
		return ew::Vec2(-1.0, 1.0);
	else if (h == 2)
		return ew::Vec2(-1.0, -1.0);
	else {
		return ew::Vec2(1.0, -1.0);
	}

}

float ir::PerlinNoise::interpolate(float a, float b, float w) { // w is the weight
	return (b - a) * (3.0 - w * 2.0) * w * w + a;
}

//float ir::PerlinNoise::noise(float x, float y) {
//	int _x = static_cast<int>(floor(x)) % 256;
//	int _y = static_cast<int>(floor(y)) % 256;
//
//	float xf = x - floor(x);
//	float yf = y - floor(y);
//
//	ew::Vec2 topRight = ew::Vec2(xf - 1.0, yf - 1.0);
//	ew::Vec2 topLeft = ew::Vec2(xf, yf - 1.0);
//	ew::Vec2 bottomRight = ew::Vec2(xf - 1.0, yf);
//	ew::Vec2 bottomLeft = ew::Vec2(xf, yf);
//
//	std::vector<int> permutations = makePerm();
//
//	int valTopRight = permutations[permutations[_x + 1] + _y + 1];
//	int valTopLeft = permutations[permutations[_x] + _y + 1];
//	int valBottomRight = permutations[permutations[_x + 1] + _y];
//	int valBottomLeft = permutations[permutations[_x] + _y];
//
//	float dotTopRight = ew::Dot(topRight, getConstants(valTopRight));
//	float dotTopLeft = ew::Dot(topLeft, getConstants(valTopLeft));
//	float dotBottomRight = ew::Dot(bottomRight, getConstants(valBottomRight));
//	float dotBottomLeft = ew::Dot(bottomLeft, getConstants(valBottomLeft));
//
//	return -1;
//
//}

ew::Vec2 ir::PerlinNoise::randomGrad(int ix, int iy) {
	const unsigned w = 8 * sizeof(unsigned);
	const unsigned s = w / 2;
	unsigned a = ix, b = iy;
	a *= 3285157443; b ^= a << s | a >> w - s; // The numbers just work don't touch them please
	b *= 1911520719; a ^= b << s | b >> w - s;
	a *= 2048419325;
	float rando = a * (ew::PI / ~(~0u >> 1));
	ew::Vec2 v;
	v.x = cos(rando);
	v.y = sin(rando);
	return v;
}

float ir::PerlinNoise::dotGridGrad(int ix, int iy, float x, float y) {
	ew::Vec2 grad = randomGrad(ix, iy);
	float dx = x - (float)ix;
	float dy = y - (float)iy;

	return (dx * grad.x + dy * grad.y);
}

float ir::PerlinNoise::noiseGen(float x, float y) {
	// Determine grid cell coordinates
	int x0 = (int)floor(x);
	int x1 = x0 + 1;
	int y0 = (int)floor(y);
	int y1 = y0 + 1;

	// Determine interpolation weights
	// Could also use higher order polynomial/s-curve here
	float sx = x - (float)x0;
	float sy = y - (float)y0;

	// Interpolate between grid point gradients
	float n0, n1, ix0, ix1, value;

	n0 = dotGridGrad(x0, y0, x, y);
	n1 = dotGridGrad(x1, y0, x, y);
	ix0 = interpolate(n0, n1, sx);

	n0 = dotGridGrad(x0, y1, x, y);
	n1 = dotGridGrad(x1, y1, x, y);
	ix1 = interpolate(n0, n1, sx);

	value = interpolate(ix0, ix1, sy);
	return value; // Will return in range -1 to 1. To make it in range 0 to 1, multiply by 0.5 and add 0.5
}