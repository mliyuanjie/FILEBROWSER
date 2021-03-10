#include "nps.h" 
//#include <Python.h>
//#include <ndarrayobject.h>
#include <gsl/gsl_histogram.h>

void NPS::load(std::string fn) {
	std::ifstream file;
	file.open(fn, std::ios::binary);
	size_t length, size;
	int j = 0;
	while (1) {
		file.read(reinterpret_cast<char*>(&length), sizeof(size_t));
		if (file.eof())
			break;
		char* filename = new char[length];
		file.read(filename, length);
		filelist.push_back(std::string(filename, length));
		file.read(reinterpret_cast<char*>(&size), sizeof(size_t));
		Peak* buffer = new Peak[size];
		file.read(reinterpret_cast<char*>(buffer), size * sizeof(Peak));
		for (int i = 0; i < size; i++) 
			siglist.push_back(buffer[i]);
		mymap[j] = std::pair<int, int>(siglist.size() - size, siglist.size());
		j++;
		delete[] filename;
		delete[] buffer;
		
	}
	file.close();
	if (filelist.size() > 0) {
		counter = 0;
		loaddata();
		hist(0, 50);
		emit sendtracenum(filelist.size());
		emit sendsignum(siglist.size());
		emit sendtracecur(counter);
	}	
	return;
}

void NPS::trace(float xmin, float xmax) {
	unsigned int s = (xmin * 1000 / interval >= 0) ? xmin * 1000 / interval : 0;
	unsigned int e = (xmax * 1000 / interval <= data.size() && xmax * 1000 / interval >= 0) ? xmax * 1000 / interval : data.size();
	unsigned int n = e - s;
	unsigned int skip = n / 1500;
	skip = (skip == 0) ? 1 : skip;
	QVector<QPointF> point;
	for (int i = s; i < e; i += skip) {
		if (i + skip > e)
			skip = data.size() - i;
		std::pair<std::vector<float>::iterator, std::vector<float>::iterator> it = std::minmax_element(data.begin() + i, data.begin() + i + skip);
		if (it.first <= it.second) {
			point.append(QPointF((it.first - data.begin()) * interval / 1000, *(it.first)));
			point.append(QPointF((it.second - data.begin()) * interval / 1000, *(it.second)));
		}
		else {
			point.append(QPointF((it.second - data.begin()) * interval / 1000, *(it.second)));
			point.append(QPointF((it.first - data.begin()) * interval / 1000, *(it.first)));
		}
	}
	emit sendtrace(point);
}

