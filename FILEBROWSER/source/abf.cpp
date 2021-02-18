#include "abf.h"

ABF::ABF(std::string f, QObject* parent, unsigned int n) : QObject(parent) {
	fn = f;
	if (fn.substr(fn.length() - 3, 3) == "dat")
		return;
	error = 0;
	hfile = 1;
	maxsamples = n;
	maxepi = 0;
	module = LoadLibraryA("./bin/ABFFIO.dll");
	ABF_ReadOpen = (pABF_ReadOpen)GetProcAddress(module, "ABF_ReadOpen");
	ABF_Close = (pABF_Close)GetProcAddress(module, "ABF_Close");
	ABF_ReadChannel = (pABF_ReadChannel)GetProcAddress(module, "ABF_ReadChannel");
	ABF_GetWaveform = (pABF_GetWaveform)GetProcAddress(module, "ABF_GetWaveform");
	ABF_WriteOpen = (pABF_WriteOpen)GetProcAddress(module, "ABF_WriteOpen");
	ABF_MultiplexWrite = (pABF_MultiplexWrite)GetProcAddress(module, "ABF_MultiplexWrite");
	ABF_WriteDACFileEpi = (pABF_WriteDACFileEpi)GetProcAddress(module, "ABF_WriteDACFileEpi");
	ABF_UpdateHeader = (pABF_UpdateHeader)GetProcAddress(module, "ABF_UpdateHeader");
	//ABF_SynchCountFromEpisode = (pABF_SynchCountFromEpisode)GetProcAddress(module, "ABF_SynchCountFromEpisode");
	ABF_MultiplexRead = (pABF_MultiplexRead)GetProcAddress(module, "ABF_MultiplexRead");
	ABF_ReadDACFileEpi = (pABF_ReadDACFileEpi)GetProcAddress(module, "ABF_ReadDACFileEpi");
	ABF_ReadOpen(fn.c_str(), &hfile, ABF_DATAFILE, &fh, &maxsamples, &maxepi, &error);
	Channel = fh.nADCNumChannels;
	if (fh.nOperationMode == 5) {
		Sweep = fh.lActualEpisodes;
	}
	Interval = fh.fADCSequenceInterval;
	ABF_Close(hfile, &error);
}

ABF::~ABF(){
	if (fn.substr(fn.length() - 3, 3) == "dat")
		return;
	FreeLibrary(module);
}

void ABF::readData(int c, int s, bool m) {
	if (fn.substr(fn.length() - 3, 3) == "dat") {
		std::ifstream file;
		file.open(fn);
		float d;
		while (!file.eof()) {
			file >> d;
			data.push_back(d);
		}
		file.close();
		return;
	}
	ABF_ReadOpen(fn.c_str(), &hfile, ABF_DATAFILE, &fh, &maxsamples, &maxepi, &error);
	float* buffer;
	if (fh.nOperationMode == 3 && m) {
		buffer = new float[maxsamples * maxepi];
		float* res = buffer;
		unsigned int numsamples = maxsamples;
		for (unsigned int i = 1; i <= maxepi; i++) {
			ABF_ReadChannel(hfile, &fh, c, i, res, &numsamples, &error);
			res += numsamples;
		}
		data.clear();
		data = std::vector<float>(buffer, res);
	}
	else if (fh.nOperationMode == 5 && m) {
		buffer = new float[maxsamples];
		ABF_ReadChannel(hfile, &fh, c, s, buffer, &maxsamples, &error);
		data = std::vector<float>(buffer, buffer + maxsamples);
	}
	else if (fh.nOperationMode == 3 && !m) {
		buffer = new float[maxsamples * maxepi];
		float* res = buffer;
		for (unsigned int i = 1; i <= maxepi; i++) {
			ABF_GetWaveform(hfile, &fh, c, i, res, &error);
			res += maxsamples;
		};
		data = std::vector<float>(buffer, buffer + fh.lActualAcqLength / fh.nADCNumChannels);
	}
	else if (fh.nOperationMode == 5 && !m) {
		buffer = new float[maxsamples];
		ABF_GetWaveform(hfile, &fh, c, s, buffer, &error);
		data = std::vector<float>(buffer, buffer + maxsamples);
	}
	ABF_Close(hfile, &error);
	delete[] buffer;
	return;
}

