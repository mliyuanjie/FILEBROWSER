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
	size_t index;
	float start;
	float end;
	float currentmax;
	float currentbase;
	float Imax;
	float Imin;
	float volume;
	float shape;	
};

class NPS: public QObject {
	Q_OBJECT;	

public slots:
	void trace(float, float);
	//void multiFit();
	//void singleFit(int);
	//void save();
	//void setPhysics(float, float, float, float, float);
	void load(std::string);
	void pretrace();
	void nexttrace();
	void prehist();
	void nexthist();
	void setIndex(int);
	void setBin(int);


signals:
	void sendtrace(QVector<QPointF>);
	void sendsig(QVector<QPointF>);
	void sendhist(QVector<QPointF>);
	void sendcursig(QVector<QPointF>);
	void sendaxis(float, float, float, float);
	void sendhistaxis(float, float, float, float);
	void sendtracenum(int);
	void sendtracecur(int);
	void sendsignum(int);
	void sendindex(QString);

private:
	std::vector<std::string> filelist;
	std::vector<Peak> siglist;
	std::unordered_map<int, std::pair<int,int>> mymap;
	std::vector<float> data;
	float interval;
	int index = 0;
	int bin = 50;
	int counter = -1;
	typedef int(_stdcall* pABF_ReadOpen)(const char* szFileName, int* phFile, UINT uFlags, ABFFileHeader* pFH, UINT* puMaxSamples, DWORD* pdwMaxEpi, int* pnError);
	typedef int(_stdcall* pABF_ReadChannel)(int hFile, ABFFileHeader* pFH, int nChannel, unsigned long dwEpisode, float* pfBuffer, unsigned int* puNumsAMPLES, int* pnError);
	typedef int(_stdcall* pABF_Close)(int hFile, int* pnError);
	void loaddata();
	void hist(int, int);
};

#endif 