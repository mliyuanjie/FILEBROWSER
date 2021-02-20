#include "algorithm.h"



float gauss(float sigma, float x) {
    float expVal = -1 * (pow(x, 2) / pow(2 * sigma, 2));
    float divider = sqrt(2 * M_PI * pow(sigma, 2));
    return (1 / divider) * exp(expVal);
}

std::vector<float> gaussKernel(int samples, float sigma) {
    std::vector<float> v;

    bool doubleCenter = false;
    if (samples % 2 == 0) {
        doubleCenter = true;
        samples--;
    }
    int steps = (samples - 1) / 2;
    float stepSize = (3 * sigma) / steps;

    for (int i = steps; i >= 1; i--) {
        v.push_back(gauss(sigma, i * stepSize * -1));
    }
    std::cout << std::endl;

    v.push_back(gauss(sigma, 0));
    if (doubleCenter) {
        v.push_back(gauss(sigma, 0));
    }

    for (int i = 1; i <= steps; i++) {
        v.push_back(gauss(sigma, i * stepSize));
    }

    std::cout << "The kernel contains " << v.size() << " entries:";
    for (auto it = v.begin(); it != v.end(); ++it) {
        std::cout << ' ' << *it;
    }
    std::cout << std::endl;
    assert(v.size() == samples);

    return v;
}

std::vector<float> gaussSmooth(std::vector<float>& values, float sigma, int samples) {
    std::vector<float> out;
    auto kernel = gaussKernel(samples, sigma);
    int sampleSide = samples / 2;
    int valueIdx = samples / 2 + 1;
    unsigned long ubound = values.size();
    for (unsigned long i = 0; i < ubound; i++) {
        float sample = 0;
        int sampleCtr = 0;
        std::cout << "Now at value" << i << ": ";
        for (long j = i - sampleSide; j <= i + sampleSide; j++) {
            std::cout << j << " ";
            if (j > 0 && j < ubound) {
                int sampleWeightIndex = sampleSide + (j - i);
                std::cout << "(" << sampleWeightIndex << " [" << kernel[sampleWeightIndex] << "]) ";
                sample += kernel[sampleWeightIndex] * values[j];
                sampleCtr++;
            }
        }
        float smoothed = sample / (float)sampleCtr;
        std::cout << " S: " << sample << " C: " << sampleCtr << " V: " << values[i] << " SM: " << smoothed << std::endl;
        out.push_back(smoothed);
    }
    return out;
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
            out[i] = out[i - 1] * e + data[++e];
            out[i] = out[i] / e;
        }
        else if (e < data.size()) {
            out[i] = out[i - 1] * window + data[++e] - data[s++];
            out[i] = out[i] / window;
        }
        else {
            out[i] = out[i - 1] * (e - s) - data[s++];
            out[i] = out[i] / (e - s);
        }
    }
    return out;
}

