#include "abf.h"
#include "algorithm.h"

ABF::ABF(std::string f, QObject* parent, unsigned int n) : QObject(parent) {
	fn = f;
	if (fn.substr(fn.length() - 3, 3) == "dat") {
		Channel = 0;
		Sweep = 1;
		Interval = 2;
		filetype = false;
		return;
	}
	error = 0;
	hfile = 1;
	maxsamples = n;
	maxepi = 0;
	module = LoadLibraryA("ABFFIO.dll");
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
	filetype = true;
	ABF_Close(hfile, &error);
}

ABF::~ABF(){
	if (fn.substr(fn.length() - 3, 3) == "abf")
		FreeLibrary(module);
	if (data != NULL) {
		gsl_vector_free(data);
		data = NULL;
	}
}

void ABF::readData(int c, int s, bool m) {
	if (data != NULL) 
		gsl_vector_free(data);
	if (!filetype) {
		std::ifstream file;
		file.open(fn, std::ios::binary);
		file.seekg(0, std::ios_base::end);
		size = file.tellg() / sizeof(float);
		file.seekg(0, std::ios::beg);
		float* buffer = new float[size];
		file.read(reinterpret_cast<char*>(buffer), size * sizeof(float));
		file.close();
		data = gsl_vector_alloc(size);
		for (int i = 0; i < size; i++) {
			gsl_vector_set(data, i, buffer[i]);
		}
		delete[] buffer;
		float xmin = 0;
		float xmax = size * Interval / 1000;
		double ymin, ymax;
		gsl_vector_minmax(data, &ymin, &ymax);
		emit sendAxis(xmin, xmax, ymin - 3 * (ymax - ymin), ymax + 3 * (ymax - ymin));
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
		size = res - buffer;
		data = gsl_vector_alloc(size);
		for (int i = 0; i < size; i++) {
			gsl_vector_set(data, i, buffer[i]);
		}
	}
	else if (fh.nOperationMode == 5 && m) {
		buffer = new float[maxsamples];
		ABF_ReadChannel(hfile, &fh, c, s, buffer, &maxsamples, &error);
		size = maxsamples;
		data = gsl_vector_alloc(size);
		for (int i = 0; i < size; i++) {
			gsl_vector_set(data, i, buffer[i]);
		}
	}
	/*
	else if (fh.nOperationMode == 3 && !m) {
		buffer = new float[maxsamples * maxepi];
		float* res = buffer;
		for (unsigned int i = 1; i <= maxepi; i++) {
			ABF_GetWaveform(hfile, &fh, c, i, res, &error);
			res += maxsamples;
		};
		data = gsl_vector_alloc(fh.lActualAcqLength / fh.nADCNumChannels);
		for (int i = 0; i < maxsamples; i++) {
			gsl_vector_set(data, i, buffer[i]);
		}
	}
	else if (fh.nOperationMode == 5 && !m) {
		buffer = new float[maxsamples];
		ABF_GetWaveform(hfile, &fh, c, s, buffer, &error);
		data = std::vector<float>(buffer, buffer + maxsamples);
	}
	*/
	ABF_Close(hfile, &error);
	delete[] buffer;
	float xmin = 0;
	float xmax = size * Interval / 1000;
	double ymin, ymax;
	gsl_vector_minmax(data, &ymin, &ymax);
	emit sendAxis(xmin, xmax, ymin - 3 * (ymax - ymin), ymax + 3 * (ymax - ymin));
	return;
}

