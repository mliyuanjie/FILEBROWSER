#include "algorithm.h"
#include <cmath>
#include <vector>
#include <iostream>

class GaussianFilter
{
public:
    GaussianFilter(int kernel_size, float sigma)
    {
        m_kernel = prepare_kernel(kernel_size, sigma);
    }

    std::vector<float> apply(std::vector<float> vector)
    {
        std::vector<float> out;

        size_t kernelSide = m_kernel.size() / 2;

        for (size_t i = 0; i < kernelSide; i++)
        {
            vector.insert(vector.begin(), vector[0]);
            vector.push_back(vector[vector.size() - 1]);
        }

        size_t ubound = vector.size();
        size_t vectorIndex;
        float smoothed;

        for (size_t i = kernelSide; i < ubound - kernelSide; i++)
        {
            smoothed = 0;

            for (size_t j = 0; j < m_kernel.size(); j++)
            {
                vectorIndex = i + j - kernelSide;

                smoothed += m_kernel[j] * vector[vectorIndex];
            }

            out.push_back(smoothed);
        }
        return out;
    }

private:
    std::vector<float> prepare_kernel(int kernel_size, float sigma)
    {
        std::vector<float> v;

        bool doubleCenter = false;
        if (kernel_size % 2 == 0) {
            doubleCenter = true;
            kernel_size--;
        }
        int steps = (kernel_size - 1) / 2;

        for (int i = steps; i >= 1; i--) {
            v.push_back(gaussian(sigma, -i));
        }

        v.push_back(gaussian(sigma, 0));
        if (doubleCenter) {
            v.push_back(gaussian(sigma, 0));
        }

        for (int i = 1; i <= steps; i++) {
            v.push_back(gaussian(sigma, i));
        }

        return v;
    }

    float gaussian(float sigma, float value)
    {
        return (1.f / std::sqrtf(2.f * M_PI * sigma) * std::expf(-(value * value) / (2.f * sigma * sigma)));
    }

private:
    std::vector<float> m_kernel;
};

std::vector<float> gaussSmooth(std::vector<float>& values, float sigma) {
    GaussianFilter filter(10, sigma);
    return filter.apply(values);
}


std::vector<float> meanSmooth(std::vector<float>& data, int window) {
    std:: vector<float> out(data.size());
    int s = 0;
    int e = window / 2;
    for (int i = 0; i < e; i++) {
        out[0] += data[i];
    }
    out[0] = out[0] / e;
    for (int i = 1; i < data.size(); i++) {
        if (e < window) {
            out[i] = out[i - 1] * e + data[e++];
            out[i] = out[i] / e;
        }
        else if (e < data.size()) {
            out[i] = out[i - 1] * window + data[e++] - data[s++];
            out[i] = out[i] / window;
        }
        else {
            out[i] = out[i - 1] * (e - s) - data[s++];
            out[i] = out[i] / (e - s);
        }
    }
    return out;
}


std::vector<std::pair<int, int>> findPeak(std::vector<float>& data, int window, float threshold) {
    std::vector<float> baseline(data.size());
    std::vector<std::pair<int, int>> out;
    int s = 0;
    int e = window / 2;
    bool flag = false;
    for (int i = 0; i < e; i++) {
        baseline[0] += data[i];
    }
    baseline[0] = baseline[0] / e;
    int begin;
    int finish;
    for (int i = 1; i < data.size(); i++) {
        if (e < window) {
            baseline[i] = baseline[i - 1] * e + data[e++];
            baseline[i] = baseline[i] / e;
        }
        else if (e < data.size()) {
            baseline[i] = baseline[i - 1] * window + data[e++] - data[s++];
            baseline[i] = baseline[i] / window;
        }
        else {
            baseline[i] = baseline[i - 1] * (e - s) - data[s++];
            baseline[i] = baseline[i] / (e - s);
        }
        if (!flag && data[i] < baseline[i] - threshold) {
            begin = i;
            while (data[begin] < baseline[begin] && begin > 1 && !(data[begin] > data[begin-1] && data[begin] >data[begin + 1])) 
                begin--;
            flag = true;
        }
        else if (flag && data[i] >= baseline[i] && i>0 && i < data.size()-1 && data[i] > data[i - 1] && data[i] > data[i + 1]) {
            finish = i;
            flag = false;
            out.push_back(std::pair<int, int>(begin, finish));
        }
    }
    return out;
}
