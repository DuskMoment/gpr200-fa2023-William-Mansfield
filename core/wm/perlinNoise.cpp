#include "perlinNoise.h"

// Isabel Rowland

ir::PerlinNoise::PerlinNoise(unsigned int seed) {
	// Having the seed here is pointless it's just a default constructor
}

// Interpolate between two values for smoothness, etc.
float ir::PerlinNoise::interpolate(float a, float b, float w) { // w is the weight
	return (b - a) * (3.0 - w * 2.0) * w * w + a;
}

// Generates a pseudorandom gradient based on given values and a seed
// base code was courtesy of both prior knowledge and the wikipedia page on Perlin Noise
ew::Vec2 ir::PerlinNoise::randomGrad(int ix, int iy, unsigned int seed) {
	const unsigned w = 8 * sizeof(unsigned);
	const unsigned s = w / 2;
	unsigned a = ix, b = iy;
	// 
	a *= 3285157000 + seed; b ^= a << s | a >> w - s; // Smoothing
	b *= 1911520000 + seed; a ^= b << s | b >> w - s;
	a *= 2048419000 + seed;
	float rando = a * (ew::PI / ~(~0u >> 1));
	ew::Vec2 v;
	v.x = cos(rando);
	v.y = sin(rando);
	return v;
}

// Dot product of the gradient and the grid values
float ir::PerlinNoise::dotGridGrad(int ix, int iy, float x, float y, unsigned int seed) {
	ew::Vec2 grad = randomGrad(ix, iy, seed);
	float dx = x - (float)ix;
	float dy = y - (float)iy;

	return (dx * grad.x + dy * grad.y);
}


// noise gen courtesy of help from wikipedia - Isabel Rowland
float ir::PerlinNoise::noiseGen(float x, float y, unsigned int seed) {
	// Determine grid cell coordinates
	int x0 = (int)floor(x);
	int x1 = x0 + 1;
	int y0 = (int)floor(y);
	int y1 = y0 + 1;

	// Determine interpolation weights
	float sx = x - (float)x0;
	float sy = y - (float)y0;

	// Interpolate between grid point gradients
	float n0, n1, ix0, ix1, value;

	n0 = dotGridGrad(x0, y0, x, y, seed);
	n1 = dotGridGrad(x1, y0, x, y, seed);
	ix0 = interpolate(n0, n1, sx);

	n0 = dotGridGrad(x0, y1, x, y, seed);
	n1 = dotGridGrad(x1, y1, x, y, seed);
	ix1 = interpolate(n0, n1, sx);

	value = interpolate(ix0, ix1, sy);
	return value; // from -1 to 1
}