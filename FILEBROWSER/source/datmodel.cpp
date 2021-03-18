#include "datmodel.h"
#include "tools.h"

void DATModel::load(std::string& f) {
	fn = f;
	dat.open(fn);
	interval = dat.interval();
	float xmin = 0;
	float xmax = dat.size() * interval / 1000;
	std::pair<float, float> y = dat.valminmax();
	emit sendAxis(xmin, xmax, y.first - 3 * (y.second - y.second), y.first + 3 * (y.second - y.first));
	return;
}

void DATModel::save(QVector<QPointF> point) {
	std::string fnout = "_cut.dat";
	fnout.insert(0, fn, 0, fn.size() - 4);
	std::ofstream file;
	file.open(fnout, std::fstream::app | std::ios::binary);
	float r = interval / 1000;
	file.write(reinterpret_cast<char*>(&r), sizeof(float));
	int i, j;
	for (int i = 0; i < point.size(); i++) {
		i = point[i].x() * 1000 / interval;
		j = point[i].y() * 1000 / interval;
		std::vector<float> res = std::move(dat.data(i, j, 1));
		file.write(reinterpret_cast<char*>(res.data()), res.size() * sizeof(float));
	}
	file.close();
	return;
}

void DATModel::draw(float xmin, float xmax) {
	unsigned int s = (xmin * 1000 / interval >= 0) ? xmin * 1000 / interval : 0;
	unsigned int e = (xmax * 1000 / interval <= n && xmax * 1000 / interval >=0) ? xmax * 1000 / interval : n;
	unsigned int n = e-s;
	unsigned int skip = n / 1500;
	skip = (skip == 0) ? 1 : skip;
	unsigned int j;
	QVector<QPointF> point;
	std::pair<float, float> x;
	for (unsigned int i = s; i < e; i += skip) {
		j = (i + skip <= e) ? i + skip : e;
		x = dat.valminmax(i, j);
		point.append(QPointF(i * interval / 1000, x.first));
		point.append(QPointF(i * interval / 1000, x.second));
	}
	emit sendData(point);
	return;
}

void DATModel::signal(float sigma, float freq, float thres) {
	sigs.clear();
	gsl_vector* data = gsl_vector_alloc(n);
	std::vector<float> t = std::move(dat.data());
	for (int i = 0; i < t.size(); i++)
		gsl_vector_set(data, i, t[i]);
	t.clear();
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
	NPSig k;
	for (int i = 0; i < sig.size(); i++) {
		s = sig[i].first;
		e = sig[i].second;
		if (data->data[s] < 0)
			val = gsl_vector_max(&gsl_vector_subvector(data, s, e - s).vector);
		else 
			val = gsl_vector_min(&gsl_vector_subvector(data, s, e - s).vector);
		point.append(QPointF(s * interval / 1000, data_f->data[s]));
		point.append(QPointF(s * interval / 1000, val));
		point.append(QPointF(e * interval / 1000, val));
		point.append(QPointF(e * interval / 1000, data_f->data[e]));
		NPSig k = { s * interval / 1000, e * interval / 1000, val, (data_f->data[s] + data_f->data[e]) / 2 };
		sigs.push_back(k);
	}
	emit sendProcess(90);
	emit sendData_f(point, mean, sd);
	gsl_vector_free(tmp);
	gsl_vector_free(data_f);
	gsl_vector_free(data);
	emit sendProcess(100);
}

void DATModel::savenps(float a, float b, float c) {
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
	file.write(reinterpret_cast<char*>(sigs.data()), size * sizeof(NPSig));
	file.close();
	emit sendProcess(0);
}

void DATModel::removenps() {
	std::string filename(fn.substr(0, fn.find_last_of("/")) + std::string("/result.nps"));
	std::vector<std::string> filelist;
	std::vector<NPSig> siglist;
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
		NPSig* buffer = new NPSig[size];
		file.read(reinterpret_cast<char*>(buffer), size * sizeof(NPSig));
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
		ofile.write(reinterpret_cast<char*>(&siglist[0] + pos), size * sizeof(NPSig));
		pos += size;
	}
	ofile.close();
	emit sendProcess(100);
}