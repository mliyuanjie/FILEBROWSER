#define _USE_MATH_DEFINES
#include <cmath>
#include <vector>

std::vector<float> gaussSmooth(std::vector<float>& values, float sigma);
std::vector<float> meanSmooth(std::vector<float>& data, int window);
std::vector<std::pair<int, int>> findPeak(std::vector<float>& data, int window, float threshold);