void NPS::loaddata() {
	std::string suffix = filelist[counter].substr(filelist[counter].size() - 3, 3);
	std::string a = filelist[counter];
	int size;
	if (suffix == std::string("dat")) {
		interval = 2;
		std::ifstream file;
		file.open(filelist[counter], std::ios::binary);
		file.seekg(0, std::ios_base::end);
		size = file.tellg() / sizeof(float);
		file.seekg(0, std::ios::beg);
		float* buffer = new float[size];
		file.read(reinterpret_cast<char*>(buffer), size * sizeof(float));
		file.close();
		data = std::vector<float>(buffer, buffer + size);
		delete[] buffer;
	}
	else if (suffix == std::string("abf")) {
		HINSTANCE module = LoadLibraryA("ABFFIO.dll");
		pABF_ReadOpen ABF_ReadOpen = (pABF_ReadOpen)GetProcAddress(module, "ABF_ReadOpen");
		pABF_Close ABF_Close = (pABF_Close)GetProcAddress(module, "ABF_Close");
		pABF_ReadChannel ABF_ReadChannel = (pABF_ReadChannel)GetProcAddress(module, "ABF_ReadChannel");
		ABFFileHeader fh;
		int hfile;
		int error;
		unsigned int maxsamples;
		unsigned long maxepi;
		ABF_ReadOpen(filelist[counter].c_str(), &hfile, ABF_DATAFILE, &fh, &maxsamples, &maxepi, &error);
		interval = fh.fADCSequenceInterval;
		float* buffer;
		if (fh.nOperationMode == 3) {
			buffer = new float[maxsamples * maxepi];
			float* res = buffer;
			unsigned int numsamples = maxsamples;
			for (unsigned int i = 1; i <= maxepi; i++) {
				ABF_ReadChannel(hfile, &fh, 0, i, res, &numsamples, &error);
				res += numsamples;
			}
			size = res - buffer;
			data = std::vector<float>(buffer, buffer + size);
		}
		else if (fh.nOperationMode == 5) {
			buffer = new float[maxsamples];
			ABF_ReadChannel(hfile, &fh, 0, 1, buffer, &maxsamples, &error);
			size = maxsamples;
			data = std::vector<float>(buffer, buffer + size);
		}
		delete[] buffer;
		ABF_Close(hfile, &error);
		FreeLibrary(module);
	}
	float xmin = 0;
	float xmax = size * interval / 1000;
	std::pair<std::vector<float>::iterator, std::vector<float>::iterator> y;
	y = std::minmax_element(data.begin(), data.end());
	emit sendaxis(xmin, xmax, *y.first - (*y.second - *y.first), *y.second + (*y.second - *y.first));
	trace(xmin, xmax);
	QVector<QPointF> r;
	for (int i = mymap[counter].first; i < mymap[counter].second; i++) {
		r.push_back(QPointF(siglist[i].start, siglist[i].currentbase));
		r.push_back(QPointF(siglist[i].start, siglist[i].currentmax));
		r.push_back(QPointF(siglist[i].end, siglist[i].currentmax));
		r.push_back(QPointF(siglist[i].end, siglist[i].currentbase));
	}
	emit sendsig(r);
	return;
}
/*
void NPS::singleFit(int n) {
	int c = counter;
	while (n < mymap[c].first || n >= mymap[c].second) {
		if (n < mymap[c].first)
			c--;
		else
			c++;
		if (c >= filelist.size() || c < 0)
			return;
	}
	Peak peak = siglist[n];
	PyObject* pModule, * pFunc, * pArgs, * pValue;
	Py_Initialize();
	pModule = PyImport_ImportModule("pyalgorithm");
	pFunc = PyObject_GetAttrString(pModule, "hist");
	size_t start, end;
	start = peak.start * 1000 / interval;
	end = peak.end * 1000 / interval;
	double* buffer = new double[end-start + 1];
	for (int i = start; i <= end; i++)
		buffer[i - start] = data[i];
	npy_intp dims[1] = { end - start + 1 };
	PyObject* args = PyArray_SimpleNewFromData(1, dims, NPY_FLOAT64, (void*)buffer);
	pArgs = PyTuple_New(1);
	PyTuple_SetItem(pArgs, 0, args);
	PyArray_ENABLEFLAGS((PyArrayObject*)args, NPY_ARRAY_OWNDATA);
	PyObject_CallObject(pFunc, args);
	Py_DECREF(pModule);
	Py_DECREF(pFunc);
	Py_DECREF(pArgs);
	Py_Finalize();
	return;
}

void NPS::multiFit() {
	std::vector<double> buffer;
	for (int i = 0; i < siglist.size(); i++) 
		buffer.push_back(siglist[i].currentmax);
	PyObject* pModule, * pFunc, * pArgs, * pValue;
	Py_Initialize();
	pModule = PyImport_ImportModule("pyalgorithm");
	pFunc = PyObject_GetAttrString(pModule, "hist");
	npy_intp dims[1] = { buffer.size() };
	PyObject* args = PyArray_SimpleNewFromData(1, dims, NPY_FLOAT64, (void*)buffer.data());
	pArgs = PyTuple_New(1);
	PyTuple_SetItem(pArgs, 0, args);
	PyObject_CallObject(pFunc, args);
	Py_DECREF(pModule);
	Py_DECREF(pFunc);
	Py_DECREF(pArgs);
	Py_Finalize();
	return;
}
*/
void NPS::hist(int n, int bin) {
	if (counter < 0)
		return;
	if (n >= 0) {
		int c = counter;
		while (n < mymap[c].first || n >= mymap[c].second) {
			if (n < mymap[c].first)
				c--;
			else
				c++;
			if (c >= filelist.size() || c < 0)
				return;
		}
		if (c != counter) {
			counter = c;
			loaddata();
		}	
		Peak peak = siglist[n];
		gsl_histogram* h = gsl_histogram_alloc(bin);
		size_t start, end;
		start = peak.start * 1000 / interval;
		end = peak.end * 1000 / interval;
		double xmax = (peak.currentbase - *std::max_element(data.begin() + start, data.begin() + end + 1)) / peak.currentbase;
		double xmin = (peak.currentbase - *std::min_element(data.begin() + start, data.begin() + end + 1)) / peak.currentbase;
		if (xmax < xmin)
			std::swap(xmin, xmax);
		gsl_histogram_set_ranges_uniform(h, xmin, xmax);
		for (int i = start; i <= end; i++)
			gsl_histogram_increment(h, (peak.currentbase - data[i]) / peak.currentbase);
		QVector<QPointF> r;
		int maxcount = 0;
		for (int i = 0; i < h->n; i++) {
			maxcount = (h->bin[i] > maxcount) ? h->bin[i] : maxcount;
			r.push_back(QPointF(h->range[i], 0));
			r.push_back(QPointF(h->range[i], h->bin[i]));
			r.push_back(QPointF(h->range[i+1], h->bin[i]));
			r.push_back(QPointF(h->range[i+1], 0));
		}
		emit sendhistaxis(xmin, xmax, 0, maxcount);
		emit sendhist(r);
		QVector<QPointF> r2;
		r2.push_back(QPointF(peak.start, peak.currentbase));
		r2.push_back(QPointF(peak.start, peak.currentmax));
		r2.push_back(QPointF(peak.end, peak.currentmax));
		r2.push_back(QPointF(peak.end, peak.currentbase));
		emit sendcursig(r2);
		emit sendindex(QString::number(n));
		gsl_histogram_free(h);
	}
	else {
		std::vector<double> multievent;
		double xmax, xmin;
		for (int i = 0; i < siglist.size(); i++) {
			multievent.push_back((siglist[i].currentbase - siglist[i].currentmax) / siglist[i].currentbase);
			if (i == 0) {
				xmax = multievent[0];
				xmin = multievent[0];
			}
			else {
				xmax = (xmax < multievent[i]) ? multievent[i] : xmax;
				xmin = (xmin > multievent[i]) ? multievent[i] : xmin;
			}
		}
		gsl_histogram* h = gsl_histogram_alloc(bin);
		gsl_histogram_set_ranges_uniform(h, xmin, xmax);
		for (int i = 0; i < multievent.size(); i++)
			gsl_histogram_increment(h, multievent[i]);
		QVector<QPointF> r;
		int maxcount = 0;
		for (int i = 0; i < h->n; i++) {
			maxcount = (h->bin[i] > maxcount) ? h->bin[i] : maxcount;
			r.push_back(QPointF(h->range[i], 0));
			r.push_back(QPointF(h->range[i], h->bin[i]));
			r.push_back(QPointF(h->range[i + 1], h->bin[i]));
			r.push_back(QPointF(h->range[i + 1], 0));
		}
		emit sendhistaxis(xmin, xmax, 0, maxcount);
		emit sendhist(r);
		gsl_histogram_free(h);
	}
}

void NPS::setBin(int i) {
	bin = (i > 0) ? i : 50;
	hist(index, bin);
}

void NPS::setIndex(int i) {
	index = (i >= -1) ? i : 0;
	hist(index, bin);
}

void NPS::pretrace() {
	if (counter <= 0)
		return;
	counter--;
	loaddata();
	index = mymap[counter].first;
	hist(index, bin);
	emit sendindex(QString::number(index));
}

void NPS::nexttrace() {
	if (counter >= filelist.size() - 1)
		return;
	counter++;
	loaddata();
	index = mymap[counter].first;
	hist(index, bin);
	emit sendindex(QString::number(index));
}

void NPS::prehist() {
	if (index <= 0)
		return;
	index--;
	hist(index, bin);
	emit sendindex(QString::number(index));
}

void NPS::nexthist() {
	if (index >= siglist.size() - 1)
		return;
	index++;
	hist(index, bin);
	emit sendindex(QString::number(index));
}