void ABF::save(QVector<QPointF> point) {
	if (!filetype) {
		std::string fnout = "_cut.dat";
		fnout.insert(0, fn, 0, fn.size() - 4);
		std::ofstream file;
		file.open(fnout, std::ios::binary);
		unsigned int size = data->size;
		float* buffer = new float[size];
		int j = 0;
		int i = 0;
		int flag = point[0].x() * 1000 / Interval;
		while (i < size) {
			if (flag >= point[j].y() * 1000 / Interval) {
				j++;
				if (j >= point.size()) {
					break;
				}
				flag = point[j].x() * 1000 / Interval;
			}
			if (flag > i) {
				buffer[i] = gsl_vector_get(data, flag); 
			}
			flag++;
			i++;
		}
		file.write(reinterpret_cast<char*>(buffer), i * sizeof(float));
		file.close();
		delete[] buffer;
		return;
	}
	//if (fh.nOperationMode != 3) {
	//	return;
	//}
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
		int flag = Channel * point[j].x() * 1000 / Interval;
		while(i < res - buffer) {
			if (flag >= Channel * point[j].x() * 1000 / Interval) {
				j++;
				if (j >= point.size()) {
					break;
				}
				flag = Channel * point[j].y() * 1000 / Interval;
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
		int flag = Channel*point[j].x() * 1000 / Interval;
		while (i < res - buffer) {
			if (flag >= Channel*point[j].y() * 1000 / Interval) {
				j++;
				if (i >= point.size()) {
					break;
				}
				flag = Channel*point[j].x() * 1000 / Interval;
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

void ABF::draw(float xmin, float xmax) {
	start_time = xmin;
	end_time = xmax;
	unsigned int s = (xmin * 1000 / Interval >= 0) ? xmin * 1000 / Interval : 0;
	unsigned int e = (xmax * 1000 / Interval <= data->size && xmax * 1000 / Interval >=0) ? xmax * 1000 / Interval : data->size;
	unsigned int n = e-s;
	unsigned int skip = n / 1500;
	skip = (skip == 0) ? 1 : skip;
	unsigned int j;
	size_t max, min;
	QVector<QPointF> point;
	for (unsigned int i = s; i < e; i += skip) {
		j = (i + skip <= e) ? i + skip : e;
		gsl_vector_view tmp;
		tmp = gsl_vector_subvector(data, i, j-i);
		gsl_vector_minmax_index(&tmp.vector, &min, &max);
		min += i;
		max += i;
		if (max > min) {
			point.append(QPointF(min * Interval / 1000, data->data[min]));
			point.append(QPointF(max * Interval / 1000, data->data[max]));
		}
		else if (min > max) {
			point.append(QPointF(max * Interval / 1000, data->data[max]));
			point.append(QPointF(min * Interval / 1000, data->data[min]));
		}
		else {
			point.append(QPointF(max * Interval / 1000, data->data[max]));
		}
	}
	emit sendData(point);
		/*
		unsigned int max, min;
		QVector<QPointF> interVariables;
		for (unsigned int i = s; i < e; i += skip) {
			int j =
			gsl_vector_view tmp;
			if (i + skip > data->size)
				tmp = gsl_vector_subvector(data_f, i, data->size - i);
			else
				tmp = gsl_vector_subvector(data_f, i, skip);
			gsl_vector_minmax_index(&tmp.vector, &min, &max);
			min += i;
			max += i;
			if (max > min) {
				interVariables.append(QPointF(min * Interval / 1000, data_f->data[min]));
				interVariables.append(QPointF(max * Interval / 1000, data_f->data[max]));
			}
			else if (min > max) {
				interVariables.append(QPointF(max * Interval / 1000, data_f->data[max]));
				interVariables.append(QPointF(min * Interval / 1000, data_f->data[min]));
			}
			else {
				interVariables.append(QPointF(max * Interval / 1000, data_f->data[max]));
			}
		}
		emit sendData_f(interVariables);
		*/
	return;
}

void ABF::readSignal(float sigma, float freq, float thres) {
	sigs.clear();
	gsl_vector* tmp;
	if (sigma == 0)
		tmp = meanSmooth(data, freq);
	else
		tmp = gaussSmooth(data, sigma, freq);
	emit sendProcess(50);
	gsl_vector* data_f = meanSmooth(tmp, 2000);
	float mean, sd;
	std::vector<std::pair<int, int>> sig = findPeak(tmp->data, data_f->data, data_f->size, thres, sd, mean);
	QVector<QPointF> point;
	float val;
	unsigned s, e;
	NanoporeSig k;
	for (int i = 0; i < sig.size(); i++) {
		s = sig[i].first;
		e = sig[i].second;
		if (data->data[s] < 0)
			val = gsl_vector_max(&gsl_vector_subvector(data, s, e - s).vector);
		else 
			val = gsl_vector_min(&gsl_vector_subvector(data, s, e - s).vector);
		point.append(QPointF(s * Interval / 1000, data_f->data[s]));
		point.append(QPointF(s * Interval / 1000, val));
		point.append(QPointF(e * Interval / 1000, val));
		point.append(QPointF(e * Interval / 1000, data_f->data[e]));
		k.index = i + 1;
		k.start = s * Interval / 1000;
		k.end = e * Interval / 1000;
		k.current = val;
		k.baseline = (data_f->data[s] + data_f->data[e]) / 2;
		sigs.push_back(k);
	}
	emit sendProcess(90);
	emit sendData_f(point, mean, sd);
	gsl_vector_free(tmp);
	gsl_vector_free(data_f);
	std::string fnout = "csv";
	fnout.insert(0, fn, 0, fn.size() - 3);
	std::ofstream file;
	file.open(fnout);
	file << "Index Start(ms) End(ms) Current(pA) Baseline(pA)\n";
	for (int i = 0; i < sigs.size(); i++) {
		file << std::to_string(sigs[i].index) + " " + std::to_string(sigs[i].start) + " " + std::to_string(sigs[i].end) + " " + std::to_string(sigs[i].current) + " " + std::to_string(sigs[i].baseline) + "\n";
	}
	file.close();
	emit sendProcess(100);
}

void ABF::savenps(float a, float b, float c) {
	std::string filename(fn.substr(0, fn.find_last_of("/")) + std::string("/result.nps"));
	std::ofstream file;
	file.open(filename, std::fstream::app | std::ios::binary);
	size_t size = fn.size();
	file.write(reinterpret_cast<char*>(&size), sizeof(size_t));
	file.write(reinterpret_cast<char*>(&a), sizeof(float));
	file.write(reinterpret_cast<char*>(&b), sizeof(float));
	file.write(reinterpret_cast<char*>(&c), sizeof(float));
	file.write(fn.c_str(), size);
	size = sigs.size();
	file.write(reinterpret_cast<char*>(&size), sizeof(size_t));
	file.write(reinterpret_cast<char*>(sigs.data()), size * sizeof(NanoporeSig));
	file.close();
	emit sendProcess(0);
}

void ABF::removenps() {
	std::string filename(fn.substr(0, fn.find_last_of("/")) + std::string("/result.nps"));
	std::vector<std::string> filelist;
	std::vector<NanoporeSig> siglist;
	std::vector<Paras> paralist;
	std::vector<int> sizelist;
	std::ifstream file;
	file.open(filename, std::ios::binary);
	size_t length, size;
	while (1) {
		file.read(reinterpret_cast<char*>(&length), sizeof(size_t));
		if (file.eof())
			break;
		Paras para;
		file.read(reinterpret_cast<char*>(&para), sizeof(Paras));
		char* filename = new char[length];
		file.read(filename, length);
		filelist.push_back(std::string(filename, length));
		paralist.push_back(para);
		file.read(reinterpret_cast<char*>(&size), sizeof(size_t));
		if (size == (size_t)0) {
			filelist.pop_back();
			continue;
		}
		NanoporeSig* buffer = new NanoporeSig[size];
		file.read(reinterpret_cast<char*>(buffer), size * sizeof(NanoporeSig));
		for (int i = 0; i < size; i++)
			siglist.push_back(buffer[i]);
		sizelist.push_back(size);
		delete[] filename;
		delete[] buffer;
	}
	file.close();
	std::ofstream ofile;
	ofile.open(filename, std::ios::binary);
	int pos = 0;
	for (int i = 0; i < filelist.size() - 1;i++) {
		size_t size = filelist[i].size();
		ofile.write(reinterpret_cast<char*>(&size), sizeof(size_t));
		ofile.write(reinterpret_cast<char*>(&paralist[i]), sizeof(Paras));
		ofile.write(filelist[i].c_str(), size);
		size = sizelist[i];
		ofile.write(reinterpret_cast<char*>(&size), sizeof(size_t));
		ofile.write(reinterpret_cast<char*>(&siglist[0] + pos), size * sizeof(NanoporeSig));
		pos += size;
	}
	ofile.close();
	emit sendProcess(100);
}