void ABF::save(std::vector<unsigned int>& start, std::vector<unsigned int>& end) {
	if (fh.nOperationMode != 3) {
		return;
	}
	int phfile = 0;
	std::string fnout = "_cut.abf";
	fnout.insert(0, fn, 0, fn.size() - 4);
	unsigned int numsamples = maxsamples;
	ABF_ReadOpen(fn.c_str(), &hfile, ABF_DATAFILE, &fh, &numsamples, &maxepi, &error);
	ABF_WriteOpen(fnout.c_str(), &phfile, ABF_DATAFILE, &fh, &error);
	if (fh.nDataFormat == 0) {
		short* buffer = new short[maxsamples * maxepi * Channel];
		short* res = buffer;
		for (unsigned int i = 1; i <= maxepi; i++) {
			ABF_MultiplexRead(hfile, &fh, i, res, &numsamples, &error);
			res += numsamples;
		}
		int a = res - buffer;
		int i = 0;
		int j = 0;
		int flag = Channel * start[j];
		while(i < res - buffer) {
			if (flag >= Channel * end[j]) {
				j++;
				if (j >= start.size()) {
					break;
				}
				flag = Channel * start[j];
			}
			if (flag > i) {
				for (int n = 0; n < Channel; n++) {
					buffer[i + n] = buffer[flag + n];
				}
			}
			flag += Channel;
			i += Channel;
		}
		ABF_MultiplexWrite(phfile, &fh, ABF_DATAFILE, buffer, 0, i, &error);
		delete[] buffer;
	}
	else if (fh.nDataFormat == 1) {
		float* buffer = new float[maxsamples * maxepi * Channel];
		float* res = buffer;
		for (unsigned int i = 1; i <= maxepi; i++) {
			ABF_MultiplexRead(hfile, &fh, i, res, &numsamples, &error);
			res += numsamples;
		}
		int i = 0;
		int j = 0;
		int flag = Channel*start[j];
		while (i < res - buffer) {
			if (flag >= Channel*end[j]) {
				j++;
				if (i >= start.size()) {
					break;
				}
				flag = Channel*start[j];
			}
			if (flag > i) {
				for (int n = 0; n < Channel; n++) {
					buffer[i + n] = buffer[flag + n]; 
				}
			}
			flag+=Channel;
			i+=Channel;
		}
		ABF_MultiplexWrite(phfile, &fh, ABF_DATAFILE, buffer, 0, i, &error);
		delete[] buffer;
	}
	ABF_UpdateHeader(phfile, &fh, &error);
	ABF_Close(hfile, &error);
	ABF_Close(phfile, &error);
	return;
}

void ABF::setSeries(QLineSeries* s) {
	series = s;
}

void ABF::draw(int s, int e) {
	if (series == NULL) 
		return;
	int n = e-s;
	int skip = (n / 3000 == 0) ? 1 : n / 3000;
	QVector<QPointF> interVariables;
	int valmax;
	int i = s;
	for (; i < e; i += skip) {
		int j = i;
		int max = j;
		int min = j;
		for (; j < i + skip && j < e; j++) {
			max = (data[max] > data[j]) ? max : j;
			min = (data[min] < data[j]) ? min : j;
		}
		interVariables.append(QPointF(min * Interval / 1000, data[min]));
		interVariables.append(QPointF(max * Interval / 1000, data[max]));
	}
	series->replace(interVariables);
}

std::pair<float, float> ABF::getLimit() {
	float min = 0;
	float max = data[0];
	for (int i = 1; i < data.size(); i++) {
		if (data[i] < min) {
			min = data[i];
		}
		else if (data[i] > max) {
			max = data[i];
		}
	}
	return std::pair<float, float>(min, max);
}
	
