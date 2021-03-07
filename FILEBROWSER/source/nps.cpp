#include "nps.h" 
#include <Python.h>
#include <ndarrayobject.h>

void NPS::load(std::string& fn) {
	std::ifstream file;
	file.open(fn, std::ios::binary);
	size_t number, length, size;
	file.read(reinterpret_cast<char*>(&number), sizeof(size_t));
	int k = 0;
	for (int i = 0; i < number; i++) {
		file.read(reinterpret_cast<char*>(&length), sizeof(size_t));
		char* filename = new char[length];
		file.read(filename, length);
		file.read(reinterpret_cast<char*>(&size), sizeof(size_t));
		Peak peak;
		for (int j = 0; j < size; j++) {
			file.read(reinterpret_cast<char*>(&peak), sizeof(Peak));
			siglist.push_back(peak);
			sigtodata[k] = i;
			k++;
		}
		filelist.push_back(std::string(filename));
		datatosig[i] = std::pair<int, int>(k-size, size);
		delete[] filename;
	}
	file.close();
	if (filelist.size() > 0) {
		counter = 0;
		loaddata();
		trace(0, data.size() * interval / 1000);
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
	for (int i = 0; i < data.size(); i + skip) {
		if (i + skip > data.size())
			skip = data.size() - i;
		std::pair<std::vector<float>::iterator, std::vector<float>::iterator> it = std::minmax_element(data.begin(), data.begin() + skip);
		if (it.first <= it.second) {
			point.push_back(QPointF((it.first - data.begin()) * interval / 1000, *it.first));
			point.push_back(QPointF((it.second - data.begin()) * interval / 1000, *it.second));
		}
		else {
			point.push_back(QPointF((it.second - data.begin()) * interval / 1000, *it.second));
			point.push_back(QPointF((it.first - data.begin()) * interval / 1000, *it.first));
		}
	}
	emit sendtrace(point);
}

void NPS::loaddata() {
	if (counter < 0)
		return;
	std::string suffix = filelist[counter].substr(filelist[counter].size() - 3, 3);
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
	emit sendaxis(xmin, xmax, *y.first - 3 * (*y.second - *y.first), *y.second + 3 * (*y.second - *y.first));
	return;
}

void NPS::setCounter(int n) {
	counter = n;
	loaddata();
	trace(0, data.size() * interval / 1000);
}

void NPS::singleFit(int n) {
	if (sigtodata[n] != counter) {
		setCounter(counter);
	}
	if (!siglist[n].eventtype)
		return;
	Peak peak = siglist[n];
	PyObject* pModule, * pFunc, * pArgs, * pValue;
	Py_Initialize();
	pModule = PyImport_ImportModule("pyalgorithm");
	pFunc = PyObject_GetAttrString(pModule, "hist");
	double* buffer = new double[peak.end - peak.start + 1];
	for (int i = peak.start; i <= peak.end; i++)
		buffer[i - peak.start] = data[i];
	npy_intp dims[1] = { peak.end - peak.start + 1 };
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
		if (siglist[i].eventtype)
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