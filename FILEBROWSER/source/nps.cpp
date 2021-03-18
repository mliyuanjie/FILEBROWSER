#include "nps.h" 
#include <gsl/gsl_histogram.h>

void NPS::load(std::string f) {
	fn = f;
	if (fn.substr(fn.length() - 3, 3) == "mat") {
		MATFile* pmatFile = NULL;
		mxArray* pMxArray = NULL;
		pmatFile = matOpen(fn.c_str(), "r");
		int M, N;
		size_t strlen;
		int start, len;
		pMxArray = matGetVariable(pmatFile, "PeaksAll");
		M = (int)mxGetM(pMxArray);
		N = (int)mxGetN(pMxArray);
		for (int i = 0; i < M; i++) {			
			mxArray* matcell = mxGetCell(mxGetCell(pMxArray, i),0);
			char* matfn = mxArrayToString(matcell);
			std::string cppfn(matfn);
			std::string filename = fn.substr(0, fn.find_last_of("/") + 1) + cppfn.substr(0, cppfn.find_last_of(","));
			std::string strstart = cppfn.substr(cppfn.find_last_of(",") + 1, cppfn.size() - cppfn.find_last_of(",") - 1);
			start = std::stoi(strstart);
			len = mxGetM(mxGetCell(mxGetCell(pMxArray, i + M),0));
			Peakmat peakmat = { start, start + len };
			siglist_mat.push_back(peakmat);
			if (mymap.find(filename) == mymap.end()) {
				filelist.push_back(filename);
				mymap[filename] = std::pair<int, int>(siglist_mat.size()-1, siglist_mat.size());
			}
			else {
				mymap[filename].second++;
			}	
			mxFree(matfn);
		}
		matClose(pmatFile);
		
		if (filelist.size() > 0) {
			counter = 0;
			loaddata();
			hist(0, 50);
			emit sendtracenum(filelist.size() - 1);
			emit sendsignum(siglist_mat.size() - 1);
		}
	}
	else if (fn.substr(fn.length() - 3, 3) == "nps") {
		std::ifstream file;
		file.open(fn, std::ios::binary);
		size_t length, size;
		int j = 0;
		while (1) {
			file.read(reinterpret_cast<char*>(&length), sizeof(size_t));
			if (file.eof())
				break;
			Para para;
			file.read(reinterpret_cast<char*>(&para), sizeof(Para));
			char* filename = new char[length];
			file.read(filename, length);
			filelist.push_back(std::string(filename, length));
			if (mymap.find(filelist.back()) != mymap.end()) {
				filelist.pop_back();
				continue;
			}
			paralist.push_back(para);
			file.read(reinterpret_cast<char*>(&size), sizeof(size_t));
			if (size == (size_t)0) {
				filelist.pop_back();
				continue;
			}
			Peak* buffer = new Peak[size];
			file.read(reinterpret_cast<char*>(buffer), size * sizeof(Peak));
			for (int i = 0; i < size; i++)
				siglist.push_back(buffer[i]);
			mymap[filelist.back()] = std::pair<int, int>(siglist.size() - size, siglist.size());
			j++;
			delete[] filename;
			delete[] buffer;
		}
		file.close();
		if (filelist.size() > 0) {
			counter = 0;
			loaddata();
			hist(0, 50);
			emit sendtracenum(filelist.size() - 1);
			emit sendsignum(siglist.size() - 1);
		}
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
	float ymin = *y.first - (*y.second - *y.first);
	float ymax = *y.second + (*y.second - *y.first);
	emit sendaxis(xmin, xmax, ymin, ymax);
	trace(xmin, xmax);
	QVector<QPointF> r;
	if (fn.substr(fn.length() - 3, 3) == "mat") {
		int start, end;
		float current;
		for (int i = mymap[filelist[counter]].first; i < mymap[filelist[counter]].second; i++) {
			start = siglist_mat[i].start;
			end = siglist_mat[i].end;
			std::vector<float>::iterator current = std::max_element(data.begin() + start, data.begin() + end + 1);
			r.push_back(QPointF(start * interval / 1000, data[start]));
			r.push_back(QPointF(start * interval / 1000, *current));
			r.push_back(QPointF(end * interval / 1000, *current));
			r.push_back(QPointF(end * interval / 1000, data[end]));
		}

	}
	else if (fn.substr(fn.length() - 3, 3) == "nps") {
		for (int i = mymap[filelist[counter]].first; i < mymap[filelist[counter]].second; i++) {
			r.push_back(QPointF(siglist[i].start, siglist[i].currentbase));
			r.push_back(QPointF(siglist[i].start, siglist[i].currentmax));
			r.push_back(QPointF(siglist[i].end, siglist[i].currentmax));
			r.push_back(QPointF(siglist[i].end, siglist[i].currentbase));
		}
	}	
	emit sendsig(r);
	emit sendtracecur(counter);
	return;
}
/*
void NPS::fit() {
	if (index >= 0) {
		Peak peak = siglist[index];
		gsl_histogram* h = gsl_histogram_alloc(bin);
		size_t start, end;
		start = peak.start * 1000 / interval;
		end = peak.end * 1000 / interval;
		double xmax = peak.currentbase - *std::max_element(data.begin() + start, data.begin() + end + 1);
		double xmin = peak.currentbase - *std::min_element(data.begin() + start, data.begin() + end + 1);
		if (xmax < xmin)
			std::swap(xmin, xmax);
		gsl_histogram_set_ranges_uniform(h, xmin, xmax);
		for (int i = start; i <= end; i++)
			gsl_histogram_increment(h, peak.currentbase - data[i]);
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
		QVector<QPointF> r2;
		r2.push_back(QPointF(peak.start, peak.currentbase));
		r2.push_back(QPointF(peak.start, peak.currentmax));
		r2.push_back(QPointF(peak.end, peak.currentmax));
		r2.push_back(QPointF(peak.end, peak.currentbase));
		emit sendcursig(r2);
		emit sendindex(QString::number(index));
		npfdata d = { h->n, h->range, h->bin };
		double para[6] = {maxcount, maxcount, xmin, xmax, 1.0, 1.0};
		gsl_vector_view p = gsl_vector_view_array(para, 6);
		double chisq;
		gsl_matrix* covar = gsl_matrix_alloc(6, 6);
		npfit(&d, &p.vector, covar, chisq);
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
	emit sendindex(QString::number(index));
}
*/
void NPS::hist(int n, int bin) {
	if (n >= 0) {
		int c = counter;
		while (n < mymap[filelist[c]].first || n >= mymap[filelist[c]].second) {
			if (n < mymap[filelist[c]].first)
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
		size_t start, end;
		double xmax, xmin;
		gsl_histogram* h = gsl_histogram_alloc(bin);
		QVector<QPointF> r2;
		if (fn.substr(fn.length() - 3, 3) == "mat") {
			Peakmat peak = siglist_mat[n];
			start = peak.start;
			end = peak.end;
			std::pair<std::vector<float>::iterator, std::vector<float>::iterator> x;
			x = std::minmax_element(data.begin() + start, data.begin() + end + 1);
			xmin = *x.first;
			xmax = *x.second;
			gsl_histogram_set_ranges_uniform(h, xmin, xmax);
			for (int i = start; i <= end; i++)
				gsl_histogram_increment(h, data[i]);
			r2.push_back(QPointF(start * interval / 1000, data[start]));
			r2.push_back(QPointF(start * interval / 1000, xmax));
			r2.push_back(QPointF(end * interval / 1000, xmax));
			r2.push_back(QPointF(end * interval / 1000, data[end]));
		}
		else if (fn.substr(fn.length() - 3, 3) == "nps") {
			Peak peak = siglist[n];
			start = peak.start * 1000 / interval;
			end = peak.end * 1000 / interval;
			xmax = peak.currentbase - *std::max_element(data.begin() + start, data.begin() + end + 1);
			xmin = peak.currentbase - *std::min_element(data.begin() + start, data.begin() + end + 1);
			if (xmax < xmin)
				std::swap(xmin, xmax);
			gsl_histogram_set_ranges_uniform(h, xmin, xmax);
			for (int i = start; i <= end; i++)
				gsl_histogram_increment(h, peak.currentbase - data[i]);
		}
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
		emit sendcursig(r2);
		emit sendindex(QString::number(n));
		gsl_histogram_free(h);
	}
	else {
		std::vector<double> multievent;
		double xmax, xmin;
		if (fn.substr(fn.length() - 3, 3) == "mat") 
			return;
		else if (fn.substr(fn.length() - 3, 3) == "nps") {
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
	emit sendindex(QString::number(n));
}

void NPS::setBin(int i) {
	if (i <= 0)
		return;
	bin = i;
	hist(index, bin);
}

void NPS::setIndex(int i) {
	if ((i < -1 || i >= siglist.size())&& (i < -1 || i >= siglist_mat.size()))
		return;
	index = i;
	hist(index, bin);
}

void NPS::pretrace() {
	if (counter <= 0)
		return;
	counter--;
	loaddata();
	index = mymap[filelist[counter]].first;
	hist(index, bin);
}

void NPS::nexttrace() {
	if (counter >= filelist.size() - 1)
		return;
	counter++;
	loaddata();
	index = mymap[filelist[counter]].first;
	hist(index, bin);
	
}

void NPS::prehist() {
	if (index <= -1)
		return;
	index--;
	hist(index, bin);
}

void NPS::nexthist() {
	if ((index < -1 || index >= siglist.size()) && (index < -1 || index >= siglist_mat.size()))
		return;
	index++;
	hist(index, bin);
}

void NPS::sethist(float t) {
	if (counter < 0)
		return;
	size_t start = mymap[filelist[counter]].first;
	size_t end = mymap[filelist[counter]].second;
	size_t mid;
	if (fn.substr(fn.length() - 3, 3) == "nps") {
		while (true) {
			mid = (start + end) / 2;
			if (t >= siglist[mid].start && t <= siglist[mid].end)
				break;
			else if (t < siglist[mid].start)
				end = mid;
			else
				start = mid + 1;
			if (start >= end)
				return;
		}
	}
	else {
		t = 1000 * t / interval;
		while (true) {
			mid = (start + end) / 2;
			if (t >= siglist_mat[mid].start && t <= siglist_mat[mid].end)
				break;
			else if (t < siglist_mat[mid].start)
				end = mid;
			else
				start = mid + 1;
			if (start >= end)
				return;
		}
	}
	index = mid;
	hist(index, bin);
}