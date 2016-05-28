#include "Utils.h"

static std::mt19937 getRandomEngine() {
	std::random_device rd;
	std::mt19937 mt(rd());
	return mt;
}

double Utils::getRandNumber(double min, double max) {
	std::mt19937 mt = getRandomEngine();
	std::uniform_real_distribution<double> dist(min, max);
	return dist(mt);
}

float Utils::getRandNumber(float min, float max) {
	std::mt19937 mt = getRandomEngine();
	std::uniform_real_distribution<float> dist(min, max);
	return dist(mt);
}

