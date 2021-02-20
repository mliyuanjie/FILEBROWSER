#define _USE_MATH_DEFINES
#include <cmath>
#include <vector>
#include <assert.h>
#include <iostream>

std::vector<float> gaussSmooth(std::vector<float>& values, float sigma, int samples);
std::vector<float> meanSmooth(std::vector<float>& data, int window);
std::vector<std::pair<int, int>> findPeak(std::vector<float>& data, int window, float threshold);