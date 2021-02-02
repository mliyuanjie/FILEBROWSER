#include "abf.h"

ABF::ABF(const char* f, unsigned int n) {
	fn = f;
	error = 0;
	hfile = 0;
	maxsamples = n;
	maxepi = 0;
	module = LoadLibraryA("../FILEBROWSER/bin/ABFFIO.dll");
	ABF_ReadOpen = (pABF_ReadOpen)GetProcAddress(module, "ABF_ReadOpen");
	ABF_Close = (pABF_Close)GetProcAddress(module, "ABF_Close");
	ABF_ReadChannel = (pABF_ReadChannel)GetProcAddress(module, "ABF_ReadChannel");
	ABF_GetWaveform = (pABF_GetWaveform)GetProcAddress(module, "ABF_GetWaveform");
	ABF_ReadOpen(fn, &hfile, ABF_DATAFILE, &fh, &maxsamples, &maxepi, &error);
	if (fh.nOperationMode == 3) {
		buffer = new float[maxsamples * maxepi];
	}
	else if (fh.nOperationMode == 5) {
		buffer = new float[maxsamples];
	}
	Channel = fh.nADCNumChannels;
	if (fh.nOperationMode == 5) {
		Sweep = fh.lActualEpisodes;
	}
	Interval = fh.fADCSequenceInterval;
}

ABF::~ABF(){
	if (buffer == NULL) {
		return;
	}
	ABF_Close(hfile, &error);
	FreeLibrary(module);
	delete[] buffer;
	buffer = NULL;
}

std::vector<float> ABF::data(int c, int s, bool m) {
	//ABF_ReadOpen(fn, &hfile, ABF_DATAFILE, &fh, &maxsamples, &maxepi, &error);
	std::vector<float> t;
	if (buffer == NULL) {
		return t;
	}
	if (fh.nOperationMode == 3 && m) {
		float* res = buffer;
		unsigned int numsamples = maxsamples;
		for (unsigned int i = 1; i <= maxepi; i++) {
			ABF_ReadChannel(hfile, &fh, c, i, res, &numsamples, &error);
			res += numsamples;
		}
		t = std::vector<float>(buffer, res);
	}
	else if (fh.nOperationMode == 5 && m) {
		ABF_ReadChannel(hfile, &fh, c, s, buffer, &maxsamples, &error);
		t = std::vector<float>(buffer, buffer + maxsamples);
	}
	else if (fh.nOperationMode == 3 && !m) {
		float* res = buffer;
		for (unsigned int i = 1; i <= maxepi; i++) {
			ABF_GetWaveform(hfile, &fh, c, i, res, &error);
			res += maxsamples;
		};
		t = std::vector<float>(buffer, buffer + fh.lActualAcqLength/fh.nADCNumChannels);
	}
	else if (fh.nOperationMode == 5 && !m) {
		ABF_GetWaveform(hfile, &fh, c, s, buffer, &error);
		t = std::vector<float>(buffer, buffer + maxsamples);
	}
	return t;
}