#ifndef NPS_H
#define NPS_H

#include <vector>
#include <string>
#include <unordered_map>
#include <QtCore/qobject.h>
#include <QtCore/qvector.h>
#include <fstream>
#include <algorithm>
#include "abffiles.h"

struct Peak {
	bool eventtype;
	unsigned int start;
	unsigned int end;
	float currentmax;
	float currentbase;
	float Imax;
	float Imin;
	float volume;
	float shape;	
};

class NPS: QObject {
	Q_OBJECT;	

public slots:
	void trace(float, float);
	void setCounter(int);
	void multiFit();
	void singleFit(int);
	void save();
	void setPhysics(float, float, float, float, float);
	void load(std::string&);

signals:
	void sendtrace(QVector<QPointF>);
	void sendaxis(float, float, float, float);

private:
	std::vector<std::string> filelist;
	std::vector<Peak> siglist;
	std::unordered_map<int, std::pair<int,int>> datatosig;
	std::unordered_map<int, int> sigtodata;
	std::vector<float> data;
	float interval;
	int counter = -1;
	typedef int(_stdcall* pABF_ReadOpen)(const char* szFileName, int* phFile, UINT uFlags, ABFFileHeader* pFH, UINT* puMaxSamples, DWORD* pdwMaxEpi, int* pnError);
	typedef int(_stdcall* pABF_ReadChannel)(int hFile, ABFFileHeader* pFH, int nChannel, unsigned long dwEpisode, float* pfBuffer, unsigned int* puNumsAMPLES, int* pnError);
	typedef int(_stdcall* pABF_Close)(int hFile, int* pnError);
	void loaddata();
};

#endif 