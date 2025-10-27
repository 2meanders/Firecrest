#include "Random.h"
#include "Maths.h"
#include <chrono>

fc::Random::Random() {
	rng.seed((unsigned int)std::chrono::high_resolution_clock::now().time_since_epoch().count());
}

fc::Random::Random(unsigned int seed) {
	rng.seed(seed);
}

void fc::Random::seed(unsigned int seed) {
	rng.seed(seed);
}

int fc::Random::nextInt() {
	return rng() - rng.max();
}

unsigned int fc::Random::nextUInt() {
	return rng();
}

float fc::Random::nextFraction() {
	return static_cast<float>(rng() / (float) rng.max());
}

int fc::Random::nextInt(int min, int max) {
	return static_cast<int>(floor((nextFraction() * (max - min))) + min);
}

int fc::Random::nextIntInc(int min, int max) {
	return static_cast<int>(floor(nextFraction() * (max - min + 1)) + min);
}

float fc::Random::nextFloat(float min, float max) {
	return nextFraction() * (max - min) + min;
}

float fc::Random::nextFloatInc(float min, float max) {
	return nextFraction() * (max - min + 1) + min;
}

float fc::Random::next() {
	static bool rngInitialized = false;
	if (!rngInitialized) {
		fc::Random::staticRNG.seed(static_cast<unsigned int>(std::chrono::high_resolution_clock::now().time_since_epoch().count()));
		rngInitialized = true;
	}
	return fc::Random::staticRNG() / static_cast<float>(fc::Random::staticRNG.max());
}

std::mt19937 fc::Random::staticRNG;
